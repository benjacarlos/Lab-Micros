/***************************************************************************/ /**
  @file     file_system_manager.h
  @brief    File System header
  @author   Grupo 2 - Lab de Micros
 ******************************************************************************/

#ifndef FILE_SYSTEM_MANAGER_H_
#define FILE_SYSTEM_MANAGER_H_

#include <stdbool.h>
#define STR_SIZE 255
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct
{
  char path[STR_SIZE];
  int index;
} Mp3File_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/*
 * @brief Scan all mp3 files in the storage device
 * @param path: base dir to start searching
 */
void FileSystem_ScanFiles();

/*
 * @brief Detects if a file's path corresponds to a .mp3 file.
 * */
bool FileSystem_isMp3File(char *path);
/*
 * @brief Adds a file to the file system.
 * @param path: complete file's path.
 *
 * */
void FileSystem_AddFile(char *path);

/*
 * @brief Gets the first file of the file system.
 *
 * */
Mp3File_t FileSystem_GetFirstFile(void);

/*
 * @brief Gets the next file of the file system.
 * @param currentFile: The file that is considered the 'current' one. The 'next' file will be defined based on this param.
 * @return Mp3File_t next file of the file system.  If the current file is the last one, returns the first file.
 *
 * */
Mp3File_t FileSystem_GetNextFile(Mp3File_t currentFile);

/*
 * @brief Gets the previous file of the file system.
 * @param currentFile: The file that is considered the 'current' one. The 'previous' file will be defined based on this param.
 * @return Mp3File_t previous file of the file system.  If the current file is the first one, returns the last file.
 *
 * */
Mp3File_t FileSystem_GetPreviousFile(Mp3File_t currentFile);

/*
 * @brief Gets the file's name of a Mp3File_t file.
 * @param file: The file whose file's name is wanted.
 * @return char * file's name. (Ex.: if file's path is "/dir0/dir1/hello.mp3", it's file's name is "hello");
 *
 * */
char *FileSystem_GetFileName(Mp3File_t file);

/*
 * @brief Runs tests on each function of the FileSystem's driver.
 * */
void FileSystem_Test(void);

/*
 * @brief Prints all the files of the file system.
 * @param completePath.
 * 				true: prints the complete path of the file.
 * 				false: prints only the file's name.
 *
 * */
void FileSystem_PrintFiles(bool completePath);

/*
 * @brief Gets the amount of files that exist in the file system.
 */

int FileSystem_GetFilesCount(void);

/*
 * @brief Reset the file system and return a new first file.
 *
 * */
Mp3File_t FileSystem_ResetFiles(void);

#endif /* FILE_SYSTEM_MANAGER_H_ */
