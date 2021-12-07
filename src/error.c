/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include "dust/ustring.h"
#include "dust/ansi.h"


typedef enum {
    ErrorType_Syntax,
} ErrorType;

char *Error_repr(ErrorType type) {
    switch (type) {
        case ErrorType_Syntax:
            return "SyntaxError";
            break;
    }
}


int ERROR_ANSI = 1;

void raise_ansi(ErrorType type, u32char *message, u32char *source, int x, int y) {
    printf("\n%s %s%d%s:%s%d\n%s%s%s: %s%s\n%s...\n#%d %sline\n",
           utf32_to_utf8(source), ANSI_FG_YELLOW, (y+1), ANSI_END, ANSI_FG_YELLOW, x,
           ANSI_FG_LIGHTRED, Error_repr(type), ANSI_FG_DARKGRAY, ANSI_END, utf32_to_utf8(message),
           ANSI_FG_DARKGRAY, (y+1),
           ANSI_END);
            
    exit(1);
}

void raise_noansi(ErrorType type, u32char *message, u32char *source, int x, int y) {
    printf("\n%s %d:%d\n%s: %s\n...\n#%d line\n",
           utf32_to_utf8(source), (y+1), x, Error_repr(type), utf32_to_utf8(message), (y+1));
            
    exit(1);
}

void raise(ErrorType type, u32char *message, u32char *source, int x, int y) {
    switch (ERROR_ANSI) {
        case 1:
            raise_ansi(type, message, source, x, y);
            break;

        case 0:
            raise_noansi(type, message, source, x, y);
            break;
    }
}

void raise_internal(u32char *message) {
    switch (ERROR_ANSI) {
        case 1:
            printf("%sInternalError%s:%s %s",
                    ANSI_FG_LIGHTRED, ANSI_FG_DARKGRAY, ANSI_END, utf32_to_utf8(message));

            exit(1);

        case 0:
            printf("InternalError: %s", utf32_to_utf8(message));

            exit(1);
    }
}