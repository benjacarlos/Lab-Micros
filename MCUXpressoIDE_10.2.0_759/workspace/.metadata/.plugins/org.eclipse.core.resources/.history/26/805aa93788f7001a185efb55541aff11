/*
 * prueba.c
 *
 *  Created on: Sep 3, 2020
 *      Author: Agus
 */

#include "SDK/CMSIS/MK64F12.h"
#include "SDK/CMSIS/MK64F12_features.h"
#include "gpio.h"


void gpioMode (pin_t pin, uint8_t mode)
{
    if (PIN2PORT(pin) == PA)
    {
        SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
        PORTA->PCR[PIN2NUM(pin)] = 0x0;
        PORTA->PCR[PIN2NUM(pin)] |= PORT_PCR_MUX(PORT_mGpio);
        if (mode == OUTPUT)
        {
            GPIOA->PDDR = (OUTPUT<<PIN2NUM(pin));
        }
        else if (mode == INPUT)
        {
            GPIOA->PDDR = (INPUT<<PIN2NUM(pin));
        }
    }

    else if (PIN2PORT(pin) == PB)
    {
        SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
        PORTB->PCR[PIN2NUM(pin)] = 0x0;
        PORTB->PCR[PIN2NUM(pin)] |= PORT_PCR_MUX(PORT_mGpio);
        if (mode == OUTPUT)
        {
            GPIOB->PDDR = (OUTPUT<<PIN2NUM(pin));
        }
        else if (mode == INPUT)
        {
            GPIOB->PDDR = (INPUT<<PIN2NUM(pin));
        }
    }

    else if (PIN2PORT(pin) == PC)
    {
        SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
        PORTC->PCR[PIN2NUM(pin)] = 0x0;
        PORTC->PCR[PIN2NUM(pin)] |= PORT_PCR_MUX(PORT_mGpio);
        if (mode == OUTPUT)
        {
            GPIOC->PDDR = (OUTPUT<<PIN2NUM(pin));
        }
        else if (mode == INPUT)
        {
            GPIOC->PDDR = (INPUT<<PIN2NUM(pin));
        }
    }

    else if (PIN2PORT(pin) == PD)
    {
        SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
        PORTD->PCR[PIN2NUM(pin)] = 0x0;
        PORTD->PCR[PIN2NUM(pin)] |= PORT_PCR_MUX(PORT_mGpio);
        if (mode == OUTPUT)
        {
            GPIOD->PDDR = (OUTPUT<<PIN2NUM(pin));
        }
        else if (mode == INPUT)
        {
            GPIOD->PDDR = (INPUT<<PIN2NUM(pin));
        }
    }

    else if (PIN2PORT(pin) == PE)
    {
        SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
        PORTE->PCR[PIN2NUM(pin)] = 0x0;
        PORTE->PCR[PIN2NUM(pin)] |= PORT_PCR_MUX(PORT_mGpio);
        if (mode == OUTPUT)
        {
            GPIOE->PDDR = (OUTPUT<<PIN2NUM(pin));
        }
        else if (mode == INPUT)
        {
            GPIOE->PDDR = (INPUT<<PIN2NUM(pin));
        }
    }

}


void gpioToggle (pin_t pin)
{
	if(PIN2PORT(pin) == PA)
	{
		GPIOA->PTOR = 1<<PIN2NUM(pin);
	}

	else if(PIN2PORT(pin) == PB)
	{
		GPIOB->PTOR = 1<<PIN2NUM(pin);
	}

	else if(PIN2PORT(pin) == PC)
	{
		GPIOC->PTOR = 1<<PIN2NUM(pin);
	}

	else if(PIN2PORT(pin) == PD)
	{
		GPIOD->PTOR = 1<<PIN2NUM(pin);
	}

	else if(PIN2PORT(pin) == PE)
	{
		GPIOE->PTOR = 1<<PIN2NUM(pin);
	}
}


bool gpioRead (pin_t pin)
{
	switch(PIN2PORT(pin))
	{
		case PA:
			return ((GPIOA->PDIR >> PIN2NUM(pin)) & 0x01);
			break;

		case PB:
			return ((GPIOB->PDIR >> PIN2NUM(pin)) & 0x01);
			break;

		case PC:
			return ((GPIOC->PDIR >> PIN2NUM(pin)) & 0x01);
			break;

		case PD:
			return ((GPIOD->PDIR >> PIN2NUM(pin)) & 0x01);
			break;

		case PE:
			return ((GPIOE->PDIR >> PIN2NUM(pin)) & 0x01);
			break;
	}
}


bool gpioIRQ (pin_t pin, uint8_t irqMode, pinIrqFun_t irqFun)
{
	switch(PIN2PORT(pin))
		{
			case PA:
				NVIC_EnableIRQ(PORTA_IRQn);
				PORTA->PCR[PIN2NUM(pin)] = PORT_PCR_IRQC(irqMode);
				break;

			case PB:
				NVIC_EnableIRQ(PORTB_IRQn);
				PORTB->PCR[PIN2NUM(pin)] = PORT_PCR_IRQC(irqMode);
				break;

			case PC:
				NVIC_EnableIRQ(PORTC_IRQn);
				PORTC->PCR[PIN2NUM(pin)] = PORT_PCR_IRQC(irqMode);
				break;

			case PD:
				NVIC_EnableIRQ(PORTD_IRQn);
				PORTD->PCR[PIN2NUM(pin)] = PORT_PCR_IRQC(irqMode);
				break;

			case PE:
				NVIC_EnableIRQ(PORTE_IRQn);
				PORTE->PCR[PIN2NUM(pin)] = PORT_PCR_IRQC(irqMode);
				break;
		}




}

//__ISR__ PORTA_IRQHandler(void)
//{
//
//	PORTA->PCR[4]
//
//}


