/***************************************************************************/ /**
  @file     memory_manager.h
  @brief    Memory System header
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
//#include "file_system_manager.h"

#include <stdio.h>

#include "fsl_port.h"
#include "fsl_sd.h"
#include "fsl_sd_disk.h"

#include "ff.h"
#include "diskio.h"
#include "sdmmc_config.h"

#include "memory_manager.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool SD_connected = false;
static bool SD_error = false;
static bool SD_HostInitDone = false;
static FATFS g_fileSystem;
static uint8_t SD_status = 0;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void Mm_Callback(bool isInserted, void *userData);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void Mm_Init()
{
	SYSMPU->CESR &= ~SYSMPU_CESR_VLD_MASK;

	BOARD_SD_Config(&g_sd, Mm_Callback, BOARD_SDMMC_SD_HOST_IRQ_PRIORITY, NULL);

	if (SD_HostInit(&g_sd) != kStatus_Success)
	{
		printf("\r\nSD host init fail\r\n");
		SD_error = true;
	}

	SD_HostInitDone = true;

}


bool Mm_IsSDPresent(void)
{
	return SD_connected;
}

bool Mm_SDConnection(void)
{
	bool ret = SD_status == 1;
	if(ret)
		SD_status = 0;
	return ret;
}

bool Mm_SDDesconnection(void)
{
	bool ret = SD_status == 2;
	if(ret)
		SD_status = 0;
	return ret;
}
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void Mm_Callback(bool isInserted, void *userData)
{
	if(SD_connected == isInserted)
	{
		SD_error = true;
	}
	else if(isInserted)
	{
		SD_status = 1;
		//Mm_OnConnection();
	}
	else
	{
		SD_status = 2;
		//Mm_OnDesconection();
	}

	SD_connected = isInserted;
}

void Mm_OnDesconection(void)
{
	const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
	f_mount(NULL, driverNumberBuffer, 1U);
	g_sd.initReady = false;
	//firstFile = FileSystem_ResetFiles();
	//printf("fuera\r\n");
}

void Mm_OnConnection(void)
{
	FRESULT error;
	const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};

	if(!SD_HostInitDone)
	{
		SD_error = true;
		return;
	}

	if (f_mount(&g_fileSystem, driverNumberBuffer, 1U))
	{
		printf("Mount volume failed.\r\n");
		SD_error = true;
		return;
	}

    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
	if (error)
	{
		printf("Change drive failed.\r\n");
		SD_error = true;
		return;
	}

	//Mm_ScanFiles(buff);

	//FileSystem_PrintFiles(true);
}

