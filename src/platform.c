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
#include "dust/platform.h"

#if OS == OS_WINDOWS
#include <winsock2.h>
#include <windows.h>

#elif OS == OS_LINUX
#include <sys/utsname.h>

#elif OS == OS_MACOS
#include <CoreServices/CoreServices.h>
#include <sys/utsname.h>

#endif


/**
 * @param name OS Name
 * @param kernel Kernel name
 * @param hostname Device host/node name
 * @param version OS version
 * @param prettyname Pretty OS name (Usually OS name + version)
 */
typedef struct {
    u32char *name;
    u32char *kernel;
    u32char *hostname;
    u32char *version;
    u32char *prettyname;
} Platform;

/**
 * @brief Get platform information
 * 
 * @return Platform object
 */
Platform get_platform(){
    Platform platform;

    #if OS == OS_WINDOWS

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


    #elif OS == OS_LINUX

    FILE *fp;
    char line[128];

    fp = fopen("/etc/os-release", "r");
    if (fp == NULL) raise_internal(U"reading file '/etc/os-release' failed");

    while (fgets(line, sizeof(line), fp) != NULL) {
        u32char *uline = utf8_to_utf32(line);

        if (u32startswith(uline, U"NAME=")) {
            u32char *new = u32strip(u32slice(uline, u32find(uline, U"=")+1, u32len(uline)));
            platform.name = new;
        }

        else if (u32startswith(uline, U"PRETTY_NAME=")) {
            u32char *new = u32strip(u32slice(uline, u32find(uline, U"=")+1, u32len(uline)));
            platform.prettyname = new;
        }
    }

    fclose(fp);

    struct utsname uts;
    uname(&uts);

    if (u32startswith(platform.name, U"Manjaro") || u32startswith(platform.name, U"Ubuntu")) {
        fp = fopen("/etc/lsb-release", "r");
        if (fp == NULL) raise_internal(U"reading file '/etc/lsb-release' failed");

        while (fgets(line, sizeof(line), fp) != NULL) {
            u32char *uline = utf8_to_utf32(line);

            if (u32startswith(uline, U"DISTRIB_RELEASE=")) {
                u32char *new = u32strip(u32slice(uline, u32find(uline, U"=")+1, u32len(uline)));
                platform.version = new;
            }
        }

        fclose(fp);
    }
    else {
        platform.version = utf8_to_utf32(uts.release)
    }
    
    platform.kernel = utf8_to_utf32(uts.sysname);

    platform.hostname = utf8_to_utf32(uts.nodename);


    #elif OS == OS_MACOS

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wno-deprecated"

    SInt32 osx_major_version, osx_minor_version, osx_patch_version;

    Gestalt(gestaltSystemVersionMajor, &osx_major_version);
    Gestalt(gestaltSystemVersionMinor, &osx_minor_version);
    Gestalt(gestaltSystemVersionBugFix, &osx_patch_version);

    #pragma GCC diagnostic pop

    struct utsname uts;
    uname(&uts);

    platform.kernel = utf8_to_utf32(uts.sysname);
    platform.name = U"MacOS";
    platform.hostname = utf8_to_utf32(uts.nodename);

    char *osxverstr;
    sprintf(osxverstr, "%u.%u.%u",
            osx_major_version,
            osx_minor_version,
            osx_patch_version);
    platform.version = utf8_to_utf32(osxverstr);

    // https://en.wikipedia.org/wiki/MacOS_version_history
    if (osx_major_version == 10 && osx_minor_version == 0) {
        platform.prettyname = U"MacOS Cheetah";
    }
    else if (osx_major_version == 10 && osx_minor_version == 1) {
        platform.prettyname = U"MacOS Puma";
    }
    else if (osx_major_version == 10 && osx_minor_version == 2) {
        platform.prettyname = U"MacOS Jaguar";
    }
    else if (osx_major_version == 10 && osx_minor_version == 3) {
        platform.prettyname = U"MacOS Panther";
    }
    else if (osx_major_version == 10 && osx_minor_version == 4) {
        platform.prettyname = U"MacOS Tiger";
    }
    else if (osx_major_version == 10 && osx_minor_version == 5) {
        platform.prettyname = U"MacOS Leopard";
    }
    else if (osx_major_version == 10 && osx_minor_version == 6) {
        platform.prettyname = U"MacOS Snow Leopard";
    }
    else if (osx_major_version == 10 && osx_minor_version == 7) {
        platform.prettyname = U"MacOS Lion";
    }
    else if (osx_major_version == 10 && osx_minor_version == 8) {
        platform.prettyname = U"MacOS Mountain Lian";
    }
    else if (osx_major_version == 10 && osx_minor_version == 9) {
        platform.prettyname = U"MacOS Mavericks";
    }
    else if (osx_major_version == 10 && osx_minor_version == 10) {
        platform.prettyname = U"MacOS Yosemite";
    }
    else if (osx_major_version == 10 && osx_minor_version == 11) {
        platform.prettyname = U"MacOS El Capitan";
    }
    else if (osx_major_version == 10 && osx_minor_version == 12) {
        platform.prettyname = U"MacOS Sierra";
    }
    else if (osx_major_version == 10 && osx_minor_version == 13) {
        platform.prettyname = U"MacOS High Sierra";
    }
    else if (osx_major_version == 10 && osx_minor_version == 14) {
        platform.prettyname = U"MacOS Mojave";
    }
    else if (osx_major_version == 10 && osx_minor_version == 15) {
        platform.prettyname = U"MacOS Catalina";
    }
    else if (osx_major_version == 11) {
        platform.prettyname = U"MacOS Big Sur";
    }
    else if (osx_major_version == 12) {
        platform.prettyname = U"MacOS Monterey";
    }
    else {
        platform.prettyname = U"MacOS";
    }

    #endif

    return platform;
}