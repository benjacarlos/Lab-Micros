
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "uart.h"
#include "SDK/CMSIS/MK64F12.h"
#include "SDK/CMSIS/MK64F12_features.h"


/*******************************************************************************
 * PROTOTIPOS LOCALES
 ******************************************************************************/
void clockGating(uint8_t id);
void setbBaudrate(uint8_t id, uint32_t baudrate);



/*******************************************************************************
 * DEFINICONES FUNCIONES
 ******************************************************************************/

void uartInit (uint8_t id, uart_cfg_t config)
{
	//ID: 0, 1, 2, 3, 4, 5

	UART_Type * uartPointers[] = UART_BASE_PTRS;
	clockgating(id);

	UART_C2_RE(uartPointers[id]) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
	UART_C1_RE(uartPointers[id]) = 0;
	setBaudrate(id, config.baudrate);
	UART_C2_REG(uartPointers[id]) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}

uint8_t uartWriteMsg(uint8_t id, const char* msg, uint8_t cant) //hacer no bloqueante y evr que hacer con cant
{
	UART_Type * uartPointers[] = UART_BASE_PTRS;
	//blocking

	 while(((uartPointers[id]->S1) & UART_S1_TDRE_MASK) == 0); //Puedo Transmitir ?
	 uartPointers[id]->D = tx_data; // Transmito

}

uint8_t uartReadMsg(uint8_t id, char* msg, uint8_t cant)//hacer no bloqueante y ver que hacer con cant y el return
{
	UART_Type * uartPointers[] = UART_BASE_PTRS;

	while(((uartPointers[id]->S1)& UART_S1_RDRF_MASK) ==0); // Espero recibir un caracter
	msg = &uartPointers[id]->D;


	return(uartPointers[id]->D); //devuelvo???
}

void clockGating(uint8_t id)
{
	SIM_Type * sim = SIM;
	switch(id)
	{
		case 0:
			sim->SCGC4 |= SIM_SCGC4_UART0_MASK;
			break;
		case 1:
			sim->SCGC4 |= SIM_SCGC4_UART1_MASK;
			break;
		case 2:
			sim->SCGC4 |= SIM_SCGC4_UART2_MASK;
			break;
		case 3:
			sim->SCGC4 |=  SIM_SCGC4_UART3_MASK;
			break;
		case 4:
			sim->SCGC1 |= SIM_SCGC1_UART4_MASK;
			break;
		case 5:
			sim->SCGC1 |= SIM_SCGC1_UART5_MASK;
			break;
	}
}


void setbBaudrate(uint8_t id, uint32_t baudrate)
{
	UART_Type * uartPointers[] = UART_BASE_PTRS;

	uint16_t sbr, brfa;
	uint32_t clock;

	clock = ((uartPointers[id] == UART0) || (uartPoingters[id] == UART1)) ? (__CORE_CLOCK__) : (__CORE_CLOCK__ >> 1);

	baudrate = ((baudrate == 0) ? (UART_HAL_DEFAULT_BAUDRATE) : ((baudrate > 0x1FFF) ? (UART_HAL_DEFAULT_BAUDRATE) : (baudrate)));

	sbr = clock / (baudrate << 4);
	brfa = (clock << 1) / baudrate - (sbr << 5);

	uartPointers[id]->BDH = UART_BDH_SBR(sbr >> 8);
	uartPointers[id]->BDL = UART_BDL_SBR(sbr);
	uartPointers[id]->C4 = (uartPointers[id]->C4 & ~UART_C4_BRFA_MASK) | UART_C4_BRFA(brfa);
}
