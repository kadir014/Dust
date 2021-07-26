/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#ifndef PARSER_H
#define PARSER_H


typedef enum {
    NodeType_VAR,
    NodeType_NUMERIC,
    NodeType_STRING,
    NodeType_IMPORT,
    NodeType_BINOP,
    NodeType_UNARYOP,
    NodeType_RUNARYOP,
    NodeType_CHILD,
    NodeType_SUBSCRIPT,
    NodeType_FUNCCALL,
    NodeType_BODY,
    NodeType_IF,
    NodeType_WHEN,
    NodeType_FOR,
    NodeType_WHILE
} NodeType;

typedef enum {
    OpType_ADD,
    OpType_SUB,
    OpType_MUL,
    OpType_DIV
} OpType;

struct _Node {
    NodeType type;
    union {
        double numeric;
        struct {
            OpType bin_operator;
            struct _Node *bin_left;
            struct _Node *bin_right;
        };
        struct {
            OpType unary_operator;
            struct _Node *unary_right;
        };
    };
};
typedef struct _Node Node;

Node *NodeNumeric_new(double numeric);

Node *NodeBinOp_new(OpType op, Node *left, Node *right);

Node *NodeUnaryOp_new(OpType op, Node *right);

void Node_free(Node *node);

u8char *Node_repr(Node *node, int ident);


#endif