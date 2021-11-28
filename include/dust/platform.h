/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#pragma once
#ifndef PLATFORM_H
#define PLATFORM_H


#include "dust/ustring.h"

u32char *get_gcc_version();

u32char *get_osname();

typedef struct {
    u32char *name;
    u32char *kernel;
    u32char *hostname;
    u32char *version;
    u32char *prettyname;
} Platform;

Platform get_platform();

typedef struct {
    u32char *name;
    int corecount;
} CPUInfo;

CPUInfo *get_cpuinfo();


#endif