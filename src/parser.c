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
#include "dust/tokenizer.h"


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
        /* INTEGER LITERAL */
        long integer;

        /* FLOAT LITERAL */
        double floating;

        /* STRING LITERAL*/
        u8char *string;

        /* VARIABLE */
        u8char *variable;

        /* DECLERATION */
        struct {
            DeclType decl_type;
            u8char *decl_var;
            struct _Node *decl_expr;
        };

        /* ASSIGNMENT */
        struct {
            u8char *assign_var;
            struct _Node *assign_expr;
        };

        /* FUNCTION/CLASS CALL */
        struct {
            struct _Node *call_base;
            NodeArray *call_args;
        };

        /* FUNCTION BASE */
        u8char *func_base;

        /* BINARY OPERATOR */
        struct {
            OpType bin_optype;
            struct _Node *bin_left;
            struct _Node *bin_right;
        };

        /* UNARY OPERATOR */
        struct {
            OpType unary_optype;
            struct _Node *unary_right;
        };

        /* IMPORT */
        struct {
            u8char *import_module;
            u8char *import_member;
        };

        /* SUBSCRIPT */
        struct {
            struct _Node *subs_node;
            struct _Node *subs_expr;
        };

        /* CHILD */
        struct {
            struct _Node *chld_parent;
            struct _Node *chld_child;
        };

        /* ENUMERATION */
        struct {
            u8char *enum_name;
            struct _Node *enum_body;
        };

        /* BODY */
        struct {
            NodeArray *body;
            int body_tokens;
        };

        /* IF */
        struct {
            struct _Node *if_expr;
            struct _Node *if_body;
        };

        /* ELIF */
        struct {
            struct _Node *elif_expr;
            struct _Node *elif_body;
        };

        /* ELSE */
        struct _Node *else_body;

        /* REPEAT */
        struct {
            struct _Node *repeat_expr;
            struct _Node *repeat_body;
        };

        /* WHILE */
        struct {
            struct _Node *while_expr;
            struct _Node *while_body;
        };
    };
};
typedef struct _Node Node;

/*
  Create a new Integer Node and return its pointer

  long integer  ->  Value of node
*/
Node *NodeInteger_new(long integer) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_INTEGER;
    node->integer = integer;
    return node;
}

/*
  Create a new Float Node and return its pointer

  double floating  ->  Value of node
*/
Node *NodeFloat_new(double floating) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_FLOAT;
    node->floating = floating;
    return node;
}

/*
  Create a new String Node and return its pointer

  u8char *str  ->  Value of node
*/
Node *NodeString_new(u8char *str) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_STRING;
    node->string = str;
    return node;
}

/*
  Create a new Function/Class Call Node and return its pointer

  u8char *call_base  ->  Value of node
  u8char *call_args  ->  Argument array (might be NULL)
*/
Node *NodeCall_new(Node *call_base, NodeArray *call_args) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_CALL;
    node->call_base = call_base;
    node->call_args = call_args;
    return node;
}

/*
  Create a new Function Base Node and return its pointer

  u8char *func_base  ->  Function base
*/
Node *NodeFuncBase_new(u8char *func_base) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_FUNCBASE;
    node->func_base = func_base;
    return node;
}

/*
  Create a new Variable Node and return its pointer

  u8char *variable  ->  Value of node
*/
Node *NodeVar_new(u8char *variable) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_VAR;
    node->variable = variable;
    return node;
}

/*
  Create a new Decleration Node and return its pointer

  u8char *type      ->  Type of the variable
  u8char *var       ->  Identifier of variable
  Node *expression  ->  Decleration expression
*/
Node *NodeDecl_new(DeclType type, u8char *variable, Node *expression) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_DECL;
    node->decl_type = type;
    node->decl_var  = variable;
    node->decl_expr = expression;
    return node;
}

/*
  Create a new Assignment Node and return its pointer

  u8char *var       ->  Identifier of variable
  Node *expression  ->  Decleration expression
*/
Node *NodeAssign_new(u8char *variable, Node *expression) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_ASSIGN;
    node->assign_var  = variable;
    node->assign_expr = expression;
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
  Create a new Import Node and return its pointer

  u8char *module  ->  Name of the module
*/
Node *NodeImport_new(u8char *module) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_IMPORT;
    node->import_module = module;
    return node;
}

/*
  Create a new Relative Import Node and return its pointer

  u8char *module  ->  Name of the module
  u8char *member  ->  Member to import from module
*/
Node *NodeImportFrom_new(u8char *module, u8char *member) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_IMPORTF;
    node->import_module = module;
    node->import_member = member;
    return node;
}

/*
  Create a new Subscript (Indexing) Node and return its pointer

  Node *snode  ->  Node that is getting subscripted
  Node *expr   ->  Subscripting expression
*/
Node *NodeSubscript_new(Node *snode, Node *expr) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_SUBSCRIPT;
    node->subs_node = snode;
    node->subs_expr = expr;
    return node;
}

/*
  Create a new Child (Dot notation) Node and return its pointer

  Node *parent  ->  Parent node
  Node *child   ->  Child node
*/
Node *NodeChild_new(Node *parent, Node *child) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_CHILD;
    node->chld_parent = parent;
    node->chld_child  = child;
    return node;
}

