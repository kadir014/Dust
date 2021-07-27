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

        /* FUNCTION/CLASS CALL */
        u8char *call_base;

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
*/
Node *NodeCall_new(u8char *call_base) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_CALL;
    node->call_base = call_base;
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
            swprintf(numstr, 50, L"%d", node->integer);
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
            finalstr = u8join(finalstr, u8join(identstr, u8join(L"base: ", u8join(node->call_base, L"\n"))));
            finalstr = u8join(finalstr, u8join(identstr, L"args:\n"));
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
    }

    return finalstr;
}


size_t _token_index = 0;

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
}

Token *current_token(TokenArray *tokens) {
    return &(tokens->array[_token_index]);
}

void next_token(TokenArray *tokens) {
    _token_index++;
}

byte expect_token(TokenArray *tokens, TokenType type) {
    if (tokens->array[_token_index+1].type != type &&
        tokens->array[_token_index+1].type != TokenType_NEXTSTM &&
        tokens->array[_token_index+1].type != TokenType_EOF) {
            return 0;
    }
    else {
        return 1;
    }
}

Node *parse_expr_EXPR(TokenArray *tokens);

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
        Node *returnnode = NodeString_new(current_token(tokens)->data);
        next_token(tokens);
        return returnnode;
    }

    /* Numeric literal */
    else if (token->type == TokenType_NUMERIC) {
        u8char *end;
        //Node *returnnode = NodeNumeric_new(wcstod(current_token(tokens)->data, &end));
        Node *returnnode = NodeInteger_new(wcstol(current_token(tokens)->data, &end, 10));
        next_token(tokens);
        return returnnode;
    }

    /* Identifier  |  Function/Class call */
    else if (token->type == TokenType_IDENTIFIER) {
        next_token(tokens);

        // call
        if (current_token(tokens)->type == TokenType_LPAREN) {
            next_token(tokens);

            /* Instant close () */
            if (current_token(tokens)->type == TokenType_RPAREN) {

                // check if next token is valid
                byte next_valid = 0;
                next_valid += expect_token(tokens, TokenType_LPAREN);
                next_valid += expect_token(tokens, TokenType_LSQRB);
                next_valid += expect_token(tokens, TokenType_OPERATOR);
                if (!next_valid) {
                    raise(ErrorType_Syntax, u8join(L"Unexpected symbol '", u8join(tokens->array[_token_index+1].data, L"' after function call")), L"<raw>", 0, 0);
                }

                next_token(tokens);
                return NodeCall_new(token->data);
            }

            // TODO: Check arguments
        }
        // identifier
        else {
            return NodeVar_new(token->data);
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
            return expr;
        }
        else {
            raise(ErrorType_Syntax, L"Closing paranthesis expected", L"<raw>", current_token(tokens)->x, current_token(tokens)->y);
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
               u8isequal(current_token(tokens)->data, L"xor")) {

                    OpType optype = get_optype(current_token(tokens)->data);
                    next_token(tokens);
                    left = NodeBinOp_new(optype, left, parse_expr_TERM(tokens));
            }
    }

    return left;
}

/*
  Parse an expression and return a Node object

  TokenArray *tokens  ->  TokenArray to parse
*/
Node *parse_expr(TokenArray *tokens) {
    Node *expr = parse_expr_EXPR(tokens);
    _token_index = 0;
    return expr;
}