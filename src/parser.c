/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "dust/u8string.h"
#include "dust/error.h"


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
        /* NUMERIC */
        double numeric;

        /* BINOP */
        struct {
            OpType bin_optype;
            struct _Node *bin_left;
            struct _Node *bin_right;
        };

        /* UNARYOP */
        struct {
            OpType unary_optype;
            struct _Node *unary_right;
        };
    };
};
typedef struct _Node Node;

/*
  Create a new Numeric Node and return its pointer

  double numeric  ->  Value of node
*/
Node *NodeNumeric_new(double numeric) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_NUMERIC;
    node->numeric = numeric;
    return node;
}

/*
  Create a new Binary Operator Node and return its pointer

  OpType op    ->  Type of the operator
  Node* left   ->  Left-hand node
  Node* right  ->  Right-hand node
*/
Node *NodeBinOp_new(OpType op, Node *left, Node *right) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_BINOP;
    node->bin_optype = op;
    node->bin_left = left;
    node->bin_right = right;
    return node;
}

/*
  Create a new Unary Operator Node and return its pointer

  OpType op    ->  Type of the operator
  Node *right  ->  Right-hand node
*/
Node *NodeUnaryOp_new(OpType op, Node *right) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_UNARYOP;
    node->unary_optype = op;
    node->unary_right = right;
    return node;
}

/*
  Release all resources used by the Node

  Node *node  ->  Node to free
*/
void Node_free(Node *node) {
    switch (node->type) {
        case NodeType_NUMERIC:
            break;

        case NodeType_BINOP:
            Node_free(node->bin_left);
            Node_free(node->bin_right);
            break;

        case NodeType_UNARYOP:
            Node_free(node->unary_right);
            break;
    }

    free(node);
}

/*
  Return a string representation of Node

  Node *node  ->  Node to return a repr. string of
*/
u8char *Node_repr(Node *node, int ident) {
    u8char *finalstr = L"";

    u8char numstr[50];
    u8char *identstr = L"";
    identstr = u8fill(identstr, L"  ", (ident+1)*2);

    switch (node->type) {
        case NodeType_NUMERIC:
            swprintf(numstr, 50, L"%f", node->numeric);
            finalstr = u8join(u8join(finalstr, u8join(L"number: ", numstr)), L"\n");
            break;

        case NodeType_BINOP:
            finalstr = u8join(finalstr, L"binop:\n");
            
            switch (node->bin_optype) {
                case OpType_ADD:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: +\n"));
                    break;

                case OpType_SUB:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: -\n"));
                    break;

                case OpType_MUL:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: *\n"));
                    break;

                case OpType_DIV:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: /\n"));
                    break;
            }

            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->bin_left, ident+1)));
            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->bin_right, ident+1)));
            break;

        case NodeType_UNARYOP:
            finalstr = u8join(finalstr, L"unaryop:\n");
            
            switch (node->unary_optype) {
                case OpType_ADD:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: +\n"));
                    break;

                case OpType_SUB:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: -\n"));
                    break;
            }

            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->unary_right, ident+1)));
            break;
    }

    return finalstr;
}