/*
  Create a new Enumeration Node and return its pointer

  Node *name  ->  Identifier of enumeration
  Node *body  ->  Body of enumeration
*/
Node *NodeEnum_new(u8char *name, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_ENUM;
    node->enum_name = name;
    node->enum_body = body;
    return node;
}

/*
  Create a new Body Node and return its pointer

  NodeArray *node_array  ->  Array of statement nodes
  int tokens             ->  Number of tokens body includes
*/
Node *NodeBody_new(NodeArray *node_array, int tokens) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_BODY;
    node->body = node_array;
    node->body_tokens = tokens;
    return node;
}

/*
  Create a new If Node and return its pointer

  Node *expression  ->  If statement's condition
  Node *body        ->  If statement's body
*/
Node *NodeIf_new(Node *expression, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_IF;
    node->if_expr = expression;
    node->if_body = body;
    return node;
}

/*
  Create a new Elif (Else If) Node and return its pointer

  Node *expression  ->  Elif statement's condition
  Node *body        ->  Elif statement's body
*/
Node *NodeElif_new(Node *expression, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_ELIF;
    node->elif_expr = expression;
    node->elif_body = body;
    return node;
}

/*
  Create a new Else Node and return its pointer

  Node *body  ->  Else statement's body
*/
Node *NodeElse_new(Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_ELSE;
    node->else_body = body;
    return node;
}

/*
  Create a new Repeat Loop Node and return its pointer

  Node *expression  ->  Repeat loop's count expression
  Node *body        ->  Repeat loop's body
*/
Node *NodeRepeat_new(Node *expression, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_REPEAT;
    node->repeat_expr = expression;
    node->repeat_body = body;
    return node;
}

/*
  Create a new While Loop Node and return its pointer

  Node *expression  ->  While loop's condition
  Node *body        ->  While loop's body
*/
Node *NodeWhile_new(Node *expression, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_WHILE;
    node->while_expr = expression;
    node->while_body = body;
    return node;
}

/*
  Release all resources used by the Node

  Node *node  ->  Node to free
*/
void Node_free(Node *node) {
    switch (node->type) {
        case NodeType_STRING:
            free(node->string);
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
        case NodeType_INTEGER:
            swprintf(numstr, 50, L"%ld", node->integer);
            finalstr = u8join(u8join(finalstr, u8join(L"integer: ", numstr)), L"\n");
            break;

        case NodeType_FLOAT:
            swprintf(numstr, 50, L"%f", node->floating);
            finalstr = u8join(u8join(finalstr, u8join(L"float: ", numstr)), L"\n");
            break;

        case NodeType_STRING:
            finalstr = u8join(u8join(finalstr, u8join(L"string: ", node->string)), L"\n");
            break;

        case NodeType_VAR:
            finalstr = u8join(u8join(finalstr, u8join(L"var: ", node->variable)), L"\n");
            break;

        case NodeType_CALL:
            finalstr = u8join(finalstr, L"call:\n");
            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->call_base, ident+1)));
            if (node->call_args) {
                finalstr = u8join(finalstr, u8join(identstr, L"args:\n"));
                int i = 0;
                while (i < node->call_args->used) {
                    finalstr = u8join(finalstr, u8join(u8join(identstr, L"    "), Node_repr(&(node->call_args->array[i]), ident+2)));
                    i++;
                }
            }
            else {
                finalstr = u8join(finalstr, u8join(identstr, L"args: no args\n"));
            }
            break;

        case NodeType_FUNCBASE:
            finalstr = u8join(finalstr, u8join(L"function: ", u8join(node->func_base, L"\n")));
            break;

        case NodeType_DECL:
            finalstr = u8join(finalstr, L"declaration:\n");
            
            switch (node->decl_type) {
                case DeclType_INT8:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: int8\n"));
                    break;

                case DeclType_INT16:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: int16\n"));
                    break;

                case DeclType_INT32:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: int32\n"));
                    break;

                case DeclType_INT64:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: int64\n"));
                    break;

                case DeclType_INT128:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: int128\n"));
                    break;

                case DeclType_UINT8:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: uint8\n"));
                    break;

                case DeclType_UINT16:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: uint16\n"));
                    break;

                case DeclType_UINT32:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: uint32\n"));
                    break;

                case DeclType_UINT64:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: uint64\n"));
                    break;

                case DeclType_UINT128:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: uint128\n"));
                    break;

                case DeclType_FLOAT32:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: float32\n"));
                    break;

                case DeclType_FLOAT64:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: float64\n"));
                    break;

                case DeclType_BOOL:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: bool\n"));
                    break;

                case DeclType_STRING:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: string\n"));
                    break;

                case DeclType_BUFFER:
                    finalstr = u8join(finalstr, u8join(identstr, L"type: buffer\n"));
                    break;
            }

            finalstr = u8join(u8join(finalstr, u8join(identstr, u8join(L"var: ", node->decl_var))), L"\n");
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"expr: ", Node_repr(node->decl_expr, ident+1))));
            break;

        case NodeType_ASSIGN:
            finalstr = u8join(finalstr, L"assignment:\n");
            finalstr = u8join(u8join(finalstr, u8join(identstr, u8join(L"var: ", node->assign_var))), L"\n");
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"expr: ", Node_repr(node->assign_expr, ident+1))));
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

                case OpType_POW:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: ^\n"));
                    break;

                case OpType_RANGE:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: ..\n"));
                    break;

                case OpType_AND:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: and\n"));
                    break;

                case OpType_OR:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: or\n"));
                    break;

                case OpType_XOR:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: xor\n"));
                    break;

                case OpType_EQ:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: ==\n"));
                    break;

                case OpType_NEQ:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: !=\n"));
                    break;

                case OpType_LT:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: <\n"));
                    break;

                case OpType_LE:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: <=\n"));
                    break;

                case OpType_GT:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: >\n"));
                    break;

                case OpType_GE:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: >=\n"));
                    break;

                case OpType_HAS:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: has\n"));
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

                case OpType_NOT:
                    finalstr = u8join(finalstr, u8join(identstr, L"op: not\n"));
                    break;
            }

            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->unary_right, ident+1)));
            break;

        case NodeType_IMPORT:
            finalstr = u8join(finalstr, L"import:\n");
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"module: ", u8join(node->import_module, L"\n"))));
            break;

        case NodeType_IMPORTF:
            finalstr = u8join(finalstr, L"import:\n");
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"member: ", u8join(node->import_member, L"\n"))));
            finalstr = u8join(finalstr, u8join(identstr, L"from:\n"));
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"    ", u8join(L"module: ", u8join(node->import_module, L"\n")))));
            break;

        case NodeType_ENUM:
            finalstr = u8join(finalstr, L"enum:\n");
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"name: ", u8join(node->enum_name, L"\n"))));
            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->enum_body, ident+1)));
            break;

        case NodeType_BODY:
            finalstr = u8join(finalstr, L"body:\n");
            
            int i = 0;
            while (i < node->body->used) {
                finalstr = u8join(finalstr, u8join(identstr, Node_repr(&(node->body->array[i]), ident+1)));
                i++;
            }
            break;

        case NodeType_SUBSCRIPT:
            finalstr = u8join(finalstr, L"subscript:\n");
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"node: ", Node_repr(node->subs_node, ident+1))));
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"expr: ", Node_repr(node->subs_expr, ident+1))));
            break;

        case NodeType_CHILD:
            finalstr = u8join(finalstr, L"member:\n");
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"parent: ", Node_repr(node->subs_node, ident+1))));
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"child: ", Node_repr(node->subs_expr, ident+1))));
            break;

        case NodeType_IF:
            finalstr = u8join(finalstr, L"if:\n");
            finalstr = u8join(finalstr, u8join(identstr, L"condition:\n"));
            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->if_expr, ident+1)));
            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->if_body, ident+1)));
            break;

        case NodeType_ELIF:
            finalstr = u8join(finalstr, L"elif:\n");
            finalstr = u8join(finalstr, u8join(identstr, L"condition:\n"));
            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->if_expr, ident+1)));
            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->if_body, ident+1)));
            break;

        case NodeType_ELSE:
            finalstr = u8join(finalstr, L"else:\n");
            finalstr = u8join(finalstr, u8join(identstr, Node_repr(node->if_body, ident+1)));
            break;
    }

    return finalstr;
}


