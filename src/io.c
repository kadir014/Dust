/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include "dust/ustring.h"
#include "dust/platform.h"

#if OS == OS_WINDOWS
#include <windows.h>

#elif OS == OS_LINUX
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#endif


/**
 * @brief Read file into multibyte UTF-8 encoded string
 * 
 * @param path Path to file
 * @return File content as string
 */
char *read_file(char *path) {
    FILE *f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = (char *)malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);

    string[fsize] = '\0';

    return string;
}

/**
 * @brief Write string on file
 * 
 * @param path Path to file
 */
void write_file(char *path, char *content) {

}

/**
 * @brief Create a file
 * 
 * @param path Path to file
 * @return Non-zero if file creation succeded
 */
int create_file(char *path) {
    FILE *f = fopen(path, "w");
    if (f == NULL) return 0;
    fclose(f);
    return 1;
}

/**
 * @brief Remove a file
 * 
 * @param path Path to file
 * @return Non-zero if file deletion succeded
 */
int remove_file(char *path) {
    if (!remove(path)) return 1;
    else return 0;
}

/**
 * @brief Create a directory
 * 
 * @param path Path to directory
 * @return Non-zero if directory creation succeded
 */
int create_dir(char *path) {
    #if OS == OS_WINDOWS

    if (CreateDirectoryA(path, NULL)) return 1;
    else return 0;


    #elif OS == OS_LINUX

    if (!mkdir(path, 0777)) return 1;
    else return 0;

    #endif
}

/**
 * @brief Remove a directory
 * 
 * @param path Path to directory
 * @return Non-zero if file deletion succeded
 */
int remove_dir(char *path) {
    #if OS == OS_WINDOWS

    if (RemoveDirectoryA(path)) return 1;
    else return 0;


    #elif OS == OS_LINUX

    if (!rmdir(path)) return 1;
    else return 0;

    #endif
}