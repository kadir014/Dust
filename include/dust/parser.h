/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#ifndef PARSER_H
#define PARSER_H


typedef enum {
    NodeType_INTEGER,
    NodeType_FLOAT,
    NodeType_STRING,
    NodeType_VAR,
    NodeType_DECL,
    NodeType_ASSIGN,
    NodeType_IMPORT,
    NodeType_BINOP,
    NodeType_UNARYOP,
    NodeType_RUNARYOP,
    NodeType_CHILD,
    NodeType_SUBSCRIPT,
    NodeType_CALL,
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
    OpType_DIV,
    OpType_AND,
    OpType_OR,
    OpType_XOR,
    OpType_NOT,
    OpType_POW,
    OpType_RANGE,
    OpType_EQ,
    OpType_NEQ,
    OpType_LT,
    OpType_LE,
    OpType_GT,
    OpType_GE
} OpType;


typedef enum {
    DeclType_INT8,
    DeclType_INT16,
    DeclType_INT64,
    DeclType_INT32,
    DeclType_INT128,
    DeclType_UINT8,
    DeclType_UINT16,
    DeclType_UINT32,
    DeclType_UINT64,
    DeclType_UINT128,
    DeclType_FLOAT32,
    DeclType_FLOAT64,
    DeclType_BOOL,
    DeclType_STRING,
    DeclType_BUFFER
} DeclType;

struct _Node;

typedef struct {
    struct _Node *array;
    size_t size;
    size_t used;
} NodeArray;

struct _Node {
    NodeType type;
    union {
        long integer;

        double floating;

        u8char *string;

        u8char *variable;

        struct {
            DeclType decl_type;
            u8char *decl_var;
            struct _Node *decl_expr;
        };

        struct {
            u8char *assign_var;
            struct _Node *assign_expr;
        };

        u8char *call_base;

        struct {
            OpType bin_optype;
            struct _Node *bin_left;
            struct _Node *bin_right;
        };

        struct {
            OpType unary_optype;
            struct _Node *unary_right;
        };

        struct {
            NodeArray *body;
            int body_tokens;
        };
    };
};
typedef struct _Node Node;

Node *NodeInteger_new(long integer);

Node *NodeFloat_new(double floating);

Node *NodeBinOp_new(OpType op, Node *left, Node *right);

Node *NodeUnaryOp_new(OpType op, Node *right);

void Node_free(Node *node);

u8char *Node_repr(Node *node, int ident);

NodeArray *NodeArray_new(size_t def_size);

void NodeArray_free(NodeArray *node_array);

void NodeArray_append(NodeArray *node_array, Node *node);

Node *parse_expr(TokenArray *tokens);

Node *parse_body(TokenArray *tokens);


#endif