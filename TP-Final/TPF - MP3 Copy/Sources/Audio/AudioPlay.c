/***************************************************************************//**
 @file     AudioPlay.h
 @brief    Audio Reproductor
 @author   Grupo 5
******************************************************************************/

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "AudioPlay.h"
#include "fsl_dac.h"
#include "fsl_edma.h"
#include "fsl_pdb.h"
#include "fsl_dmamux.h"
#include "fsl_common.h"


/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define PDB_BASEADDR           PDB0
#define PDB_MODULUS_VALUE      0xFFFU
#define PDB_DELAY_VALUE        0U
#define PDB_DAC_CHANNEL        kPDB_DACTriggerChannel0
#define PDB_DAC_INTERVAL_VALUE 0xFFFU
#define DAC_BASEADDR           DAC0
#define DMAMUX_BASEADDR        DMAMUX
#define DMA_CHANNEL            1U
#define DMA_DAC_SOURCE         45U
#define DMA_BASEADDR           DMA0
#define DAC_DATA_REG_ADDR      0x400cc000U
#define DMA_IRQ_ID             DMA0_IRQn

#define FREC_BASE              60000000 //60MHz

/*******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
/*!
 * @brief Initialize the EDMA.
 */
static void EDMA_Configuration(void);
/*!
 * @brief Initialize the DMAMUX.
 */
static void DMAMUX_Configuration(void);
/*!
 * @brief Initialize the PDB.
 */
static void PDB_Configuration(void);
/*!
 * @brief Initialize the DAC.
 */
static void DAC_Configuration(void);
/*!
 * @brief Callback function for EDMA.
 */
static void EDMA_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static edma_handle_t g_EDMA_Handle;                             /* Edma handler */
static edma_transfer_config_t g_transferConfig;                 /* Edma transfer config. */
static volatile uint32_t g_index = 0U; 							/* Index of the g_dacDataArray array. */


/*
uint16_t buffers[2][AUDIO_PLAYER_BUFF_SIZE];
uint16_t * activeBuffer = buffers[0];
uint16_t * backBuffer= buffers[1];
*/
static int16_t buffers[2][AUDIO_PLAYER_BUFF_SIZE];
static int16_t * activeBuffer = buffers[0];
static int16_t * backBuffer= buffers[1];
static bool backBufferFree = false;
static bool pause = false, stop = false;
// Inicializamos el DAC con un valor máximo que no puede representar
static int16_t mute[DAC_DATL_COUNT] = {2048U};


/*******************************************************************************
*******************************************************************************
                       GLOBAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/

void AudioPlay_Init(void)
{
	// Initialize buffer for DAC
	for(uint8_t i = 0; i < DAC_DATL_COUNT; i++)
	{
		mute[i] = 0;
	}

	/* Initialize the MUX of the DMA. */
	DMAMUX_Configuration();
	/* Initialize EDMA for transfer a chunk of memory. */
	EDMA_Configuration();
	/* Initialize the HW trigger source from when to transfer the buffer to the DAC. */
	// Lo de PDB se hace directamente en update sample rate porque esta relacionado.
	PDB_Configuration();
	/* Initialize DAC. */
	DAC_Configuration();
}

void AudioPlay_LoadSongInfo(uint16_t * firstSongFrame, uint16_t sampleRate)
{
	memcpy(activeBuffer, firstSongFrame, AUDIO_PLAYER_BUFF_SIZE * sizeof(uint16_t));
	//activeBuffer = firstSongFrame;
	//backBuffer = secondSongFrame;
	backBufferFree = true;
	g_index = 0U;

	AudioPlayer_UpdateSampleRate(sampleRate);
}


// Y para eso hay que chequear que a lo que usa PDB para triggerear no le afecte esto....
void AudioPlay_UpdateSampleRate(uint16_t sampleRate) //PDB_Configuration
{
    pdb_config_t pdbConfigStruct;
    pdb_dac_trigger_config_t pdbDacTriggerConfigStruct;
    PDB_GetDefaultConfig(&pdbConfigStruct);

    //para 60MHz
    pdbConfigStruct.dividerMultiplicationFactor = kPDB_DividerMultiplicationFactor1;
    pdbConfigStruct.prescalerDivider = kPDB_PrescalerDivider1;
    PDB_SetModulusValue(PDB_BASEADDR, (uint32_t) (FREC_BASE/sampleRate));
    PDB_SetDACTriggerIntervalValue(PDB_BASEADDR, PDB_DAC_CHANNEL, (uint32_t) (FREC_BASE/sampleRate));

	// Pongo estas configuraciones de PDB aca porque necesitan info del sample rate
	pdbConfigStruct.enableContinuousMode        = true;
	PDB_Init(PDB_BASEADDR, &pdbConfigStruct);
	PDB_EnableInterrupts(PDB_BASEADDR, kPDB_DelayInterruptEnable);
	PDB_SetCounterDelayValue(PDB_BASEADDR, PDB_DELAY_VALUE);

    /* Set DAC trigger. */
    pdbDacTriggerConfigStruct.enableExternalTriggerInput = false;
    pdbDacTriggerConfigStruct.enableIntervalTrigger      = true;
    PDB_SetDACTriggerConfig(PDB_BASEADDR, PDB_DAC_CHANNEL, &pdbDacTriggerConfigStruct);

    /* Load PDB values. */
    PDB_DoLoadValues(PDB_BASEADDR);
}

audioPlayerError AudioPlay_UpdateBackBuffer(uint16_t * newBackBuffer)
{
	if(backBufferFree)
	{
		memcpy( backBuffer, newBackBuffer, AUDIO_PLAYER_BUFF_SIZE * sizeof(uint16_t));
		backBufferFree = false;
		return AP_NO_ERROR;
	}
	else
		return AP_ERROR_BB_NOT_FREE;
}