/*
  Create a new NodeArray and return its pointer

  size_t def_size  ->  Initial size of the array
*/
NodeArray *NodeArray_new(size_t def_size) {
    NodeArray *node_array = (NodeArray *)malloc(sizeof(NodeArray));

    node_array->array = malloc(def_size * sizeof(Node));
    node_array->used = 0;
    node_array->size = def_size;

    return node_array;
}

/*
  Release all resources used by the NodeArray

  NodeArray *node_array  ->  NodeArray to free
*/
void NodeArray_free(NodeArray *node_array) {
    free(node_array->array);
    node_array->array = NULL;
    node_array->used = 0;
    node_array->size = 0;
    free(node_array);
}

/*
  Append a Node to NodeArray

  NodeArray *node_array  ->  NodeArray to append to
  Node *node             ->  Node to append
*/
void NodeArray_append(NodeArray *node_array, Node *node) {
    if (node_array->used == node_array->size) {
        node_array->size *= 2;
        node_array->array = realloc(node_array->array, node_array->size * sizeof(Node));
    }

    node_array->array[node_array->used++] = *node;
}


DeclType get_decltype(u8char *tokenval);

Node *parse_expr(TokenArray *tokens);

Node *parse_expr_EXPR(TokenArray *tokens);

Node *parse_expr_FACTOR(TokenArray *tokens);

Node *parse_child(TokenArray *tokens, Node *node);

Node *parse_subscript(TokenArray *tokens, Node *node);

Node *parse_call(TokenArray *tokens, Node *node);

Token *current_token(TokenArray *tokens);

size_t _token_index = 0;
size_t _last_token_count = 0;
int _body_count = 0;


