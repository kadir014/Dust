/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "dust/u8string.h"
#include "dust/tokenizer.h"


u8char _GCC_VERSION_STRING[8];
u8char *get_gcc_version() {
    swprintf(_GCC_VERSION_STRING, 8, L"%d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    return _GCC_VERSION_STRING;
}

#ifdef _WIN32
u8char *get_platform() {
    return L"Windows";
}
#elif __APPLE__
u8char *get_platform() {
    return L"MacOS";
}
#elif __linux__
u8char *get_platform() {
    return L"Linux";
}
#elif __FreeBSD__
u8char *get_platform() {
    return L"FreeBSD";
}
#else
u8char *get_platform() {
    return L"other";
}
#endif


int main(int argc, char *argv[]) {
    if (argc == 1) {
        wprintf(L"\nUse 'dust help' to see available commands\n");
    }

    else {
        if (strcmp(argv[1], "help") == 0) {
            wprintf(L"\nDust Programming Language - Command Line Interface\n\n"
                    L"dust help                  : Information about CLI\n"
                    L"dust version               : Version related information\n"
                    L"dust tokenize source [-fp] : Tokenizes the source, -fp arg. for filepath\n"
                    L"\n");
        }

        else if (strcmp(argv[1], "version") == 0) {
            wprintf(L"\n"
                    L"Dust version : 0.0.1\n"
                    L"GCC version  : %s\n"
                    L"Platform     : %s\n"
                    L"\n", get_gcc_version(), get_platform());
        }

        else if (strcmp(argv[1], "tokenize") == 0) {
            TokenArray *tokens = TokenArray_new(1);

            if (argc == 4 && (strcmp(argv[3], "-fp")) == 0) {
                tokens = tokenize_file(argv[2]);
            }
            else {
                wchar_t ws[500];
                swprintf(ws, 500, L"%hs", argv[2]);
                tokens = tokenize(ws);
            }

            wprintf(L"\n%ls\n", TokenArray_repr(tokens));

            TokenArray_free(tokens);
        }
    }

    return 0;
}