/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include "dust/u8string.h"
#include "dust/ansi.h"


typedef enum {
    ErrorType_Syntax,
} ErrorType;


int ERROR_ANSI = 1;

void raise_ansi(ErrorType type, u8char *message, u8char *source, int x, int y) {
    u8char *type_repr;
    switch (type) {
        case ErrorType_Syntax:
            type_repr = L"SyntaxError";
            break;
    }

    wprintf(L"\n%ls %ls%d%ls:%ls%d\n%ls%ls%ls: %ls%ls\n%ls...\n#%d %lsline\n",
            source, ANSI_FG_YELLOW, x, ANSI_END, ANSI_FG_YELLOW, (y+1),
            ANSI_FG_LIGHTRED, type_repr, ANSI_FG_DARKGRAY, ANSI_END, message,
            ANSI_FG_DARKGRAY, (y+1),
            ANSI_END);
            
    exit(1);
}

void raise_noansi(ErrorType type, u8char *message, u8char *source, int x, int y) {
    u8char *type_repr;
    switch (type) {
        case ErrorType_Syntax:
            type_repr = L"SyntaxError";
            break;
    }

    wprintf(L"\n%ls %d:%d\n%ls: %ls\n...\n#%d line\n",
            source, x, (y+1), type_repr, message, (y+1));
            
    exit(1);
}

void raise(ErrorType type, u8char *message, u8char *source, int x, int y) {
    switch (ERROR_ANSI) {
        case 1:
            raise_ansi(type, message, source, x, y);
            break;

        case 0:
            raise_noansi(type, message, source, x, y);
            break;
    }
}

void raise_internal(u8char *message) {
    switch (ERROR_ANSI) {
        case 1:
            wprintf(L"%lsInternalError%ls:%ls %ls",
                    ANSI_FG_LIGHTRED, ANSI_FG_DARKGRAY, ANSI_END, message);

            exit(1);

        case 0:
            wprintf(L"InternalError: %ls",
                    ANSI_FG_LIGHTRED, ANSI_FG_DARKGRAY, ANSI_END, message);

            exit(1);
    }
}