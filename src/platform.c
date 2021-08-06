/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include "dust/u8string.h"
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#elif defined(__linux__)
#include <sys/utsname.h>
#endif

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
u8char *get_osname() {
#if defined(_WIN32)
    return L"Windows";
#elif defined(__linux__)
    return L"Linux";
#elif defined(__APPLE__)
    return L"MacOS";
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

typedef struct {
    u8char *name;
    u8char *kernel;
    u8char *hostname;
    u8char *version;
    u8char *prettyname;
} Platform;

/*
  Get plaftorm information
*/
Platform *get_platform(){
    Platform *platform = (Platform *)malloc(sizeof(Platform));

    #if defined(_WIN32)

    platform->name = L"Windows";
    platform->kernel = L"Windows";

    // https://svsivan.blogspot.com/2020/07/get-os-version-c.html
    // GetVersion functions doesn't work on Windows >= 8
    NTSTATUS(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
    OSVERSIONINFOEXW osinfo;
    *(FARPROC *)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

    if (NULL != RtlGetVersion) {
        osinfo.dwOSVersionInfoSize = sizeof(osinfo);
        RtlGetVersion(&osinfo);
    }

    if (osinfo.dwMajorVersion == 10) {
        platform->version = L"10";
    }
    else if (osinfo.dwMajorVersion == 6) {
        if (osinfo.dwMinorVersion == 3) {
            platform->version = L"8.1";
        }
        else if (osinfo.dwMinorVersion == 2) {
            platform->version = L"8";
        }
        else if (osinfo.dwMinorVersion == 1) {
            platform->version = L"7";
        }
        else if (osinfo.dwMinorVersion == 0) {
            platform->version = L"Vista";
        }
    }
    else if (osinfo.dwMajorVersion == 5) {
        if (osinfo.dwMinorVersion == 2 || osinfo.dwMinorVersion == 1) {
            platform->version = L"XP";
        }
        else if (osinfo.dwMinorVersion == 0) {
            platform->version = L"2000";
        }
    }

    platform->prettyname = u8join(platform->name, u8join(L" ", platform->version));

    char hostname[64];
    gethostname(hostname, 64);
    u8char *uhostname = (u8char *)malloc(sizeof(u8char)*64);
    swprintf(uhostname, u8size(uhostname), L"%hs", hostname);

    platform->hostname = uhostname;

    #elif defined(__linux__)

    struct utsname uts;
    uname(&uts);
    
    u8char *kernel = (u8char *)malloc(sizeof(u8char)*24);
    swprintf(kernel, u8size(kernel), L"%hs", uts.sysname);
    platform->kernel = kernel;

    u8char *hostname = (u8char *)malloc(sizeof(u8char)*64);
    swprintf(hostname, u8size(hostname), L"%hs", uts.nodename);
    platform->hostname = hostname;

    u8char *version = (u8char *)malloc(sizeof(u8char)*12);
    swprintf(version, u8size(version), L"%hs", uts.release);
    platform->version = version;

    #elif defined(__APPLE__)

    platform->kernel = L"Unix";

    #endif

    return platform;
}