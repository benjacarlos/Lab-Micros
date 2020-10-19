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
                      DEFINICION FUNCIONES DEL HEADER
 *******************************************************************************
 ******************************************************************************/

void timerInit(void)
{
    static bool yaInit = false; //variable estática que indica si se inicializó el módulo de timers
    if (yaInit)
        return;

    for (int i = 0; i < TIMERS_CANT; i++)
    {
    	(timers + i)->period = UINT32_MAX ; //aca deberia poner un valor por defecto maximo
    	(timers + i)->cnt = 0;
    	(timers + i)->callback  = NULL;
    	(timers + i)->state = false;
    }

    SysTick_Init(&timer_isr); // init peripheral

    yaInit = true;
}


void timerStart(tim_id_t id, ttick_t ticks, tim_callback_t callback)
{
	timerDisable(id); //se desactiva el timer para realizar cambios


	if(ticks > MS_LIMIT)
	{
		(timers + id)->period = MS_TO_TICKS_MAX(ticks);
	}
	else
	{
		(timers + id)->period = MS_TO_TICKS(ticks);
	}

	(timers + id)->cnt = 0;
	(timers + id)->callback = callback;
	timerEnable(id);

}

ttick_t timerGetCnt(tim_id_t id)
{
	return (timers + id)->cnt;
}

void timerRestart(tim_id_t id)
{
	(timers + id)->cnt = 0;
}

void timerEnable(tim_id_t id)
{
	(timers + id)->state = true;
}

void timerDisable(tim_id_t id)
{
	(timers + id)->state = false;
}


/*******************************************************************************
 *******************************************************************************
                        DEFINICION FUNCIONES LOCALES
 *******************************************************************************
 ******************************************************************************/

static void timer_isr(void)
{

	for (int i = 0; i < TIMERS_CANT; i++)
	{
		if( (timers + i)->state)
		{
			if( ++((timers + i)->cnt) >=  ((timers + i)->period)) //incremento el contador y veo si hubo timeout
			{
				(timers + i)->cnt = 0;
				((timers + i)->callback)();  //se llama el callback
			}

		}
	}
}


/******************************************************************************/
