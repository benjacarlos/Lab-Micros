/***************************************************************************/ /**
  @file    	sd_file.h
  @brief    SD File System Handle
  @author   Grupo 5
 ******************************************************************************/

#include "sd_file.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ff.h"

#define FILE_ARRAY_SIZE 200

/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

Mp3File_t files[FILE_ARRAY_SIZE] = {};
int filesCount = 0;

static void SD_File_ScanHelper(char * path);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void SD_File_Scan(void)
{
	char path_temp[FILE_ARRAY_SIZE] = {0U};

	FileSystem_ScanHelper(path_temp);
}

bool SD_File_isFileMp3(char *path)
{
	char *extension;
	if ((extension = strrchr(path, '.')) != NULL)
	{
		if (strcmp(extension, ".mp3") == 0)
		{
			return true;
		}
	}
	return false;
}

void SD_File_Add(char *path)
{
	Mp3File_t *newFile = &files[filesCount];
	strcpy(newFile->path, path);
	newFile->index = filesCount;
	filesCount++;
}

Mp3File_t SD_File_GetFirst(void)
{
	if (filesCount == 0)
	{
		Mp3File_t nullFile = {.index = -1, .path = ""};
		return nullFile;
	}
	return files[0];
}

Mp3File_t SD_File_GetNext(Mp3File_t currentFile)
{
	int nextFileIndex = currentFile.index + 1;
	if (nextFileIndex == filesCount)
	{
		nextFileIndex = 0;
	}
	return files[nextFileIndex];
}

Mp3File_t SD_File_GetPrevious(Mp3File_t currentFile)
{
	int previousFileIndex = currentFile.index - 1;
	if (currentFile.index == 0)
	{
		previousFileIndex = filesCount - 1;
	}
	return files[previousFileIndex];
}

char *SD_File_GetName(Mp3File_t file)
{
	char str[STR_SIZE];
	strcpy(str, file.path);
	char *pch;
	pch = strtok(str, "/");
	char fileNamePart[STR_SIZE];
	while (pch != NULL)
	{
		//printf("%s\n", pch);
		strcpy(fileNamePart, pch);
		pch = strtok(NULL, "/");
	}
	char *fileName;
	fileName = strtok(fileNamePart, ".");
	return fileName;
}


void SD_File_PrintFiles(bool completePath)
{
	printf("Files list: \n");
	for (int i = 0; i < filesCount; i++)
	{
		if (completePath)
		{
			printf("Track: %d -> %s\n", i, files[i].path);
		}
		else
		{
			char *fileName = SD_File_GetFileName(files[i]);
			printf("Track: %d -> %s\n", i, fileName);
		}
	}
	printf("\n");
}

int SD_File_GetFilesCount(void)
{
	return filesCount;
}

Mp3File_t SD_File_ResetFiles(void)
{
	filesCount = 0;
	return SD_File_GetFirstFile();
}

Mp3File_t SD_File_GetFirstFile(void)
{
	if (filesCount == 0)
	{
		Mp3File_t nullFile = {.index = -1, .path = ""};
		return nullFile;
	}
	return files[0];
}

//void SD_File_Test(void)
//{
//	/*TEST*/
//	char *filename = "/dir0/hola.mp3";
//	char *filename2 = "/dir0/hola2.mp3";
//	char *filename3 = "/dir11/dir1/hola3.mp3";
//	char *filename4 = "/hola4.mp3";
//	char *filename5 = "/dir2/hola5.mp3";
//	SD_File_AddFile(filename);
//	SD_File_AddFile(filename2);
//	SD_File_AddFile(filename3);
//	SD_File_AddFile(filename4);
//	SD_File_AddFile(filename5);
//
//	/* Test File Addition & Test Get File Name */
//	printf("Test File Addition & Test Get File Name\n");
//	for (int i = 0; i < filesCount; i++)
//	{
//		printf("File n%d path: %s\n", i, files[i].path);
//		char *fileName = SD_File_GetFileName(files[i]);
//		printf("File n%d name: %s\n", i, fileName);
//	}
//	printf("\n");
//	/* Test Mp3 Recognition */
//	printf("Test Mp3 Recognition\n");
//	if (SD_File_isMp3File("/dir0/archivo0.mp3"))
//		printf("archivo0 es mp3\n");
//	if (SD_File_isMp3File("/dir0/archivo1.xls"))
//		printf("archivo1 es mp3\n");
//	if (SD_File_isMp3File("/dir0"))
//		printf("dir0 es mp3\n");
//
//	printf("\n");
//
//	/* Test Get First File */
//	printf("Test Get First File\n");
//	Mp3File_t currentFile = SD_File_GetFirstFile();
//	printf("Primer Archivo: %s\n", currentFile.path);
//	printf("\n");
//
//	/*Test File System Navigation*/
//	printf("Test File System Navigation\n");
//	for (int j = 0; j < 2; j++)
//	{
//		currentFile = SD_File_GetNextFile(currentFile);
//		printf("Archivo Siguiente: %s\n", currentFile.path);
//	}
//
//	for (int k = 0; k < 2; k++)
//	{
//		currentFile = SD_File_GetPreviousFile(currentFile);
//		printf("Anterior Archivo: %s\n", currentFile.path);
//	}
//
//	printf("\n");
//	/*Test File System Overflow*/
//	printf("Test File System Overflow\n");
//	currentFile = SD_File_GetFirstFile();
//	printf("Index: %d\n", currentFile.index);
//	for (int l = 0; l < 10; l++)
//	{
//		currentFile = SD_File_GetNextFile(currentFile);
//		printf("Index Siguiente: %d\n", currentFile.index);
//	}
//	for (int m = 0; m < 10; m++)
//	{
//		currentFile = SD_File_GetPreviousFile(currentFile);
//		printf("Index Anterior: %d\n", currentFile.index);
//	}
//	printf("\n");
//}

/*************************************************************************************
 * 		LOCAL FUNCTIONS DECLARATIONS
 ************************************************************************************/
static void SD_File_ScanHelper(char * path)
{
	FRESULT error;
	DIR directory; /* Directory object */
	FILINFO fileInformation;

	if (f_opendir(&directory, path))
	{
		printf("Open directory failed.\r\n");
		return;
	}
	for (;;)
	{
		error = f_readdir(&directory, &fileInformation);
		if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
		{
			break;
		}
		if (fileInformation.fname[0] == '.')
		{
			continue;
		}
		if (fileInformation.fattrib & AM_DIR)
		{
			int i = strlen(path);
			char * fn = fileInformation.fname;
			*(path+i) = '/'; strcpy(path+i+1, fn);
			FileSystem_ScanHelper(path);
			*(path+i) = 0;
		}
		else
		{
			int i = strlen(path);
			char * fn = fileInformation.fname;
			*(path+i) = '/'; strcpy(path+i+1, fn);

			if (FileSystem_isMp3File(path))
				FileSystem_AddFile(path);

			*(path+i) = 0;
		}
	}
	f_closedir(&directory);
}