bool AudioPlay_IsBackBufferFree(void)
{
	return backBufferFree;
}

void AudioPlay_Play(void)
{
	g_index = 0U;
	stop = false;
	if(pause)
	{
		pause = false;
		return;
	}
	// DMAMUX:
    DMAMUX_EnableChannel(DMAMUX_BASEADDR, DMA_CHANNEL);

	// EDMA:
	 EDMA_PrepareTransfer(&g_transferConfig, (void *)(activeBuffer + g_index), sizeof(uint16_t),
	                         (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), DAC_DATL_COUNT * sizeof(uint16_t),
	                         DAC_DATL_COUNT * sizeof(uint16_t), kEDMA_MemoryToMemory);
	 EDMA_SubmitTransfer(&g_EDMA_Handle, &g_transferConfig);
	 /* Enable interrupt when transfer is done. */
	 EDMA_EnableChannelInterrupts(DMA_BASEADDR, DMA_CHANNEL, kEDMA_MajorInterruptEnable);

	 /* Enable transfer. */
	EDMA_StartTransfer(&g_EDMA_Handle);

	// PDB:

	// DAC:
	/* Enable DMA. */
	DAC_EnableBufferInterrupts(DAC_BASEADDR, kDAC_BufferReadPointerTopInterruptEnable);
	DAC_EnableBufferDMA(DAC_BASEADDR, true);

	PDB_DoSoftwareTrigger(PDB_BASEADDR);
}

void AudioPlay_Pause(void)
{
	pause = true;
}

void AudioPlay_Stop(void)
{
	stop = true;
}


/*******************************************************************************
*******************************************************************************
                       LOCAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/

static void EDMA_Configuration(void)
{
    /*
     *
    edma_config_t userConfig;

    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(DMA_BASEADDR, &userConfig);
     *
     *
     */
	EDMA_CreateHandle(&g_EDMA_Handle, DMA_BASEADDR, DMA_CHANNEL);
    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
}

static void DMAMUX_Configuration(void)
{
    /* Configure DMAMUX */
    // Init en matrix // DMAMUX_Init(DMAMUX_BASEADDR);
    DMAMUX_SetSource(DMAMUX_BASEADDR, DMA_CHANNEL, DMA_DAC_SOURCE); /* Map ADC source to channel 0 */
}

/* Enable the trigger source of PDB. */
static void PDB_Configuration(void)
{
    pdb_config_t pdbConfigStruct;
    pdb_dac_trigger_config_t pdbDacTriggerConfigStruct;

    PDB_GetDefaultConfig(&pdbConfigStruct);
    // Paso esto a load song info para cambiar los valores para el PDB
    pdbConfigStruct.dividerMultiplicationFactor = kPDB_DividerMultiplicationFactor40;
    pdbConfigStruct.enableContinuousMode        = true;
    PDB_Init(PDB_BASEADDR, &pdbConfigStruct);
    PDB_EnableInterrupts(PDB_BASEADDR, kPDB_DelayInterruptEnable);
    PDB_SetModulusValue(PDB_BASEADDR, PDB_MODULUS_VALUE);
    PDB_SetCounterDelayValue(PDB_BASEADDR, PDB_DELAY_VALUE);


    /* Set DAC trigger. */
    pdbDacTriggerConfigStruct.enableExternalTriggerInput = false;
    pdbDacTriggerConfigStruct.enableIntervalTrigger      = true;
    PDB_SetDACTriggerConfig(PDB_BASEADDR, PDB_DAC_CHANNEL, &pdbDacTriggerConfigStruct);
    PDB_SetDACTriggerIntervalValue(PDB_BASEADDR, PDB_DAC_CHANNEL, PDB_DAC_INTERVAL_VALUE);

    /* Load PDB values. */
    PDB_DoLoadValues(PDB_BASEADDR);
}

static void DAC_Configuration(void)
{
    dac_config_t dacConfigStruct;
    dac_buffer_config_t dacBufferConfigStruct;

    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DAC_BASEADDR, true); /* Enable output. */

    /* Configure the DAC buffer. */
    DAC_EnableBuffer(DAC_BASEADDR, true);
    DAC_GetDefaultBufferConfig(&dacBufferConfigStruct);
    dacBufferConfigStruct.triggerMode = kDAC_BufferTriggerByHardwareMode;
    DAC_SetBufferConfig(DAC_BASEADDR, &dacBufferConfigStruct);
    DAC_SetBufferReadPointer(DAC_BASEADDR, 0U); /* Make sure the read pointer to the start. */
}

static void EDMA_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    /* Clear Edma interrupt flag. */
    EDMA_ClearChannelStatusFlags(DMA_BASEADDR, DMA_CHANNEL, kEDMA_InterruptFlag);
    /* Setup transfer */

    void * srcAdd = NULL;
    if(pause || stop)
	{
		srcAdd = mute;
	}
    else
    {
    	g_index += DAC_DATL_COUNT;
		if (g_index == AUDIO_PLAYER_BUFF_SIZE)
		{
			g_index = 0U;
			void * temp = activeBuffer;
			activeBuffer = backBuffer;
			backBuffer = temp;
			backBufferFree = true;
		}
		srcAdd = (activeBuffer + g_index);
    }

    EDMA_PrepareTransfer(&g_transferConfig, (void *)(srcAdd), sizeof(uint16_t),
                        (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), DAC_DATL_COUNT * sizeof(uint16_t),
                        DAC_DATL_COUNT * sizeof(uint16_t), kEDMA_MemoryToMemory);


    EDMA_SetTransferConfig(DMA_BASEADDR, DMA_CHANNEL, &g_transferConfig, NULL);
    /* Enable transfer. */
    EDMA_StartTransfer(&g_EDMA_Handle);
}

