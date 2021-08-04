/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include "dust/u8string.h"


/*
  Get GNU C compiler's version in major.minor.patch format
*/
u8char _GCC_VERSION_STRING[8];
u8char *get_gcc_version() {
    swprintf(_GCC_VERSION_STRING, 8, L"%d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    return _GCC_VERSION_STRING;
}

/*
  Get generic OS name
*/
u8char *get_platform() {
#if defined(_WIN32)
    return L"Windows";
#elif defined(__APPLE__)
    return L"MacOS";
#elif defined(__linux__)
    return L"Linux";
#elif defined(__FreeBSD__)
    return L"FreeBSD";
#elif defined(__NetBSD__)
    return L"NetBSD"
#elif defined(__OpenBSD__)
    return L"OpenBSD"
#elif defined(__DragonFly__)
    return L"DragonFly"
#elif defined(__CYGWIN__)
    return L"Cygwin"
#elif defined(__amigaos__)
    return L"AmigaOS"
#elif defined(__ANDROID__)
    return L"Android"
#else
    return L"other";
#endif
}