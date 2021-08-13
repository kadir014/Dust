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
    OpType_HAS
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

        struct {
            struct _Node *call_base;
            NodeArray *call_args;
        };

        u8char *func_base;

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
            u8char *import_module;
            u8char *import_member;
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
            u8char *enum_name;
            struct _Node *enum_body;
        };

        struct {
            NodeArray *body;
            int body_tokens;
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
    };
};
typedef struct _Node Node;

Node *NodeInteger_new(long integer);

Node *NodeFloat_new(double floating);

Node *NodeString_new(u8char *str);

Node *NodeCall_new(Node *call_base);

Node *NodeFuncBase_new(u8char *func_base);

Node *NodeVar_new(u8char *variable);

Node *NodeDecl_new(DeclType type, u8char *variable, Node *expression);

Node *NodeAssign_new(u8char *variable, Node *expression);

Node *NodeBinOp_new(OpType op, Node *left, Node *right);

Node *NodeUnaryOp_new(OpType op, Node *right);

Node *NodeImport_new(u8char *module);

Node *NodeImportFrom_new(u8char *module, u8char *member);

Node *NodeSubscript_new(Node *snode, Node *expr);

Node *NodeChild_new(Node *parent, Node *child);

Node *NodeEnum_new(u8char *name, Node *body);

Node *NodeBody_new(NodeArray *node_array, int tokens);

Node *NodeIf_new(Node *expression, Node *body);

Node *NodeElif_new(Node *expression, Node *body);

Node *NodeElse_new(Node *body);

Node *NodeRepeat_new(Node *expression, Node *body);

Node *NodeWhile_new(Node *expression, Node *body);

void Node_free(Node *node);

u8char *Node_repr(Node *node, int ident);

NodeArray *NodeArray_new(size_t def_size);

void NodeArray_free(NodeArray *node_array);

void NodeArray_append(NodeArray *node_array, Node *node);

Node *parse_expr(TokenArray *tokens);

Node *parse_body(TokenArray *tokens);


#endif