/*
  Parse an enumeration body and return a Node object

  TokenArray *tokens  ->  TokenArray to parse
*/
Node *parse_enum(TokenArray *tokens) {
    size_t i = 0;
    NodeArray *node_array = NodeArray_new(1);

    while (i < tokens->used) {
        Token *token = &(tokens->array[i]);

        if (token->type == TokenType_RCURLY) {
            break;
        }

        else if (token->type == TokenType_EOF) {
            break;
        }

        else if (token->type == TokenType_NEXTSTM) {
            raise(ErrorType_Syntax, L"Unexpected symbol ; in enumeration", L"<raw>", token->x, token->y);
        }

        else if (token->type == TokenType_COMMA) {
            if (tokens->array[i-1].type == TokenType_COMMA) {
                raise(ErrorType_Syntax, L"Statement expected before ,", L"<raw>", token->x, token->y);
            }
            i++;
            continue;
        }

        else if (token->type == TokenType_IDENTIFIER) {

            /* ASSIGNMENT   identifier = expression, */
            if ((&(tokens->array[i]))->type == TokenType_IDENTIFIER &&
               (&(tokens->array[i+1]))->type == TokenType_OPERATOR &&
                    u8isequal((&(tokens->array[i+1]))->data, L"=")) {
                
                    u8char *var = (&(tokens->array[i]))->data;

                    TokenArray *slice = TokenArray_slice(tokens, i+2);
                    TokenArray_append(slice, Token_new(TokenType_EOF, L""));
                    Node *expr = parse_expr(slice);
                    TokenArray_free(slice);

                    NodeArray_append(node_array, NodeAssign_new(var, expr));

                    i += _last_token_count+1;
                    continue;
            }
            
            else if (tokens->array[i+1].type == TokenType_COMMA ||
                     tokens->array[i+1].type == TokenType_RCURLY) {
                NodeArray_append(node_array, NodeVar_new(token->data));
                i += 2;
                continue;
            }
        }

        else {
            raise(ErrorType_Syntax, L"Unexpected field in enumeration", L"<raw>", token->x, token->y);
        }

        i++;
    }

    return NodeBody_new(node_array, i);
}


