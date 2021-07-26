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
#include "dust/platform.h"
#include "dust/tokenizer.h"
#include "dust/parser.h"


int main(int argc, char *argv[]) {
    if (u8isequal(get_platform(), L"Windows")) {
        u8winterminal();
    }

    if (argc == 1) {
        wprintf(L"\nUse 'dust help' to see available commands\n");
    }

    else {
        if (strcmp(argv[1], "help") == 0) {
            wprintf(L"\nDust Programming Language - Command Line Interface\n\n"
                    L"dust help                  : Information about CLI\n"
                    L"dust version               : Version related information\n"
                    L"dust tokenize source [-fp] : Tokenizes the source, -fp for filepath\n"
                    L"dust parse source [-fp]    : Parse the source, -fp for filepath\n");
        }

        else if (strcmp(argv[1], "version") == 0) {
            wprintf(L"\n"
                    L"Dust version : 0.0.2\n"
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

        else if (strcmp(argv[1], "parse") == 0) {
            wprintf(L"not implemented");
        }
    }

    return 0;
}