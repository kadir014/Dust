/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <wchar.h>
#include "dust/u8string.h"
#include "dust/error.h"
#include "dust/platform.h"
#include "dust/tokenizer.h"
#include "dust/parser.h"


typedef enum {
    cmd_none,    // no command was passed
    cmd_unknown, // unknown command was passed
    cmd_help,
    cmd_version,
    cmd_tokenize,
    cmd_parse
} CommandType;

typedef struct {
    bool opt_fp;      // --fp
    bool opt_nocolor; // --no-color

    CommandType cmd;
} ArgumentParser;

ArgumentParser *ArgumentParser_new() {
    ArgumentParser *ap = (ArgumentParser *)malloc(sizeof(ArgumentParser));
    ap->opt_fp = false;
    ap->opt_nocolor = false;
    return ap;
}

void ArgumentParser_parse(ArgumentParser *ap, int argc, char *argv[]) {
    if (argc == 1) {
        ap->cmd = cmd_none;
        return;
    }
    else {
        if (strcmp(argv[1], "help") == 0) {
            ap->cmd = cmd_help;
            return;
        }
        else if (strcmp(argv[1], "version") == 0) {
            ap->cmd = cmd_version;
            return;
        }
        else if (strcmp(argv[1], "tokenize") == 0) {
            ap->cmd = cmd_tokenize;

            // no need to check options/flags
            if (argc == 2) return;

            int i = 0;
            while (i < argc) {
                if (strcmp(argv[i], "--fp") == 0) {
                    ap->opt_fp = true;
                }
                else if (strcmp(argv[i], "--no-color") == 0) {
                    ap->opt_nocolor = true;
                }
                i++;
            }

            return;
        }
        else if (strcmp(argv[1], "parse") == 0) {
            ap->cmd = cmd_parse;

            // no need to check options/flags
            if (argc == 2) return;

            int i = 0;
            while (i < argc) {
                if (strcmp(argv[i], "--fp") == 0) {
                    ap->opt_fp = true;
                }
                else if (strcmp(argv[i], "--no-color") == 0) {
                    ap->opt_nocolor = true;
                }
                i++;
            }

            return;
        }
    }
}


int main(int argc, char *argv[]) {
    Platform *platform = get_platform();

    u8terminal();

    ArgumentParser *ap = ArgumentParser_new();
    ArgumentParser_parse(ap, argc, argv);

    if (ap->cmd == cmd_none) {
        wprintf(L"\nUse 'dust help' to see available commands\n\n");
    }

    else if (ap->cmd == cmd_help) {
        wprintf(L"\nDust Programming Language - Command Line Interface\n\n"
                L"dust help      :  Information about CLI\n"
                L"dust version   :  Version related information\n"
                L"dust tokenize  :  Tokenize source code\n"
                L"dust parse     :  Parse source code\n"
                L"\n");
    }

    else if (ap->cmd == cmd_version) {
        wprintf(L"\n"
                L"Dust version : 0.0.15\n"
                L"GCC version  : %ls\n"
                L"Platform     : %ls\n"
                L"\n", get_gcc_version(), platform->prettyname);
    }
    else if (ap->cmd == cmd_tokenize) {
        TokenArray *tokens = TokenArray_new(1);

        if (ap->opt_nocolor) {
            ERROR_ANSI = 0;
        }

        if (ap->opt_fp) {
            tokens = tokenize_file(argv[2]);
        }
        else {
            tokens = tokenize(ctou8(argv[2]));
        }

        wprintf(L"\n%ls\n\n", TokenArray_repr(tokens));

        TokenArray_free(tokens);
    }

    else if (ap->cmd == cmd_parse) {
        TokenArray *tokens = TokenArray_new(1);

        if (ap->opt_nocolor) {
            ERROR_ANSI = 0;
        }

        if (ap->opt_fp) {
            tokens = tokenize_file(argv[2]);
        }
        else {
            tokens = tokenize(ctou8(argv[2]));
        }

        Node *expr = parse_body(tokens);

        wprintf(L"\n%ls\n\n", Node_repr(expr, 0));

        TokenArray_free(tokens);
        Node_free(expr);
    }

    else if (ap->cmd == cmd_unknown) {
        wprintf(L"\nUnknown command '%hs'\nUse 'dust help' to see available commands\n\n", argv[1]);
    }


    free(platform);
    free(ap);

    return 0;
}