/*
  Parse a body and return a Node object

  TokenArray *tokens  ->  TokenArray to parse
*/
Node *parse_body(TokenArray *tokens) {
    size_t i = 0;
    NodeArray *node_array = NodeArray_new(1);

    while (i < tokens->used) {
        Token *token = &(tokens->array[i]);

        /* BODY   {statement; statement; ...} */
        if (token->type == TokenType_LCURLY) {
            _body_count++;

            TokenArray *slice = TokenArray_slice(tokens, i+1);
            Node *body = parse_body(slice);
            NodeArray_append(node_array, body);
            TokenArray_free(slice);

            i += body->body_tokens+2;
            continue;
        }

        /* End of body */
        else if (token->type == TokenType_RCURLY) {
            if (_body_count < 0) {
                raise(ErrorType_Syntax, L"Unexpected }", L"<raw>", token->x, token->y);
            }

            _body_count--;
            break;
        }

        else if (token->type == TokenType_EOF) {
            break;
        }

        else if (token->type == TokenType_NEXTSTM) {
            if (tokens->array[i-1].type == TokenType_NEXTSTM) {
                raise(ErrorType_Syntax, L"Statement expected before ;", L"<raw>", token->x, token->y);
            }
            i++;
            continue;
        }

        else if (token->type == TokenType_IDENTIFIER) {

            if (u8isequal(token->data, L"import")) {

                /* IMPORT   import module; */
                if (tokens->array[i+1].type == TokenType_IDENTIFIER &&
                    (tokens->array[i+2].type == TokenType_NEXTSTM   ||
                     tokens->array[i+2].type == TokenType_EOF)) {

                        NodeArray_append(node_array, NodeImport_new(tokens->array[i+1].data));
                        
                        i += 2;
                        continue;
                     }

                /* IMPORT   import member from module; */
                else if (tokens->array[i+1].type == TokenType_IDENTIFIER &&
                         tokens->array[i+2].type == TokenType_IDENTIFIER &&
                         u8isequal(tokens->array[i+2].data, L"from")     &&
                         tokens->array[i+3].type == TokenType_IDENTIFIER &&
                         (tokens->array[i+4].type == TokenType_NEXTSTM   ||
                          tokens->array[i+4].type == TokenType_EOF)) {

                        NodeArray_append(node_array, NodeImportFrom_new(tokens->array[i+3].data, tokens->array[i+1].data));

                        i += 4;
                        continue;
                     }

                else {
                    raise(ErrorType_Syntax, L"Invalid import scheme", L"<raw>", token->x, token->y);
                }
            }

            /* DECLERATION   type identifier = expression; */
            else if ((&(tokens->array[i+1]))->type == TokenType_IDENTIFIER &&
                (&(tokens->array[i+2]))->type == TokenType_OPERATOR   &&
                u8isequal((&(tokens->array[i+2]))->data, L"=")) {
                    
                    DeclType type = get_decltype((&(tokens->array[i]))->data);
                    u8char *var = (&(tokens->array[i+1]))->data;

                    TokenArray *slice = TokenArray_slicet(tokens, i+3);
                    TokenArray_append(slice, Token_new(TokenType_EOF, L""));
                    Node *expr = parse_expr(slice);
                    TokenArray_free(slice);

                    NodeArray_append(node_array, NodeDecl_new(type, var, expr));

                    // end of the expression
                    int a = 0;
                    while (i+a < tokens->used) {
                        if ((&(tokens->array[a+i]))->type == TokenType_NEXTSTM ||
                            (&(tokens->array[a+i]))->type == TokenType_EOF) break;
                        a++;
                    }
                    i += a+1;
                    continue;
                }
            
            /* ASSIGNMENT   identifier = expression; */
            else if ((&(tokens->array[i]))->type == TokenType_IDENTIFIER &&
                     (&(tokens->array[i+1]))->type == TokenType_OPERATOR &&
                      u8isequal((&(tokens->array[i+1]))->data, L"=")) {
                    
                        u8char *var = (&(tokens->array[i]))->data;

                        TokenArray *slice = TokenArray_slicet(tokens, i+2);
                        TokenArray_append(slice, Token_new(TokenType_EOF, L""));
                        Node *expr = parse_expr(slice);
                        TokenArray_free(slice);

                        NodeArray_append(node_array, NodeAssign_new(var, expr));

                        // end of the expression
                        int a = 0;
                        while (i+a < tokens->used) {
                            if ((&(tokens->array[a+i]))->type == TokenType_NEXTSTM ||
                                (&(tokens->array[a+i]))->type == TokenType_EOF) break;
                            a++;
                        }
                        i += a+1;
                        continue;
                }

            /* ENUM   enum {identifier|assignment, ...} */
            else if (u8isequal(token->data, L"enum")) {

                u8char *name;
                if (tokens->array[i+1].type == TokenType_IDENTIFIER) {
                    name = tokens->array[i+1].data;
                }
                else {
                    raise(ErrorType_Syntax, L"Identifier expected after enum", L"<raw>",
                          tokens->array[i+1].x,
                          tokens->array[i+1].y);
                }

                if (!(tokens->array[i+2].type == TokenType_LCURLY)) {
                    raise(ErrorType_Syntax, L"Expected }", L"<raw>",
                          tokens->array[i+2].x,
                          tokens->array[i+2].y);
                }

                TokenArray *slice = TokenArray_slice(tokens, i+3);
                Node *body = parse_enum(slice);
                TokenArray_free(slice);
                i += body->body_tokens+4;

                if (!(tokens->array[i].type == TokenType_NEXTSTM ||
                      tokens->array[i].type == TokenType_EOF)) {

                    raise(ErrorType_Syntax, L"Expected ;", L"<raw>",
                          tokens->array[i+2].x,
                          tokens->array[i+2].y);
                }

                NodeArray_append(node_array, NodeEnum_new(name, body));

                continue;
            }
            
            /* IF   if expression body */
            else if (u8isequal(token->data, L"if")) {

                TokenArray *slice = TokenArray_slicet(tokens, i+1);
                TokenArray_append(slice, Token_new(TokenType_EOF, L""));
                Node *expr = parse_expr(slice);
                TokenArray_free(slice);
                i += _last_token_count;

                TokenArray *slice2 = TokenArray_slice(tokens, i+1);
                Node *body = parse_body(slice2);
                TokenArray_free(slice2);
                i += body->body_tokens+2;

                NodeArray_append(node_array, NodeIf_new(expr, body));

                continue;
            }

            /* ELIF   elif expression body */
            else if (u8isequal(token->data, L"elif")) {

                TokenArray *slice = TokenArray_slicet(tokens, i+1);
                TokenArray_append(slice, Token_new(TokenType_EOF, L""));
                Node *expr = parse_expr(slice);
                TokenArray_free(slice);
                i += _last_token_count;

                TokenArray *slice2 = TokenArray_slice(tokens, i+1);
                Node *body = parse_body(slice2);
                TokenArray_free(slice2);
                i += body->body_tokens+2;

                NodeArray_append(node_array, NodeElif_new(expr, body));

                continue;
            }

            /* ELSE   else body */
            else if (u8isequal(token->data, L"else")) {

                if (tokens->array[i+1].type != TokenType_LCURLY) {
                    raise(ErrorType_Syntax, L"Expected {", L"<raw>", tokens->array[i+1].x, tokens->array[i+1].y);
                }

                TokenArray *slice = TokenArray_slice(tokens, i+1);
                Node *body = parse_body(slice);
                TokenArray_free(slice);
                i += body->body_tokens+2;

                NodeArray_append(node_array, NodeElse_new(body));

                continue;
            }

            /* REPEAT   repeat expression body */
            else if (u8isequal(token->data, L"repeat")) {

                TokenArray *slice = TokenArray_slicet(tokens, i+1);
                TokenArray_append(slice, Token_new(TokenType_EOF, L""));
                Node *expr = parse_expr(slice);
                TokenArray_free(slice);
                i += _last_token_count;

                TokenArray *slice2 = TokenArray_slice(tokens, i+1);
                Node *body = parse_body(slice2);
                TokenArray_free(slice2);
                i += body->body_tokens+2;

                NodeArray_append(node_array, NodeRepeat_new(expr, body));

                continue;
            }

            /* WHILE   while expression body */
            else if (u8isequal(token->data, L"while")) {

                TokenArray *slice = TokenArray_slicet(tokens, i+1);
                TokenArray_append(slice, Token_new(TokenType_EOF, L""));
                Node *expr = parse_expr(slice);
                TokenArray_free(slice);
                i += _last_token_count;

                TokenArray *slice2 = TokenArray_slice(tokens, i+1);
                Node *body = parse_body(slice2);
                TokenArray_free(slice2);
                i += body->body_tokens+2;

                NodeArray_append(node_array, NodeWhile_new(expr, body));

                continue;
            }

            else {  
                TokenArray *slice = TokenArray_slice(tokens, i);
                Node *expr = parse_expr(slice);
                TokenArray_free(slice);

                NodeArray_append(node_array, expr);

                i += _last_token_count;
                continue;
            }
        }

        else {
            TokenArray *slice = TokenArray_slice(tokens, i);
            Node *expr = parse_expr(slice);
            TokenArray_free(slice);

            NodeArray_append(node_array, expr);

            i += _last_token_count;
            continue;
        }

    i++;
    }

    return NodeBody_new(node_array, i);
}


