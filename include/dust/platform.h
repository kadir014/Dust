/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#ifndef PLATFORM_H
#define PLATFORM_H


#include "dust/u8string.h"

u8char *get_gcc_version();

u8char *get_osname();

typedef struct {
    u8char *name;
    u8char *kernel;
    u8char *hostname;
    u8char *version;
    u8char *prettyname;
} Platform;

Platform *get_platform();

typedef struct {
    u8char *name;
    int corecount;
} CPUInfo;

CPUInfo *get_cpuinfo();


#endif