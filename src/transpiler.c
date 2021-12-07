/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust


  WARNING: Dust Transpiler (transpiler.c & dust/transpiler.h)
  is still an experimental part of Dust, and may change or
  get removed in the future

*/

#include <stdio.h>
#include <stdlib.h>
#include "dust/ustring.h"
#include "dust/parser.h"
#include "dust/transpiler.h"


void transpile(NodeArray *node_array) {
    u32char *final = U"";
    size_t i = 0;

    while (i < node_array->used) {
        Node *node = &(node_array->array[i]);

        switch (node->type) {
            case NodeType_DECL:
                final = u32join(final, u32join(translate_decl(node), U"\n"));
        }

        i++;
    }

    printf("/* Transpiled from Dust */\n\n#include <stdint.h>\n\n\n%s", utf32_to_utf8(final));
}

u32char *translate_expr(Node *node) {
    char transtr[15];
    char tmp[15];

    switch (node->type) {
        case NodeType_INTEGER:
            sprintf(tmp, "%ld", node->integer);
            return ascii_to_utf32(tmp);
            break;

        case NodeType_FLOAT:
            sprintf(tmp, "%lf", node->floating);
            return ascii_to_utf32(tmp);
            break;

        case NodeType_STRING:
            return node->string;
            break;

        case NodeType_BINOP:
            sprintf(transtr, "(%s%s%s)",
                    utf32_to_utf8(translate_expr(node->bin_left)),
                    utf32_to_utf8(translate_op(node->bin_optype)),
                    utf32_to_utf8(translate_expr(node->bin_right)));
            break;

        case NodeType_UNARYOP:
            sprintf(transtr, "(%s%s)",
                    utf32_to_utf8(translate_op(node->unary_optype)),
                    utf32_to_utf8(translate_expr(node->unary_right)));
            break;
    }

    return utf8_to_utf32(transtr);
}

u32char *translate_op(OpType op) {
    switch (op) {
        case OpType_ADD: return U"+"; break;
        case OpType_SUB: return U"-"; break;
        case OpType_MUL: return U"*"; break;
        case OpType_DIV: return U"/"; break;
        case OpType_AND: return U"and"; break;
        case OpType_OR: return U"or"; break;
        case OpType_XOR: return U"xor"; break;
        case OpType_NOT: return U"not"; break;
        case OpType_POW: return U"^"; break;
        case OpType_RANGE: return U".."; break;
        case OpType_EQ: return U"=="; break;
        case OpType_NEQ: return U"!="; break;
        case OpType_LT: return U"<"; break;
        case OpType_LE: return U"<="; break;
        case OpType_GT: return U">"; break;
        case OpType_GE: return U">="; break;
        case OpType_IN: return U"in"; break;
    }
}

u32char *translate_decl(Node *node) {
    char transtr[15];

    sprintf(transtr, "int32_t %s = %s;",
            utf32_to_utf8(node->decl_var),
            utf32_to_utf8(translate_expr(node->decl_expr)));

    return utf8_to_utf32(transtr);
}