OpType get_optype(u8char *tokenval) {
    if (u8isequal(tokenval, L"+")) {
        return OpType_ADD;
    }
    else if (u8isequal(tokenval, L"-")) {
        return OpType_SUB;
    }
    else if (u8isequal(tokenval, L"*")) {
        return OpType_MUL;
    }
    else if (u8isequal(tokenval, L"/")) {
        return OpType_DIV;
    }
    else if (u8isequal(tokenval, L"^")) {
        return OpType_POW;
    }
    else if (u8isequal(tokenval, L"..")) {
        return OpType_RANGE;
    }
    else if (u8isequal(tokenval, L"and")) {
        return OpType_AND;
    }
    else if (u8isequal(tokenval, L"or")) {
        return OpType_OR;
    }
    else if (u8isequal(tokenval, L"xor")) {
        return OpType_XOR;
    }
    else if (u8isequal(tokenval, L"not")) {
        return OpType_NOT;
    }
    else if (u8isequal(tokenval, L"==")) {
        return OpType_EQ;
    }
    else if (u8isequal(tokenval, L"!=")) {
        return OpType_NEQ;
    }
    else if (u8isequal(tokenval, L"<")) {
        return OpType_LT;
    }
    else if (u8isequal(tokenval, L"<=")) {
        return OpType_LE;
    }
    else if (u8isequal(tokenval, L">")) {
        return OpType_GT;
    }
    else if (u8isequal(tokenval, L">=")) {
        return OpType_GE;
    }
    else if (u8isequal(tokenval, L"has")) {
        return OpType_HAS;
    }
}

DeclType get_decltype(u8char *tokenval) {
    if (u8isequal(tokenval, L"int")) {
        return DeclType_INT32;
    }
    else if (u8isequal(tokenval, L"int8")) {
        return DeclType_INT8;
    }
    else if (u8isequal(tokenval, L"int16")) {
        return DeclType_INT16;
    }
    else if (u8isequal(tokenval, L"int32")) {
        return DeclType_INT32;
    }
    else if (u8isequal(tokenval, L"int64")) {
        return DeclType_INT64;
    }
    else if (u8isequal(tokenval, L"int128")) {
        return DeclType_INT128;
    }
    else if (u8isequal(tokenval, L"uint")) {
        return DeclType_UINT32;
    }
    else if (u8isequal(tokenval, L"uint8")) {
        return DeclType_UINT8;
    }
    else if (u8isequal(tokenval, L"uint16")) {
        return DeclType_UINT16;
    }
    else if (u8isequal(tokenval, L"uint32")) {
        return DeclType_UINT32;
    }
    else if (u8isequal(tokenval, L"uint64")) {
        return DeclType_UINT64;
    }
    else if (u8isequal(tokenval, L"uint128")) {
        return DeclType_UINT128;
    }
    else if (u8isequal(tokenval, L"float")) {
        return DeclType_FLOAT32;
    }
    else if (u8isequal(tokenval, L"float32")) {
        return DeclType_FLOAT32;
    }
    else if (u8isequal(tokenval, L"float64")) {
        return DeclType_FLOAT64;
    }
    else if (u8isequal(tokenval, L"bool")) {
        return DeclType_BOOL;
    }
    else if (u8isequal(tokenval, L"string")) {
        return DeclType_STRING;
    }
    else if (u8isequal(tokenval, L"buffer")) {
        return DeclType_BUFFER;
    }
}

Token *current_token(TokenArray *tokens) {
    return &(tokens->array[_token_index]);
}

void next_token(TokenArray *tokens) {
    _token_index++;
}

char expect_token(TokenArray *tokens, TokenType type) {
    if (tokens->array[_token_index+1].type != type &&
        tokens->array[_token_index+1].type != TokenType_NEXTSTM &&
        tokens->array[_token_index+1].type != TokenType_EOF) {
            return 0;
    }
    else {
        return 1;
    }
}

Node *parse_child(TokenArray *tokens, Node *node) {
    if (current_token(tokens)->type == TokenType_PERIOD) {
        next_token(tokens);

        Node *child = parse_expr_FACTOR(tokens);

        return NodeChild_new(node, child);
    }
    else {
        return node;
    }
}

