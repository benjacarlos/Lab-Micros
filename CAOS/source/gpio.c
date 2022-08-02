/*
 * prueba.c
 *
 *  Created on: Sep 3, 2020
 *      Author: Agus
 */

#include "SDK/CMSIS/MK64F12.h"
#include "SDK/CMSIS/MK64F12_features.h"
#include "gpio.h"
#include "hardware.h"

//variables globaels que sirven para el funcionamiento de las funciones de interrupcion

//pinIrqFun_t irqfun_p_array[86];
//pin_t current_pin;

gpio_funs irq_data;

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

void gpioPull(pin_t pin, bool mode)//arreglar modo
{
	switch(PIN2PORT(pin))
		{
			case PA:
				PORTA->PCR[PIN2NUM(pin)] = PORT_PCR_PE(1);
				PORTA->PCR[PIN2NUM(pin)] = PORT_PCR_PS(mode);
				break;

			case PB:
				PORTB->PCR[PIN2NUM(pin)] = PORT_PCR_PE(1);
				PORTB->PCR[PIN2NUM(pin)] = PORT_PCR_PS(mode);
				break;

			case PC:
				PORTC->PCR[PIN2NUM(pin)] = PORT_PCR_PE(1);
				PORTC->PCR[PIN2NUM(pin)] = PORT_PCR_PS(mode);
				break;

			case PD:
				PORTD->PCR[PIN2NUM(pin)] = PORT_PCR_PE(1);
				PORTD->PCR[PIN2NUM(pin)] = PORT_PCR_PS(mode);
				break;

			case PE:
				PORTE->PCR[PIN2NUM(pin)] = PORT_PCR_PE(1);
				PORTE->PCR[PIN2NUM(pin)] = PORT_PCR_PS(mode);
				break;
		}
}


bool gpioIRQ (pin_t pin, uint8_t irqMode, pinIrqFun_t irqFun)
{
//	current_pin = pin;


	switch(PIN2PORT(pin))
		{
			case PA:
				irq_data.irq_pins[PA] = pin;
				NVIC_EnableIRQ(PORTA_IRQn);
				PORTA->PCR[PIN2NUM(pin)] |= PORT_PCR_ISF_MASK;
				PORTA->PCR[PIN2NUM(pin)] |= PORT_PCR_IRQC(irqMode);
//				irqfun_p_array[PORTA_IRQn] = irqFun;
				irq_data.irqfun_p_array[PA] = irqFun;
				break;

			case PB:
				irq_data.irq_pins[PB] = pin;
				NVIC_EnableIRQ(PORTB_IRQn);
				PORTB->PCR[PIN2NUM(pin)] |= PORT_PCR_ISF_MASK;
				PORTB->PCR[PIN2NUM(pin)] |= PORT_PCR_IRQC(irqMode);
//				irqfun_p_array[PORTB_IRQn] = irqFun;
				irq_data.irqfun_p_array[PB] = irqFun;
				break;

			case PC:
				irq_data.irq_pins[PC] = pin;
				NVIC_EnableIRQ(PORTC_IRQn);
				PORTC->PCR[PIN2NUM(pin)] |= PORT_PCR_ISF_MASK;
				PORTC->PCR[PIN2NUM(pin)] |= PORT_PCR_IRQC(irqMode);
//				irqfun_p_array[PORTB_IRQn] = irqFun;
				irq_data.irqfun_p_array[PC] = irqFun;
				break;

			case PD:
				irq_data.irq_pins[PD] = pin;
				NVIC_EnableIRQ(PORTD_IRQn);
				PORTD->PCR[PIN2NUM(pin)] |= PORT_PCR_ISF_MASK;
				PORTD->PCR[PIN2NUM(pin)] |= PORT_PCR_IRQC(irqMode);
//				irqfun_p_array[PORTD_IRQn] = irqFun;
				irq_data.irqfun_p_array[PD] = irqFun;
				break;

			case PE:
				irq_data.irq_pins[PE] = pin;
				NVIC_EnableIRQ(PORTE_IRQn);
				PORTE->PCR[PIN2NUM(pin)] |= PORT_PCR_ISF_MASK;
				PORTE->PCR[PIN2NUM(pin)] |= PORT_PCR_IRQC(irqMode);
//				irqfun_p_array[PORTE_IRQn] = irqFun;
				irq_data.irqfun_p_array[PE] = irqFun;
				break;
		}
}


__ISR__ PORTA_IRQHandler(void)
{
	PORTA->PCR[PIN2NUM(irq_data.irq_pins[PA])] |= PORT_PCR_ISF_MASK;
//	(*irqfun_p_array[PORTA_IRQn])();
	(*irq_data.irqfun_p_array[PA])();
}

__ISR__ PORTB_IRQHandler(void)
{
	PORTB->PCR[PIN2NUM(irq_data.irq_pins[PB])] |= PORT_PCR_ISF_MASK;
//	(*irqfun_p_array[PORTB_IRQn])();
	(*irq_data.irqfun_p_array[PB])();
}

__ISR__ PORTC_IRQHandler(void)
{
	PORTC->PCR[PIN2NUM(irq_data.irq_pins[PC])] |= PORT_PCR_ISF_MASK;
//	(*irqfun_p_array[PORTC_IRQn])();
	(*irq_data.irqfun_p_array[PC])();
}

__ISR__ PORTD_IRQHandler(void)
{
	PORTD->PCR[PIN2NUM(irq_data.irq_pins[PD])] |= PORT_PCR_ISF_MASK;
//	(*irqfun_p_array[PORTD_IRQn])();
	(*irq_data.irqfun_p_array[PD])();
}

__ISR__ PORTE_IRQHandler(void)
{
	PORTE->PCR[PIN2NUM(irq_data.irq_pins[PD])] |= PORT_PCR_ISF_MASK;
//	(*irqfun_p_array[PORTE_IRQn])();
	(*irq_data.irqfun_p_array[PE])();
}
