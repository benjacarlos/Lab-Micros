/*
 * prueba.c
 *
 *      Author: Agus
 */

#include "SDK/CMSIS/MK64F12.h"
#include "SDK/CMSIS/MK64F12_features.h"
#include "gpio.h"
#include "hardware.h"


#ifndef GPIO_MUX
#define GPIO_MUX     001
#endif

#ifndef PORTS_N
#define PORTS_N 5
#define PINS_N 32
#endif

_Bool validPin(pin_t pin);

void setPCRmux(PORT_Type * p2port, uint8_t numPin, uint8_t mux);

void setPCRpullEnable(PORT_Type * portPointer, uint8_t pinNum);

void setPCRpullUp(PORT_Type * portPointer, uint8_t numPin);

void setPCRpullDown(PORT_Type * portPointer, uint8_t numPin);

void setGPIOddr(GPIO_Type * p2port, uint8_t numPin, uint32_t mode);

void setPCRirqc(PORT_Type * p2port, uint8_t numPin, uint8_t irqMode);

void setGPIOdataOut(GPIO_Type * gpioPortPointer, uint8_t numPin, _Bool value);

gpio_funs irq_data;

pinIrqFun_t irqFuns[PORTS_N][PINS_N];

void gpioMode (pin_t pin, uint8_t mode)
{
	uint8_t port = PIN2PORT(pin);
	uint8_t pinNum = PIN2NUM(pin);

	PORT_Type * portPointer[] = PORT_BASE_PTRS;
	GPIO_Type * gpioPortPointer[] = GPIO_BASE_PTRS;

	if (validPin(pin))
	{
			setPCRmux(portPointer[port], pinNum, GPIO_MUX);

			if(mode == INPUT_PULLUP || mode == INPUT_PULLDOWN)
			{
				setPCRpullEnable(portPointer[port], pinNum);
				if (mode == INPUT_PULLUP)
				{
					setPCRpullUp(portPointer[port], pinNum);
				}
				else
				{
					setPCRpullDown(portPointer[port], pinNum);
				}
				mode = INPUT;

			}
			setGPIOddr(gpioPortPointer[port], pinNum, (uint32_t) mode);
		}
}


void gpioToggle (pin_t pin)
{
	uint8_t port = PIN2PORT(pin);
	uint8_t pinNum = PIN2NUM(pin);

	GPIO_Type * gpioPortPointer[] = GPIO_BASE_PTRS;


	uint32_t writemask = ((uint32_t)(1<<pinNum));
	uint32_t deletemask = ~writemask;

	gpioPortPointer[port]->PTOR = gpioPortPointer[port]->PTOR & deletemask;
	gpioPortPointer[port]->PTOR = gpioPortPointer[port]->PTOR | writemask;
}


_Bool gpioRead (pin_t pin)
{
	uint8_t port = PIN2PORT(pin);
	uint8_t pinNum = PIN2NUM(pin);

	GPIO_Type * gpioPortPointer[] = GPIO_BASE_PTRS;

	uint32_t readmask = (uint32_t)(1 << pinNum);
	return ((gpioPortPointer[port]->PDIR) & readmask);
}


void gpioWrite (pin_t pin, _Bool value)
{
	uint8_t port = PIN2PORT(pin);
	uint8_t numPin = PIN2NUM(pin);
	GPIO_Type * gpioPortPointer[] = GPIO_BASE_PTRS;
	uint32_t maskPin = (uint32_t)(1 << numPin);


	if((gpioPortPointer[port]->PDDR) & maskPin)
	{
		setGPIOdataOut(gpioPortPointer[port], numPin, value);
	}
}


void setGPIOdataOut(GPIO_Type * gpioPortPointer, uint8_t numPin, _Bool value)
{
	uint32_t maskDataOut = (uint32_t)(value << numPin);
	uint32_t mask2delete = ~((uint32_t)(1 << numPin));
	gpioPortPointer->PDOR = (gpioPortPointer->PDOR & mask2delete);
	gpioPortPointer->PDOR = (gpioPortPointer->PDOR | maskDataOut);
}

_Bool validPin(pin_t pin)
{
	_Bool valid = false;
	if((pin >= PORTNUM2PIN(PA,0)) && (pin <= PORTNUM2PIN(PE,31)))
	{
		valid = true;
	}

	return valid;
}

