/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#pragma once
#ifndef PARSER_H
#define PARSER_H


#include <stdlib.h>
#include "dust/ustring.h"
#include "dust/tokenizer.h"

typedef enum {
    NodeType_INTEGER,
    NodeType_FLOAT,
    NodeType_STRING,
    NodeType_VAR,
    NodeType_PRIMITIVE,
    NodeType_ARRAY,
    NodeType_DECL,
    NodeType_DECLN,
    NodeType_ASSIGN,
    NodeType_BINOP,
    NodeType_UNARYOP,
    NodeType_RUNARYOP,
    NodeType_IMPORT,
    NodeType_IMPORTF,
    NodeType_CHILD,
    NodeType_SUBSCRIPT,
    NodeType_CALL,
    NodeType_FUNCBASE,
    NodeType_ENUM,
    NodeType_BODY,
    NodeType_GENTYPE,
    NodeType_IF,
    NodeType_ELIF,
    NodeType_ELSE,
    NodeType_WHEN,
    NodeType_REPEAT,
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
    OpType_GE,
    OpType_IN
} OpType;


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

        u32char *string;

        u32char *variable;

        u32char *primitive;

        struct {
            NodeArray *array_nodearray;
            bool array_empty;
        };

        struct {
            struct _Node *decl_type;
            u32char *decl_var;
            struct _Node *decl_expr;
        };

        struct {
            struct _Node *decln_type;
            u32char *decln_var;
        };

        struct {
            u32char *assign_var;
            struct _Node *assign_expr;
        };

        struct {
            struct _Node *call_base;
            NodeArray *call_args;
        };

        u32char *func_base;

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
            u32char *import_module;
            u32char *import_member;
        };

        struct {
            struct _Node *subs_node;
            struct _Node *subs_expr;
        };

        struct {
            struct _Node *chld_parent;
            struct _Node *chld_child;
        };

        struct {
            u32char *enum_name;
            struct _Node *enum_body;
        };

        struct {
            NodeArray *body;
            int body_tokens;
        };

        struct {
            NodeArray *gentype;
            int gentype_tokens;
        };

        struct {
            struct _Node *if_expr;
            struct _Node *if_body;
        };

        struct {
            struct _Node *elif_expr;
            struct _Node *elif_body;
        };

        struct _Node *else_body;

        struct {
            struct _Node *repeat_expr;
            struct _Node *repeat_body;
        };

        struct {
            struct _Node *while_expr;
            struct _Node *while_body;
        };
        
        struct {
            struct _Node *for_var;
            struct _Node *for_expr;
            struct _Node *for_body;
        };
    };
};
typedef struct _Node Node;

Node *NodeInteger_new(long integer);

Node *NodeFloat_new(double floating);

Node *NodeString_new(u32char *str);

Node *NodeCall_new(Node *call_base, NodeArray *call_args);

Node *NodeFuncBase_new(u32char *func_base);

Node *NodeVar_new(u32char *variable);

Node *NodeNArray_new(NodeArray *node_array, bool empty);

Node *NodeDecl_new(Node *type, u32char *variable, Node *expression);

Node *NodeDecln_new(Node *type, u32char *variable);

Node *NodeAssign_new(u32char *variable, Node *expression);

Node *NodeBinOp_new(OpType op, Node *left, Node *right);

Node *NodeUnaryOp_new(OpType op, Node *right);

Node *NodeImport_new(u32char *module);

Node *NodeImportFrom_new(u32char *module, u32char *member);

Node *NodeSubscript_new(Node *snode, Node *expr);

Node *NodeChild_new(Node *parent, Node *child);

Node *NodeEnum_new(u32char *name, Node *body);

Node *NodeBody_new(NodeArray *node_array, int tokens);

Node *NodeGenType_new(NodeArray *node_array, int tokens);

Node *NodeIf_new(Node *expression, Node *body);

Node *NodeElif_new(Node *expression, Node *body);

Node *NodeElse_new(Node *body);

Node *NodeRepeat_new(Node *expression, Node *body);

Node *NodeWhile_new(Node *expression, Node *body);

Node *NodeFor_new(Node *var, Node *iterator, Node *body);

void Node_free(Node *node);

u32char *Node_repr(Node *node, int ident);

NodeArray *NodeArray_new(size_t def_size);

void NodeArray_free(NodeArray *node_array);

void NodeArray_append(NodeArray *node_array, Node *node);

Node *parse_expr(TokenArray *tokens);

Node *parse_enum(TokenArray *tokens);

Node *parse_body(TokenArray *tokens);

OpType get_optype(u32char *tokenval);

Token *current_token(TokenArray *tokens);

void next_token(TokenArray *tokens);

char expect_token(TokenArray *tokens, TokenType type);

Node *parse_child(TokenArray *tokens, Node *node);

Node *parse_subscript(TokenArray *tokens, Node *node);

Node *parse_call(TokenArray *tokens, Node *node);

Node *parse_expr_FACTOR(TokenArray *tokens);

Node *parse_expr_POW(TokenArray *tokens);

Node *parse_expr_TERM(TokenArray *tokens);

Node *parse_expr_EXPR(TokenArray *tokens);

#endif