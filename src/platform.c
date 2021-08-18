/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include "dust/u8string.h"
#include "dust/error.h"
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#elif defined(__linux__)
#include <sys/utsname.h>
#endif


/*
  Get GNU C compiler's version in major.minor.patch format
*/
u8char *get_gcc_version() {
    u8char *result = (u8char *)malloc(10*sizeof(u8char));
    swprintf(result, 10, L"%d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    return result;
}


/*
  Get generic platform name
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


/*
  name        ->  OS name
  kernel      ->  Kernel name
  hostname    ->  Device host/nodename
  version     ->  OS version
  prettyname  ->  Usually OS name + version
*/
typedef struct {
    u8char *name;
    u8char *kernel;
    u8char *hostname;
    u8char *version;
    u8char *prettyname;
} Platform;

/*
  Get plaftorm information, return Platform struct
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

    FILE *fp;
    char line[128];

    fp = popen("lsb_release -a", "r");
    if (fp == NULL) {
        raise_internal(L"popen failed");
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        u8char uline[128];
        swprintf(uline, 128, L"%hs", line);

        if (u8startswith(uline, L"Distributor ID:")) {
            u8char *new = u8strip(u8slice(uline, u8find(uline, L":")+1, u8len(uline)));
            platform->name = new;
        }

        else if (u8startswith(uline, L"Description:")) {
            u8char *new = u8strip(u8slice(uline, u8find(uline, L":")+1, u8len(uline)));
            platform->prettyname = new;
        }

        else if (u8startswith(uline, L"Release:")) {
            u8char *new = u8strip(u8slice(uline, u8find(uline, L":")+1, u8len(uline)));
            platform->version = new;
        }
    }

    pclose(fp);

    struct utsname uts;
    uname(&uts);
    
    u8char *kernel = (u8char *)malloc(sizeof(u8char)*24);
    swprintf(kernel, u8size(kernel), L"%hs", uts.sysname);
    platform->kernel = kernel;

    u8char *hostname = (u8char *)malloc(sizeof(u8char)*64);
    swprintf(hostname, u8size(hostname), L"%hs", uts.nodename);
    platform->hostname = hostname;


    #elif defined(__APPLE__)

    // TODO
    platform->kernel = L"Unix";
    platform->name = L"unknown";
    platform->hostname = L"unknown";
    platform->prettyname = L"unknown";
    platform->version = L"unknown";

    #endif

    return platform;
}