Node *parse_subscript(TokenArray *tokens, Node *node) {
    if (current_token(tokens)->type == TokenType_LSQRB) {
        next_token(tokens);

        /* Instant close [] */
        if (current_token(tokens)->type == TokenType_RSQRB) {
            raise(ErrorType_Syntax, L"Subscripting with nothing", L"<raw>",
                    current_token(tokens)->x,
                    current_token(tokens)->y);
        }

        Node *expr = parse_expr_EXPR(tokens);

        if (current_token(tokens)->type == TokenType_RSQRB) {
            next_token(tokens);
            return parse_subscript(tokens,
                   parse_call(tokens,
                   parse_child(tokens, NodeSubscript_new(node, expr))));
        }
        else {
            raise(ErrorType_Syntax, L"Expected ]", L"<raw>",
                    current_token(tokens)->x,
                    current_token(tokens)->y);
        }
    }

    return node;
}

Node *parse_call(TokenArray *tokens, Node *node) {
    if (current_token(tokens)->type == TokenType_LPAREN) {
        next_token(tokens);

        /* Instant close () */
        if (current_token(tokens)->type == TokenType_RPAREN) {

            // check if next token is valid
            char next_valid = 0;
            next_valid += expect_token(tokens, TokenType_LPAREN);
            next_valid += expect_token(tokens, TokenType_LSQRB);
            next_valid += expect_token(tokens, TokenType_OPERATOR);
            next_valid += expect_token(tokens, TokenType_PERIOD);
            if (!next_valid) {
                raise(ErrorType_Syntax, u8join(L"Unexpected symbol '", u8join(tokens->array[_token_index+1].data, L"' after function call")), L"<raw>", 0, 0);
            }

            next_token(tokens);
            return parse_call(tokens,
                   parse_subscript(tokens,
                   parse_child(tokens, NodeCall_new(node, NULL))));
        }

        /* Arguments (arg1, arg2, ...) */
        NodeArray *args = NodeArray_new(1);
        
        NodeArray_append(args, parse_expr_EXPR(tokens));

        while (current_token(tokens)->type == TokenType_COMMA) {
            next_token(tokens);
            NodeArray_append(args, parse_expr_EXPR(tokens));
        }

        if (current_token(tokens)->type == TokenType_RPAREN) {
            next_token(tokens);
            return parse_call(tokens,
                    parse_subscript(tokens,
                    parse_child(tokens, NodeCall_new(node, args))));
        }
        else {
            raise(ErrorType_Syntax, L"Expected ;", L"<raw>",
                    current_token(tokens)->x,
                    current_token(tokens)->y);
        }
    }

    return node;
}

Node *parse_expr_FACTOR(TokenArray *tokens) {
    Token *token = current_token(tokens);

    /* Unary operator */
    if (token->type == TokenType_OPERATOR && (
        u8isequal(token->data, L"+") ||
        u8isequal(token->data, L"-") ||
        u8isequal(token->data, L"not"))) {

            next_token(tokens);
            return NodeUnaryOp_new(get_optype(token->data), parse_expr_FACTOR(tokens));
    }

    /* String literal */
    else if (token->type == TokenType_STRING) {
        next_token(tokens);
        
        if (current_token(tokens)->type == TokenType_LSQRB) {
            next_token(tokens);

            /* Instant close [] */
            if (current_token(tokens)->type == TokenType_RSQRB) {
                raise(ErrorType_Syntax, L"Subscripting with nothing", L"<raw>",
                      current_token(tokens)->x,
                      current_token(tokens)->y);
            }

            Node *expr = parse_expr_EXPR(tokens);

            if (current_token(tokens)->type == TokenType_RSQRB) {
                next_token(tokens);
                return parse_subscript(tokens,
                       parse_child(tokens, NodeSubscript_new(NodeString_new(token->data), expr)));
            }
            else {
                raise(ErrorType_Syntax, L"Expected ]", L"<raw>",
                      current_token(tokens)->x,
                      current_token(tokens)->y);
            }
        }
        else {
            return parse_subscript(tokens,
                   parse_child(tokens, NodeString_new(token->data)));
        }
    }

    /* Integer/Float literal */
    else if (token->type == TokenType_NUMERIC) {
        u8char *integer;
        u8char *intdata = current_token(tokens)->data;
        Node *integernode = NodeInteger_new(wcstol(current_token(tokens)->data, &integer, 10));

        next_token(tokens);
        if (current_token(tokens)->type == TokenType_PERIOD) {
            next_token(tokens);
            
            if (current_token(tokens)->type != TokenType_NUMERIC) {
                raise(ErrorType_Syntax, L"Can't subscript integer literal", L"<raw>",
                current_token(tokens)->x,
                current_token(tokens)->y);
            }

            u8char *floating;
            Node *floatnode = NodeFloat_new(wcstod(u8join(intdata, u8join(L".", current_token(tokens)->data)),
                                                   &floating));
            next_token(tokens);
            free(integer);
            free(intdata);
            free(floating);
            return floatnode;
        }
        else {
            free(integer);
            free(intdata);
            return integernode;
        }
    }

    /* Identifier  |  Function/Class call */
    else if (token->type == TokenType_IDENTIFIER) {
        next_token(tokens);

        if (current_token(tokens)->type == TokenType_LPAREN) {
            next_token(tokens);

            /* Instant close () */
            if (current_token(tokens)->type == TokenType_RPAREN) {

                // check if next token is valid
                char next_valid = 0;
                next_valid += expect_token(tokens, TokenType_LPAREN);
                next_valid += expect_token(tokens, TokenType_LSQRB);
                next_valid += expect_token(tokens, TokenType_OPERATOR);
                next_valid += expect_token(tokens, TokenType_PERIOD);
                if (!next_valid) {
                    raise(ErrorType_Syntax, u8join(L"Unexpected symbol '", u8join(tokens->array[_token_index+1].data, L"' after function call")), L"<raw>", 0, 0);
                }

                next_token(tokens);
                return parse_call(tokens,
                       parse_subscript(tokens,
                       parse_child(tokens, NodeCall_new(NodeFuncBase_new(token->data), NULL))));
            }

            /* Arguments (arg1, arg2, ...) */
            NodeArray *args = NodeArray_new(1);
            
            NodeArray_append(args, parse_expr_EXPR(tokens));

            while (current_token(tokens)->type == TokenType_COMMA) {
                next_token(tokens);
                NodeArray_append(args, parse_expr_EXPR(tokens));
            }

            if (current_token(tokens)->type == TokenType_RPAREN) {
                next_token(tokens);
                return parse_call(tokens,
                       parse_subscript(tokens,
                       parse_child(tokens, NodeCall_new(NodeFuncBase_new(token->data), args))));
            }
            else {
                raise(ErrorType_Syntax, L"Expected ;", L"<raw>",
                      current_token(tokens)->x,
                      current_token(tokens)->y);
            }
        }
        else {
            return parse_subscript(tokens,
                   parse_child(tokens, NodeVar_new(token->data)));
        }
    }

    /* ( Expression ) */
    else if (token->type == TokenType_LPAREN) {
        next_token(tokens);

        /* Instant close () */
        if (current_token(tokens)->type == TokenType_RPAREN) {
            raise(ErrorType_Syntax, L"Expression expected between parantheses", L"<raw>", token->x, token->y);
        }

        Node *expr = parse_expr_EXPR(tokens);

        if (current_token(tokens)->type == TokenType_RPAREN) {
            next_token(tokens);
            return parse_subscript(tokens, expr);
        }
        else {
            raise(ErrorType_Syntax, L"Expected )", L"<raw>", current_token(tokens)->x, current_token(tokens)->y);
        }
    }
}