void setPCRmux(PORT_Type * p2port, uint8_t numPin, uint8_t mux)
{
	uint32_t currentPCR = (p2port->PCR)[numPin];
	currentPCR = (currentPCR) & (~PORT_PCR_MUX_MASK);
	currentPCR = currentPCR | PORT_PCR_MUX(mux);
	(p2port->PCR)[numPin] = currentPCR;

}

void setPassiveFilter(pin_t pin)
{
	uint8_t port = PIN2PORT(pin);
	uint8_t numPin = PIN2NUM(pin);

	PORT_Type * portPointer[] = PORT_BASE_PTRS;
	uint32_t maskPFE = (HIGH << PORT_PCR_PFE_SHIFT);
	(portPointer[port]->PCR)[numPin] = ((portPointer[port]->PCR)[numPin] | maskPFE);
}


void setPCRpullEnable(PORT_Type * portPointer, uint8_t pinNum)
{
	uint32_t maskPE = (HIGH << PORT_PCR_PE_SHIFT);
	(portPointer->PCR)[pinNum] = ((portPointer->PCR)[pinNum] | maskPE);
}


void setPCRpullUp(PORT_Type * portPointer, uint8_t numPin)
{
	uint32_t maskPE = (HIGH << PORT_PCR_PS_SHIFT);
	(portPointer->PCR)[numPin] = ((portPointer->PCR)[numPin] | maskPE);
}

void setPCRpullDown(PORT_Type * portPointer, uint8_t numPin)
{
	uint32_t maskPE = (HIGH << PORT_PCR_PE_SHIFT);
	(portPointer->PCR)[numPin] = ((portPointer->PCR)[numPin] & (~maskPE));
}

void setGPIOddr(GPIO_Type * p2port, uint8_t pinNum, uint32_t mode)
{
	uint32_t maskDDR = (mode << pinNum);
	p2port->PDDR = ((p2port->PDDR) | maskDDR);
}

bool gpioIRQ (pin_t pin, uint8_t irqMode, pinIrqFun_t irqFun)
{
	_Bool out = false;

	uint8_t port;
	uint8_t pinNum;

	IRQn_Type p2portsIRQ[] = PORT_IRQS;
	PORT_Type * portPointer[] = PORT_BASE_PTRS;

	if(validPin(pin))
	{

		port = PIN2PORT(pin);
		pinNum = PIN2NUM(pin);

		if (irqMode != GPIO_IRQ_CANT_MODES)
		{
			setPCRirqc(portPointer[port], pinNum, irqMode);
			irqFuns[port][pinNum] = irqFun;

			out = true;
		}

		NVIC_EnableIRQ(p2portsIRQ[port]);
	}

	return out;

}

void setPCRirqc(PORT_Type * p2port, uint8_t pinNum, uint8_t irqMode)
{
	uint32_t currentPCR = (p2port->PCR)[pinNum];
	currentPCR = currentPCR & (~PORT_PCR_IRQC_MASK);
	currentPCR = currentPCR | PORT_PCR_IRQC(irqMode);
	(p2port->PCR)[pinNum] = currentPCR;
}


void IRQHandler(uint8_t port)
{
	_Bool foundirq = false;
	int i;
	uint32_t currentPCR;
	PORT_Type * portPointer[] = PORT_BASE_PTRS;

	for(i = 0; (i < PINS_N) && (!foundirq); i++)
	{
		if( ((portPointer[port])->PCR[i]) & PORT_PCR_ISF_MASK )
		{
			foundirq = true;
			irqFuns[port][i]();

			currentPCR = portPointer[port]->PCR[i];
			currentPCR = currentPCR & (~PORT_PCR_ISF_MASK);
			portPointer[port]->PCR[i] = currentPCR | PORT_PCR_ISF(HIGH);
		}
	}
}


__ISR__ PORTA_IRQHandler(void)
{
	IRQHandler(PA);
}

__ISR__ PORTB_IRQHandler(void)
{
	IRQHandler(PB);
}

__ISR__ PORTC_IRQHandler(void)
{
	IRQHandler(PC);
}

__ISR__ PORTD_IRQHandler(void)
{
	IRQHandler(PD);
}

__ISR__ PORTE_IRQHandler(void)
{
	IRQHandler(PE);
}
