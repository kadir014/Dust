/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "dust/u8string.h"
#include "dust/error.h"
#include "dust/platform.h"
#include "dust/tokenizer.h"
#include "dust/parser.h"


int main(int argc, char *argv[]) {
    Platform *platform = get_platform();

    if (u8isequal(platform->name, L"Windows")) {
        u8winterminal();
    }

    //TODO: Better argument parsing
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
                    L"Dust version : 0.0.11\n"
                    L"GCC version  : %s\n"
                    L"Platform     : %s\n"
                    L"\n", get_gcc_version(), platform->prettyname);
        }

        else if (strcmp(argv[1], "tokenize") == 0) {
            TokenArray *tokens = TokenArray_new(1);

            if (argc == 4 && strcmp(argv[3], "-nocolor") == 0) {
                ERROR_ANSI = 0;
            }

            if (argc == 4 && strcmp(argv[3], "-fp") == 0) {
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
            TokenArray *tokens = TokenArray_new(1);

            if (argc == 4 && (strcmp(argv[3], "-nocolor")) == 0) {
                ERROR_ANSI = 0;
            }

            if (argc == 4 && (strcmp(argv[3], "-fp")) == 0) {
                tokens = tokenize_file(argv[2]);
            }
            else {
                wchar_t ws[500];
                swprintf(ws, 500, L"%hs", argv[2]);
                tokens = tokenize(ws);
            }

            Node *expr = parse_body(tokens);

            wprintf(L"\n%ls\n", Node_repr(expr, 0));

            TokenArray_free(tokens);
            Node_free(expr);
        }

        else {
            wprintf(L"\nUnknown command '%hs'\nUse 'dust help' to see available commands\n", argv[1]);
        }
    }

    free(platform);

    return 0;
}