Node *parse_expr_POW(TokenArray *tokens) {
    Node *left = parse_expr_FACTOR(tokens);

    if (current_token(tokens)->type == TokenType_OPERATOR) {
        while (u8isequal(current_token(tokens)->data, L"^")) {

            OpType optype = get_optype(current_token(tokens)->data);
            next_token(tokens);
            left = NodeBinOp_new(optype, left, parse_expr_FACTOR(tokens));
        }
    }

    return left;
}

Node *parse_expr_TERM(TokenArray *tokens) {
    Node *left = parse_expr_POW(tokens);

    if (current_token(tokens)->type == TokenType_OPERATOR) {
        while (u8isequal(current_token(tokens)->data, L"*")  ||
               u8isequal(current_token(tokens)->data, L"/")  ||
               u8isequal(current_token(tokens)->data, L"==") ||
               u8isequal(current_token(tokens)->data, L"!=") ||
               u8isequal(current_token(tokens)->data, L"<")  ||
               u8isequal(current_token(tokens)->data, L"<=") ||
               u8isequal(current_token(tokens)->data, L">")  ||
               u8isequal(current_token(tokens)->data, L">=")) {

                    OpType optype = get_optype(current_token(tokens)->data);
                    next_token(tokens);
                    left = NodeBinOp_new(optype, left, parse_expr_POW(tokens));
                }
    }

    return left;
}

Node *parse_expr_EXPR(TokenArray *tokens) {
    Node *left = parse_expr_TERM(tokens);

    if (current_token(tokens)->type == TokenType_OPERATOR) {
        while (u8isequal(current_token(tokens)->data, L"+")   ||
               u8isequal(current_token(tokens)->data, L"-")   ||
               u8isequal(current_token(tokens)->data, L"..")  ||
               u8isequal(current_token(tokens)->data, L"and") ||
               u8isequal(current_token(tokens)->data, L"or")  ||
               u8isequal(current_token(tokens)->data, L"xor") ||
               u8isequal(current_token(tokens)->data, L"has")) {

                    OpType optype = get_optype(current_token(tokens)->data);
                    next_token(tokens);
                    left = NodeBinOp_new(optype, left, parse_expr_TERM(tokens));
            }
    }

    if (!(current_token(tokens)->type == TokenType_NEXTSTM ||
          current_token(tokens)->type == TokenType_EOF     ||
          current_token(tokens)->type == TokenType_RPAREN  ||
          current_token(tokens)->type == TokenType_LCURLY  ||
          current_token(tokens)->type == TokenType_RCURLY  ||
          current_token(tokens)->type == TokenType_COMMA   ||
          current_token(tokens)->type == TokenType_RSQRB)) {
            raise(ErrorType_Syntax, L"Expected ;", L"<raw>", current_token(tokens)->x, current_token(tokens)->y);
        }

    return left;
}

/*
  Parse an expression and return a Node object

  TokenArray *tokens  ->  TokenArray to parse
*/
Node *parse_expr(TokenArray *tokens) {
    Node *expr = parse_expr_EXPR(tokens);
    _last_token_count = _token_index+1;
    _token_index = 0;
    return expr;
}