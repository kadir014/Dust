/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include "dust/ustring.h"
#include "dust/error.h"
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#elif defined(__linux__)
#include <sys/utsname.h>
#endif


/**
 * @brief Get GNU C compiler's version in major.minor.patch format
 * @return Version string
 */
u32char *get_gcc_version() {
    char gccver[11];
    sprintf(gccver, "%d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    return ascii_to_utf32(gccver);
}


/*
  Get generic system name
*/
u32char *get_osname() {
#if defined(_WIN32)
    return U"Windows";
#elif defined(__linux__)
    return U"Linux";
#elif defined(__APPLE__)
    return U"MacOS";
#elif defined(__FreeBSD__)
    return U"FreeBSD";
#elif defined(__NetBSD__)
    return U"NetBSD"
#elif defined(__OpenBSD__)
    return U"OpenBSD"
#elif defined(__DragonFly__)
    return U"DragonFly"
#elif defined(__CYGWIN__)
    return U"Cygwin"
#elif defined(__amigaos__)
    return U"AmigaOS"
#elif defined(__ANDROID__)
    return U"Android"
#else
    return U"other";
#endif
}


/*
  u8char *name        ->  OS name
  u8char *kernel      ->  Kernel name
  u8char *hostname    ->  Device host/node name
  u8char *version     ->  OS version
  u8char *prettyname  ->  Usually OS name + version
*/
typedef struct {
    u32char *name;
    u32char *kernel;
    u32char *hostname;
    u32char *version;
    u32char *prettyname;
} Platform;

/*
  Get plaftorm information, return Platform struct
*/
Platform get_platform(){
    Platform platform;

    #if defined(_WIN32)

    platform.name = U"Windows";
    platform.kernel = U"Windows";

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
        platform.version = U"10";
    }
    else if (osinfo.dwMajorVersion == 6) {
        if (osinfo.dwMinorVersion == 3) {
            platform.version = U"8.1";
        }
        else if (osinfo.dwMinorVersion == 2) {
            platform.version = U"8";
        }
        else if (osinfo.dwMinorVersion == 1) {
            platform.version = U"7";
        }
        else if (osinfo.dwMinorVersion == 0) {
            platform.version = U"Vista";
        }
    }
    else if (osinfo.dwMajorVersion == 5) {
        if (osinfo.dwMinorVersion == 2 || osinfo.dwMinorVersion == 1) {
            platform.version = U"XP";
        }
        else if (osinfo.dwMinorVersion == 0) {
            platform.version = U"2000";
        }
    }

    platform.prettyname = u32join(platform.name, u32join(U" ", platform.version));

    char hostname[64];
    gethostname(hostname, 64);
    platform.hostname = utf8_to_utf32(hostname);


    #elif defined(__linux__)

    FILE *fp;
    char line[128];

    fp = popen("lsb_release -a", "r");
    if (fp == NULL) {
        raise_internal(L"popen failed");
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        u32char uline[128] = utf8_to_utf32(line);

        if (u32startswith(uline, L"Distributor ID:")) {
            u32char *new = u32strip(u32slice(uline, u32find(uline, L":")+1, u32len(uline)));
            platform.name = new;
        }

        else if (u32startswith(uline, L"Description:")) {
            u32char *new = u32strip(u32slice(uline, u32find(uline, L":")+1, u32len(uline)));
            platform.prettyname = new;
        }

        else if (u32startswith(uline, L"Release:")) {
            u32char *new = u32strip(u32slice(uline, u32find(uline, L":")+1, u32len(uline)));
            platform.version = new;
        }
    }

    pclose(fp);

    struct utsname uts;
    uname(&uts);
    
    platform.kernel = utf8_to_utf32(uts.sysname);

    platform.hostname = utf8_to_utf32(uts.nodename);


    #elif defined(__APPLE__)

    // TODO
    platform.kernel = L"Unix";
    platform.name = L"unknown";
    platform.hostname = L"unknown";
    platform.prettyname = L"unknown";
    platform.version = L"unknown";

    #endif

    return platform;
}


#ifdef ALLOW_CPU_INFO
/*
  u8char *name   ->  Processor name
  int corecount  ->  Core count
*/
typedef struct {
    u32char *name;
    int corecount;
} CPUInfo;

/*
  Get processor information, return CPUInfo struct
*/
CPUInfo *get_cpuinfo() {
    CPUInfo *cpuinfo = (CPUInfo *)malloc(sizeof(CPUInfo));

    #if defined(_WIN32)

    FILE *fp;
    char line[128];

    fp = popen("wmic cpu get NumberOfCores", "r");
    if (fp == NULL) {
        raise_internal(L"popen failed");
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        u32char *uline = (u32char *)malloc(128*sizeof(u8char));
        swprintf(uline, 128, L"%hs", line);
        uline = u8replace(uline, L"\n", L"");
        uline = u8strip(uline);

        if (!u8startswith(uline, L"NumberOfCores") && wcslen(uline) > 0) {
            u8char *end;
            int corecount = wcstol(uline, &end, 10);
            cpuinfo->corecount = corecount;
        }
    }
    pclose(fp);

    FILE *fp_;
    char line_[128];

    fp_ = popen("wmic cpu get Name", "r");
    if (fp_ == NULL) {
        raise_internal(L"popen failed");
    }

    while (fgets(line_, sizeof(line_), fp_) != NULL) {
        u8char *uline = (u8char *)malloc(128*sizeof(u8char));
        swprintf(uline, 128, L"%hs", line_);
        uline = u8replace(uline, L"\n", L"");
        uline = u8strip(uline);

        if (!u8startswith(uline, L"Name") && wcslen(uline) > 0) {
            cpuinfo->name = uline;
        }
    }
    pclose(fp_);

    #endif

    return cpuinfo;
}
#endif