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
#include "dust/info.h"
#include "dust/ustring.h"
#include "dust/error.h"
#include "dust/platform.h"
#include "dust/tokenizer.h"
#include "dust/parser.h"
#include "dust/transpiler.h"


typedef enum {
    cmd_none,    // no command was passed
    cmd_unknown, // unknown command was passed
    cmd_help,
    cmd_version,
    cmd_tokenize,
    cmd_parse,
    cmd_transpile
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

        else if (strcmp(argv[1], "transpile") == 0) {
            ap->cmd = cmd_transpile;

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

        else {
            ap->cmd = cmd_unknown;
        }
    }
}


int main(int argc, char *argv[]) {
    Platform platform = get_platform();

    if (u32isequal(platform.name, U"Windows")) system(" ");

    ArgumentParser *ap = ArgumentParser_new();
    ArgumentParser_parse(ap, argc, argv);

    if (ap->cmd == cmd_none) {
        printf("Use 'dust help' to see available commands\n");
    }

    else if (ap->cmd == cmd_help) {
        printf("Dust Programming Language - Command Line Interface\n\n"
                "dust help      :  Information about CLI\n"
                "dust version   :  Version related information\n"
                "dust tokenize  :  Tokenize source code\n"
                "dust parse     :  Parse source code into syntax tree\n"
                "dust transpile :  Transpile source code to C\n");
    }

    else if (ap->cmd == cmd_version) {
        printf("Dust     : %s\n"
                "Compiler : %s %s\n"
                "Platform : %s\n",
                DUST_VERSION_STR,
                COMPILER, COMPILER_VERSION_STR,
                utf32_to_utf8(platform.prettyname));
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
            tokens = tokenize(utf8_to_utf32(argv[2]));
        }

        printf("%s\n", utf32_to_utf8(TokenArray_repr(tokens)));

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
            tokens = tokenize(utf8_to_utf32(argv[2]));
        }

        Node *expr = parse_body(tokens);

        printf("%s\n", utf32_to_utf8(Node_repr(expr, 0)));

        TokenArray_free(tokens);
        Node_free(expr);
    }

    else if (ap->cmd == cmd_transpile) {
        TokenArray *tokens = TokenArray_new(1);

        if (ap->opt_nocolor) {
            ERROR_ANSI = 0;
        }

        if (ap->opt_fp) {
            tokens = tokenize_file(argv[2]);
        }
        else {
            tokens = tokenize(utf8_to_utf32(argv[2]));
        }

        Node *expr = parse_body(tokens);

        transpile(expr->body);

        TokenArray_free(tokens);
        Node_free(expr);
    }

    else if (ap->cmd == cmd_unknown) {
        printf("Unknown command '%s'\nUse 'dust help' to see available commands\n", argv[1]);
    }


    free(ap);

    return 0;
}