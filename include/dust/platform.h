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

enum {
  OS_UNKNOWN,
  OS_WINDOWS,
  OS_LINUX,
  OS_MACOS,
  OS_FREEBSD,
  OS_NETBSD,
  OS_OPENBSD,
  OS_DRAGONFLY,
  OS_AMIGAOS,
  OS_ANDROID
};

#if defined(_WIN32)
#define OS OS_WINDOWS
#define OS_STR "Windows"

#elif defined(__linux__)
#define OS OS_LINUX
#define OS_STR "Linux"

#elif defined(__APPLE__)
#define OS OS_MACOS
#define OS_STR "MacOS"

#elif defined(__FreeBSD__)
#define OS OS_FREEBSD
#define OS_STR "FreeBSD"

#elif defined(__NetBSD__)
#define OS OS_NETBSD
#define OS_STR "NetBSD"

#elif defined(__OpenBSD__)
#define OS OS_OPENBSD
#define OS_STR "OpenBSD"

#elif defined(__DragonFly__)
#define OS OS_DRAGONFLY
#define OS_STR "DragonFly"

#elif defined(__amigaos__)
#define OS OS_AMIGAOS
#define OS_STR "AmigaOS"

#elif defined(__ANDROID__)
#define OS OS_ANDROID
#define OS_STR "Android"

#else
#define OS OS_UNKNOWN
#define OS_STR "unknown"

#endif

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