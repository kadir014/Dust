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
#include "dust/ansi.h"
#include "dust/info.h"
#include "dust/ustring.h"
#include "dust/error.h"
#include "dust/platform.h"
#include "dust/tokenizer.h"
#include "dust/parser.h"
#include "dust/transpiler.h"


enum command {
    cmd_unknown,
    cmd_tokenize,
    cmd_parse,
    cmd_transpile
};

enum option {
    opt_none,    // no option was passed
    opt_help,    // -h | --help
    opt_version, // -v | --version
};

// [-h | -v] <command> [-c string | path] [-d path] [-n] [args...]
struct arg {
    enum option opt;
    enum command cmd;
    char *cmdstr;
    bool ispath;
    char *path;
    bool isdpath;
    char *dpath;
    bool nocolor;
    char *argv[];
};

struct arg parse_args(int argc, char *argv[]) {
    struct arg args;
    args.nocolor = false;
    args.isdpath = false;

    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        args.opt = opt_help;
        return args;
    }
    else if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
        args.opt = opt_version;
        return args;
    }
    else args.opt = opt_none;

    if (!strcmp(argv[1], "tokenize")) {
        args.cmd = cmd_tokenize;
        args.cmdstr = argv[1];
    }
    else if (!strcmp(argv[1], "parse")) {
        args.cmd = cmd_parse;
        args.cmdstr = argv[1];
    }
    else if (!strcmp(argv[1], "transpile")) {
        args.cmd = cmd_transpile;
        args.cmdstr = argv[1];
    }
    else {
        args.cmd = cmd_unknown;
        args.cmdstr = argv[1];
    }

    if (argc > 2) {
        int i = 2;
        if (!strcmp(argv[2], "-c")) {
            args.ispath = false;
            args.path = argv[3];
            i++;
        }
        else {
            args.ispath = true;
            args.path = argv[2];
        }

        if (argc > i+1) {
            int j = i;
            if (!strcmp(argv[i+1], "-d") || !strcmp(argv[i+1], "--dest")) {
                args.isdpath = true;
                args.dpath = argv[i+2];
                i++;
                j+=3;
            }
            else {
                args.isdpath = false;
                j++;
            }

            if (argc > j) {
                if (!strcmp(argv[j], "-n") || !strcmp(argv[j], "--no-color")) {
                    args.nocolor = true;
                }
            }
        }
    }

    return args;
}


int main(int argc, char *argv[]) {
    Platform platform = get_platform();
    if (OS == OS_WINDOWS) system(" ");

    struct arg args = parse_args(argc, argv);

    if (args.opt == opt_help) {

        printf("Usage: dust [-h | -v] <command> [-c string | path] [-d path] [-n] [args...]\n"
                "\n"
                "Options and arguments:\n"
                "-h | --help     : prints help message\n"
                "-v | --version  : prints Dust and related version information\n"
                "-c              : accepts a string as source code instead of a file\n"
                "-d | --dest     : writes the tokenized/parsed result into a file\n"
                "-n | --no-color : disables ANSI coloring in outputs\n"
                "\n"
                "Commands:\n"
                "tokenize  : tokenizes the source code and prints tokens\n"
                "parse     : parses the source code and prints the syntax tree\n"
                "transpile : transpiles the source into C code (experimental)\n");
    }

    else if (args.opt == opt_version) {
        printf("Dust     : %s\n"
                "Compiler : %s %s\n"
                "Platform : %s\n",
                DUST_VERSION_STR,
                COMPILER,
                COMPILER_VERSION_STR,
                utf32_to_utf8(platform.prettyname));
    }

    else {

        if (args.cmd == cmd_unknown) {
            printf("Unknown command: %s\n"
                    "Try 'dust -h' for more information\n",
                    args.cmdstr);
        }

        else if (args.cmd == cmd_tokenize) {
            TokenArray *tokens;

            if (args.nocolor) ERROR_ANSI = 0;

            if (args.ispath) tokens = tokenize_file(args.path);
            else tokens = tokenize(utf8_to_utf32(args.path));

            printf("%s", utf32_to_utf8(TokenArray_repr(tokens)));

            TokenArray_free(tokens);
        }

        else if (args.cmd == cmd_parse) {
            TokenArray *tokens;

            if (args.nocolor) ERROR_ANSI = 0;

            if (args.ispath) tokens = tokenize_file(args.path);
            else tokens = tokenize(utf8_to_utf32(args.path));

            Node *expr = parse_body(tokens);

            printf("%s", utf32_to_utf8(Node_repr(expr, 0)));

            TokenArray_free(tokens);
            Node_free(expr);
        }

        else if (args.cmd == cmd_transpile) {
            if (args.nocolor)
                printf("WARNING: Transpiler is still experimental and might be depreceated in the future.\n");
            else
                printf("%sWARNING%s: Transpiler is still experimental and might be depreceated in the future.\n",
                        ANSI_FG_LIGHTRED, ANSI_END);

            TokenArray *tokens;

            if (args.nocolor) ERROR_ANSI = 0;

            if (args.ispath) tokens = tokenize_file(args.path);
            else tokens = tokenize(utf8_to_utf32(args.path));

            Node *expr = parse_body(tokens);

            transpile(expr->body);

            TokenArray_free(tokens);
            Node_free(expr);
        }
    }

    return 0;
}