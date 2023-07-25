/***************************************************************************//**
  @file     timer.c
  @brief    Timer driver implementación.
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * HEADERS
 ******************************************************************************/

#include "timer.h"
#include "SysTick.h"


/*******************************************************************************
 * CONSTANTES Y MACROS USANDO DEFINE
 ******************************************************************************/

#define NULL 0

#define MS_TO_TICKS(ms) ((ms * SYSTICK_ISR_FREQUENCY_HZ)/1000UL )
#define MS_TO_TICKS_MAX(ms) ((ms / 1000UL) * SYSTICK_ISR_FREQUENCY_HZ)
#define MS_LIMIT 1000000


/*******************************************************************************
 * STRUCTURAS Y TYPEDEFS
 ******************************************************************************/

typedef struct {
	ttick_t             period;    // periodo con el que se llama al callback (medido en ticks de systick)
	ttick_t             cnt;       // tiempo transcurrido desde ultimo llamado
    tim_callback_t      callback;  // funcion callback asignada
    uint8_t             state        : 1; //enable o disable
    uint8_t             running     : 1;
    uint8_t             expired     : 1;
    uint8_t             unused      : 5;
} timer_t;



/*******************************************************************************
 * PROTOTIPOS FUNCIONES LOCALES
 ******************************************************************************/


static void timer_isr(void);


/*******************************************************************************
 * VARIABLES ESTÁTICAS
 ******************************************************************************/

static timer_t timers[TIMERS_CANT]; // Arreglo global con timers que se inicializaron


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void timerInit(void)
{
    static bool yaInit = false;
    if (yaInit)
        return;

    systickInit(timer_isr); // init peripheral

    yaInit = true;
}


tim_id_t timerGetId(void)
{
    if (timers_cant >= TIMERS_MAX_CANT)
    {
        return TIMER_INVALID_ID;
    }
    else
    {
        return timers_cant++;
    }
}


void timerStart(tim_id_t id, ttick_t ticks, uint8_t mode, tim_callback_t callback)
{
    if ((id < timers_cant) && (mode < CANT_TIM_MODES))
    {
        // disable timer
        timers[id].running = 0;

        // configure timer
        timers[id].period = ticks;
        timers[id].cnt = ticks;
        timers[id].callback = callback;
        timers[id].mode = mode;
        timers[id].expired = 0;

        // enable timer
        timers[id].running = 1;
    }
}

void timerResume(tim_id_t id)
{
    if (id < timers_cant)
    {
        // Si esta pausado el timer
        if (!timers[id].running)
        {
            // Reanudo el timer
            timers[id].cnt = timers[id].period;
            timers[id].running = 1;
        }
    }
}

void timerPause(tim_id_t id)
{
    if (id < timers_cant)
    {
        // Apago el timer
        timers[id].running = 0;

        // y bajo el flag
        timers[id].expired = 0;
    }
}

void timerRestart(tim_id_t id)
{
    if (id < timers_cant)
    {
        // disable timer
        timers[id].running = 0;

        // configure timer
        timers[id].cnt = timers[id].period;
        timers[id].expired = 0;

        // enable timer
        timers[id].running = 1;
    }
}

bool timerRunning(tim_id_t id)
{
    return timers[id].running;
}

bool timerExpired(tim_id_t id)
{
    // Verifico si expiró el timer
    bool hasExpired = timers[id].expired;

    // y bajo el flag
    timers[id].expired = 0;

    return hasExpired;
}

void timerDelay(ttick_t ticks)
{
    timerStart(TIMER_ID_INTERNAL, ticks, TIM_MODE_SINGLESHOT, NULL);
    while (!timerExpired(TIMER_ID_INTERNAL));
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void timer_isr(void)
{
    // decremento los timers activos
    uint8_t timerIndex;
    for (timerIndex = 0 ; timerIndex < timers_cant ; timerIndex++)
    {
        timer_t* currentTimer = &timers[timerIndex];
        if (currentTimer->running)
        {
            currentTimer->cnt--;
            // si hubo timeout!
            if (currentTimer->cnt == 0)
            {
            	// Important: first update state so that if timerStart()
				// is called in the callback, this block doesn't deletes
            	// the configuration
                // 1) update state
                if (currentTimer->mode == TIM_MODE_SINGLESHOT)
                {
                    currentTimer->expired = 1;
                    currentTimer->running = 0;
                }
                else
                {
                    currentTimer->cnt = currentTimer->period;
                    currentTimer->expired = 1;
                }

            	// 2) execute action: callback or set flag
                if (currentTimer->callback)
                {
                    currentTimer->callback();
                }

            }
        }
    }
}

/******************************************************************************/
