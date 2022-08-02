/***************************************************************************//**
  @file     gpio.h
  @brief    Simple GPIO Pin services, similar to Arduino
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Ports
enum { PA, PB, PC, PD, PE };

// Convert port and number into pin ID
// Ex: PTB5  -> PORTNUM2PIN(PB,5)  -> 0x25
//     PTC22 -> PORTNUM2PIN(PC,22) -> 0x56
#define PORTNUM2PIN(p,n)    (((p)<<5) + (n))
#define PIN2PORT(p)         (((p)>>5) & 0x07)
#define PIN2NUM(p)          ((p) & 0x1F)


// Modes
#ifndef INPUT
#define INPUT               0
#define OUTPUT              1
#define INPUT_PULLUP        2
#define INPUT_PULLDOWN      3
#endif // INPUT


// Digital values
#ifndef LOW
#define LOW     0
#define HIGH    1
#endif // LOW


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t pin_t;

typedef void (*pinIrqFun_t)(void);

typedef enum
{
	PORT_mAnalog,
	PORT_mGpio,
	PORT_mAlt2,
	PORT_mAlt3,
	PORT_mAlt4,
	PORT_mAlt5,
	PORT_mAlt6,
	PORT_mAlt7,

}PORTMux_t;

typedef enum
{
	PORT_eDisabled             = 0x00,
	PORT_eDMARising            = 0x01,
	PORT_eDMAFalling           = 0x02,
	PORT_eDMAEither            = 0x03,
	PORT_eInterruptDisasserted = 0x08,
	PORT_eInterruptRising      = 0x09,
	PORT_eInterruptFalling     = 0x0A,
	PORT_eInterruptEither      = 0x0B,
	PORT_eInterruptAsserted    = 0x0C,
	GPIO_IRQ_CANT_MODES        = 0x0D,

} PORTEvent_t;



typedef struct
{
	pinIrqFun_t irqfun_p_array[5];
	pin_t irq_pins[5];

} gpio_funs;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

//extern pinIrqFun_t irqfun_p_array[5];//arreglo con punteros a funcion para callbacks de interrupciones, la idea es tener un lugar en este arreglo poara cada puerto
						          //y utilizar los defines dados en mf64f PORTX_IRqn para los indices.



//extern pin_t current_pin; //esta variable contiene el ultimo pin que se quiere interrumpir, se llamó anteriormente a la funcion gpioIRQ para configurarlo.

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Configures the specified pin to behave either as an input or an output
 * @param pin the pin whose mode you wish to set (according PORTNUM2PIN)
 * @param mode INPUT, OUTPUT, INPUT_PULLUP or INPUT_PULLDOWN.
 */
void gpioMode (pin_t pin, uint8_t mode);

/**
 * @brief Configures how the pin reacts when an IRQ event ocurrs
 * @param pin the pin whose IRQ mode you wish to set (according PORTNUM2PIN)
 * @param irqMode disable, risingEdge, fallingEdge or bothEdges
 * @param irqFun function to call on pin event
 * @return Registration succeed
 */
bool gpioIRQ (pin_t pin, uint8_t irqMode, pinIrqFun_t irqFun);

/**
 * @brief Write a HIGH or a LOW value to a digital pin
 * @param pin the pin to write (according PORTNUM2PIN)
 * @param val Desired value (HIGH or LOW)
 */
void gpioWrite (pin_t pin, bool value);

/**
 * @brief Toggle the value of a digital pin (HIGH<->LOW)
 * @param pin the pin to toggle (according PORTNUM2PIN)
 */
void gpioToggle (pin_t pin);

/**
 * @brief Reads the value from a specified digital pin, either HIGH or LOW.
 * @param pin the pin to read (according PORTNUM2PIN)
 * @return HIGH or LOW
 */
bool gpioRead (pin_t pin);


void setPassiveFilter(pin_t pin);

void gpioWrite (pin_t pin, _Bool value);



/*******************************************************************************
 ******************************************************************************/

#endif // _GPIO_H_
