/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#include <stdio.h>
#include <stdlib.h>
#include "dust/ustring.h"
#include "dust/error.h"
#include "dust/tokenizer.h"
#include "dust/parser.h"


/**
 * @brief Create a new integer node
 * 
 * @param integer Value
 * @return Node's pointer
 */
Node *NodeInteger_new(long integer) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_INTEGER;
    node->integer = integer;
    return node;
}

/**
 * @brief Create a new float node
 * 
 * @param floating Value
 * @return Node's pointer
 */
Node *NodeFloat_new(double floating) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_FLOAT;
    node->floating = floating;
    return node;
}

/**
 * @brief Create a new string node
 * 
 * @param str Value
 * @return Node's pointer
 */
Node *NodeString_new(u32char *str) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_STRING;
    node->string = str;
    return node;
}

/**
 * @brief Create a new call (function/class) node
 * 
 * @param call_base Call base (node value)
 * @param call_args Argument array (might be NULL)
 * @return Node's pointer
 */
Node *NodeCall_new(Node *call_base, NodeArray *call_args) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_CALL;
    node->call_base = call_base;
    node->call_args = call_args;
    return node;
}

/**
 * @brief Create a new function base node
 * 
 * @param func_base Function base
 * @return Node's pointer
 */
Node *NodeFuncBase_new(u32char *func_base) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_FUNCBASE;
    node->func_base = func_base;
    return node;
}

/**
 * @brief Create a new variable node
 * 
 * @param variable Value
 * @return Node's pointer
 */
Node *NodeVar_new(u32char *variable) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_VAR;
    node->variable = variable;
    return node;
}

/**
 * @brief Create a new decleration node
 * 
 * @param type Type of the variable
 * @param variable Identifier of the variable
 * @param expression Decleration expression
 * @return Node's pointer
 */
Node *NodeDecl_new(Node *type, u32char *variable, Node *expression) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_DECL;
    node->decl_type = type;
    node->decl_var  = variable;
    node->decl_expr = expression;
    return node;
}

/**
 * @brief Create a new decleration (without initializer) node
 * 
 * @param type Type of the variable
 * @param variable Identifier of the variable
 * @return Node's pointer
 */
Node *NodeDecln_new(Node *type, u32char *variable) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_DECLN;
    node->decl_type = type;
    node->decl_var  = variable;
    return node;
}

Node *NodePrimitive_new(u32char *primitive) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_PRIMITIVE;
    node->primitive = primitive;
    return node;
}

/**
 * @brief Create a new assignment node
 * 
 * @param variable Identifier of the variable
 * @param expression Assignment expression
 * @return Node's pointer
 */
Node *NodeAssign_new(u32char *variable, u32char *op, Node *expression) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_ASSIGN;
    node->assign_var = variable;
    node->assign_op = op;
    node->assign_expr = expression;
    return node;
}

/**
 * @brief Create a new binary operator node
 * 
 * @param op Type of the operator
 * @param left Left-hand node
 * @param right Right-hand node
 * @return Node's pointer
 */
Node *NodeBinOp_new(OpType op, Node *left, Node *right) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_BINOP;
    node->bin_optype = op;
    node->bin_left = left;
    node->bin_right = right;
    return node;
}

/**
 * @brief Create a new unary operator node
 * 
 * @param op Type of the operator
 * @param right Right-hand node
 * @return Node's pointer
 */
Node *NodeUnaryOp_new(OpType op, Node *right) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_UNARYOP;
    node->unary_optype = op;
    node->unary_right = right;
    return node;
}

/**
 * @brief Create a new import node
 * 
 * @param module Name of the module
 * @return Node's pointer
 */
Node *NodeImport_new(u32char *module) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_IMPORT;
    node->import_module = module;
    return node;
}

/**
 * @brief Create a new relative import node
 * 
 * @param module Name of the module
 * @param member Member to import from module
 * @return Node's pointer
 */
Node *NodeImportFrom_new(u32char *module, u32char *member) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_IMPORTF;
    node->import_module = module;
    node->import_member = member;
    return node;
}

/**
 * @brief Create a new subscript (indexing) node
 * 
 * @param snode Node that is getting subscripted
 * @param expr Subscripting expression
 * @return Node's pointer
 */
Node *NodeSubscript_new(Node *snode, Node *expr) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_SUBSCRIPT;
    node->subs_node = snode;
    node->subs_expr = expr;
    return node;
}

/**
 * @brief Create a new child (dot notation) node
 * 
 * @param parent Parent node
 * @param child Child node
 * @return Node's pointer
 */
Node *NodeChild_new(Node *parent, Node *child) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_CHILD;
    node->chld_parent = parent;
    node->chld_child  = child;
    return node;
}

/**
 * @brief Create a new enumeration node
 * 
 * @param name Identifier (name) of enumeration
 * @param body Body of enumeration
 * @return Node's pointer
 */
Node *NodeEnum_new(u32char *name, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_ENUM;
    node->enum_name = name;
    node->enum_body = body;
    return node;
}

/**
 * @brief Create a new body node
 * 
 * @param node_array Array of statement nodes
 * @param tokens Number of tokens body contains
 * @return Node's pointer 
 */
Node *NodeBody_new(NodeArray *node_array, int tokens) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_BODY;
    node->body = node_array;
    node->body_tokens = tokens;
    return node;
}

/**
 * @brief Create a new generic type node
 * 
 * @param node_array Array of expression nodes
 * @param tokens Number of tokens generic type contains
 * @return Node's pointer 
 */
Node *NodeGenType_new(NodeArray *node_array, int tokens) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_GENTYPE;
    node->gentype = node_array;
    node->gentype_tokens = tokens;
    return node;
}

/**
 * @brief Create a new if node
 * 
 * @param expression If statement's condition
 * @param body If statement's body
 * @return Node's pointer
 */
Node *NodeIf_new(Node *expression, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_IF;
    node->if_expr = expression;
    node->if_body = body;
    return node;
}

/**
 * @brief Create a new elif (else if) node
 * 
 * @param expression Elif statement's condition
 * @param body Elif statement's body
 * @return Node's pointer
 */
Node *NodeElif_new(Node *expression, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_ELIF;
    node->elif_expr = expression;
    node->elif_body = body;
    return node;
}

/**
 * @brief Create a new else node
 * 
 * @param body Else statement's body
 * @return Node's pointer
 */
Node *NodeElse_new(Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_ELSE;
    node->else_body = body;
    return node;
}

/**
 * @brief Create a new repeat loop node
 * 
 * @param expression Repeat loop's expression (count)
 * @param body Repeat loop's body
 * @return Node's pointer
 */
Node *NodeRepeat_new(Node *expression, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_REPEAT;
    node->repeat_expr = expression;
    node->repeat_body = body;
    return node;
}

/**
 * @brief Create a new while loop node
 * 
 * @param expression While loop's condition
 * @param body While loop's body
 * @return Node's pointer
 */
Node *NodeWhile_new(Node *expression, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_WHILE;
    node->while_expr = expression;
    node->while_body = body;
    return node;
}

/**
 * @brief Create a new for loop node
 * 
 * @param var For loop's variable
 * @param iterator For loop's iterator
 * @param body For loop's body
 * @return Node's pointer
 */
Node *NodeFor_new(Node *var, Node *iterator, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_FOR;
    node->for_var = var;
    node->for_expr = iterator;
    node->for_body = body;
    return node;
}

/**
 * @brief Create a new array node
 * 
 * @param node_array Array's content
 * @param empty Whether the array is empty or not
 * @return Node's pointer
 */
Node *NodeNArray_new(NodeArray *node_array, bool empty) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NodeType_ARRAY;
    node->array_nodearray = node_array;
    node->array_empty = empty;
    return node;
}

//TODO: cover all cases
/**
 * @brief Release all resources used by the node
 * 
 * @param node Node to free
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

/**
 * @brief Represent node as a string
 * 
 * @param node Node to return a repr. string of
 * @param ident Indentation
 * @return Representation string
 */
u32char *Node_repr(Node *node, int ident) {
    u32char *finalstr = U"";

    char numstr[50];
    u32char *identstr = U"";
    identstr = u32fill(identstr, U"  ", (ident+1)*2);

    switch (node->type) {
        case NodeType_INTEGER:
            sprintf(numstr, "%ld", node->integer);
            finalstr = u32join(u32join(finalstr, u32join(U"integer: ", ascii_to_utf32(numstr))), U"\n");
            break;

        case NodeType_FLOAT:
            sprintf(numstr, "%lf", node->floating);
            finalstr = u32join(u32join(finalstr, u32join(U"float: ", ascii_to_utf32(numstr))), U"\n");
            break;

        case NodeType_STRING:
            finalstr = u32join(u32join(finalstr, u32join(U"string: ", node->string)), U"\n");
            break;

        case NodeType_VAR:
            finalstr = u32join(u32join(finalstr, u32join(U"var: ", node->variable)), U"\n");
            break;

        case NodeType_CALL:
            finalstr = u32join(finalstr, U"call:\n");
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->call_base, ident+1)));
            if (node->call_args) {
                finalstr = u32join(finalstr, u32join(identstr, U"args:\n"));
                int i = 0;
                while (i < node->call_args->used) {
                    finalstr = u32join(finalstr, u32join(u32join(identstr, U"    "), Node_repr(&(node->call_args->array[i]), ident+2)));
                    i++;
                }
            }
            else {
                finalstr = u32join(finalstr, u32join(identstr, U"args: no args\n"));
            }
            break;

        case NodeType_FUNCBASE:
            finalstr = u32join(finalstr, u32join(U"function: ", u32join(node->func_base, U"\n")));
            break;

        case NodeType_PRIMITIVE:
            finalstr = u32join(finalstr, u32join(U"primitive: ", u32join(node->primitive, U"\n")));
            break;

        case NodeType_ARRAY:
            finalstr = u32join(finalstr, U"array:\n");
            
            int i = 0;
            while (i < node->array_nodearray->used) {
                finalstr = u32join(finalstr, u32join(identstr, Node_repr(&(node->array_nodearray->array[i]), ident+1)));
                i++;
            }
            break;

        case NodeType_DECL:
            finalstr = u32join(finalstr, U"declaration:\n");

            finalstr = u32join(finalstr, u32join(identstr, u32join(U"type: ", Node_repr(node->decl_type, ident+1))));
            finalstr = u32join(u32join(finalstr, u32join(identstr, u32join(U"var: ", node->decl_var))), U"\n");
            finalstr = u32join(finalstr, u32join(identstr, u32join(U"expr: ", Node_repr(node->decl_expr, ident+1))));
            break;

        case NodeType_DECLN:
            finalstr = u32join(finalstr, U"declaration:\n");

            finalstr = u32join(finalstr, u32join(identstr, u32join(U"type: ", Node_repr(node->decl_type, ident+1))));
            finalstr = u32join(u32join(finalstr, u32join(identstr, u32join(U"var: ", node->decln_var))), U"\n");
            break;

        case NodeType_ASSIGN:
            finalstr = u32join(finalstr, U"assignment:\n");
            finalstr = u32join(u32join(finalstr, u32join(identstr, u32join(U"var: ", node->assign_var))), U"\n");
            finalstr = u32join(u32join(finalstr, u32join(identstr, u32join(U"op: ", node->assign_op))), U"\n");
            finalstr = u32join(finalstr, u32join(identstr, u32join(U"expr: ", Node_repr(node->assign_expr, ident+1))));
            break;

        case NodeType_BINOP:
            finalstr = u32join(finalstr, U"binop:\n");
            
            switch (node->bin_optype) {
                case OpType_ADD:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: +\n"));
                    break;

                case OpType_SUB:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: -\n"));
                    break;

                case OpType_MUL:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: *\n"));
                    break;

                case OpType_DIV:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: /\n"));
                    break;

                case OpType_POW:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: ^\n"));
                    break;

                case OpType_MOD:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: %\n"));
                    break;

                case OpType_RANGE:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: ..\n"));
                    break;

                case OpType_AND:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: and\n"));
                    break;

                case OpType_OR:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: or\n"));
                    break;

                case OpType_XOR:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: xor\n"));
                    break;

                case OpType_EQ:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: ==\n"));
                    break;

                case OpType_NEQ:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: !=\n"));
                    break;

                case OpType_LT:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: <\n"));
                    break;

                case OpType_LE:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: <=\n"));
                    break;

                case OpType_GT:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: >\n"));
                    break;

                case OpType_GE:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: >=\n"));
                    break;

                case OpType_IN:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: in\n"));
                    break;
            }

            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->bin_left, ident+1)));
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->bin_right, ident+1)));
            break;

        case NodeType_UNARYOP:
            finalstr = u32join(finalstr, U"unaryop:\n");
            
            switch (node->unary_optype) {
                case OpType_ADD:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: +\n"));
                    break;

                case OpType_SUB:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: -\n"));
                    break;

                case OpType_NOT:
                    finalstr = u32join(finalstr, u32join(identstr, U"op: not\n"));
                    break;
            }

            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->unary_right, ident+1)));
            break;

        case NodeType_IMPORT:
            finalstr = u32join(finalstr, U"import:\n");
            finalstr = u32join(finalstr, u32join(identstr, u32join(U"module: ", u32join(node->import_module, U"\n"))));
            break;

        case NodeType_IMPORTF:
            finalstr = u32join(finalstr, U"import:\n");
            finalstr = u32join(finalstr, u32join(identstr, u32join(U"member: ", u32join(node->import_member, U"\n"))));
            finalstr = u32join(finalstr, u32join(identstr, U"from:\n"));
            finalstr = u32join(finalstr, u32join(identstr, u32join(U"    ", u32join(U"module: ", u32join(node->import_module, U"\n")))));
            break;

        case NodeType_ENUM:
            finalstr = u32join(finalstr, U"enum:\n");
            finalstr = u32join(finalstr, u32join(identstr, u32join(U"name: ", u32join(node->enum_name, U"\n"))));
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->enum_body, ident+1)));
            break;

        case NodeType_BODY:
            finalstr = u32join(finalstr, U"body:\n");
            
            int t = 0;
            while (t < node->body->used) {
                finalstr = u32join(finalstr, u32join(identstr, Node_repr(&(node->body->array[t]), ident+1)));
                t++;
            }
            break;

        case NodeType_GENTYPE:
            finalstr = u32join(finalstr, U"generic type:\n");
            
            int j = 0;
            while (j < node->gentype->used) {
                finalstr = u32join(finalstr, u32join(identstr, Node_repr(&(node->gentype->array[j]), ident+1)));
                j++;
            }
            break;

        case NodeType_SUBSCRIPT:
            finalstr = u32join(finalstr, U"subscript:\n");
            finalstr = u32join(finalstr, u32join(identstr, u32join(U"node: ", Node_repr(node->subs_node, ident+1))));
            finalstr = u32join(finalstr, u32join(identstr, u32join(U"expr: ", Node_repr(node->subs_expr, ident+1))));
            break;

        case NodeType_CHILD:
            finalstr = u32join(finalstr, U"member:\n");
            finalstr = u32join(finalstr, u32join(identstr, u32join(U"parent: ", Node_repr(node->subs_node, ident+1))));
            finalstr = u32join(finalstr, u32join(identstr, u32join(U"child: ", Node_repr(node->subs_expr, ident+1))));
            break;

        case NodeType_IF:
            finalstr = u32join(finalstr, U"if:\n");
            finalstr = u32join(finalstr, u32join(identstr, U"condition:\n"));
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->if_expr, ident+1)));
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->if_body, ident+1)));
            break;

        case NodeType_ELIF:
            finalstr = u32join(finalstr, U"elif:\n");
            finalstr = u32join(finalstr, u32join(identstr, U"condition:\n"));
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->if_expr, ident+1)));
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->if_body, ident+1)));
            break;

        case NodeType_ELSE:
            finalstr = u32join(finalstr, U"else:\n");
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->if_body, ident+1)));
            break;

        case NodeType_REPEAT:
            finalstr = u32join(finalstr, U"repeat:\n");
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->repeat_expr, ident+1)));
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->repeat_body, ident+1)));
            break;

        case NodeType_WHILE:
            finalstr = u32join(finalstr, U"while:\n");
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->while_expr, ident+1)));
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->while_body, ident+1)));
            break;

        case NodeType_FOR:
            finalstr = u32join(finalstr, U"for:\n");
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->for_var, ident+1)));
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->for_expr, ident+1)));
            finalstr = u32join(finalstr, u32join(identstr, Node_repr(node->for_body, ident+1)));
            break;
        
    }

    return finalstr;
}


/**
 * @brief Create a new node array
 * 
 * @param def_size Initial size of the array
 * @return Node array's pointer
 */
NodeArray *NodeArray_new(size_t def_size) {
    NodeArray *node_array = (NodeArray *)malloc(sizeof(NodeArray));

    node_array->array = malloc(def_size * sizeof(Node));
    node_array->used = 0;
    node_array->size = def_size;

    return node_array;
}

/**
 * @brief Release all resources used by the node array
 * 
 * @param node_array Node array to free
 */
void NodeArray_free(NodeArray *node_array) {
    free(node_array->array);
    node_array->array = NULL;
    node_array->used = 0;
    node_array->size = 0;
    free(node_array);
}

/**
 * @brief Append a node to node array
 * 
 * @param node_array Node array to append to
 * @param node Node to append
 */
void NodeArray_append(NodeArray *node_array, Node *node) {
    if (node_array->used == node_array->size) {
        node_array->size *= 2;
        node_array->array = realloc(node_array->array, node_array->size * sizeof(Node));
    }

    node_array->array[node_array->used++] = *node;
}


size_t _token_index = 0;
size_t _last_token_count = 0;
int _body_count = 0;


/**
 * @brief Parse an enumeration body
 * 
 * @param tokens Token array to parse
 * @return Node's pointer
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
            raise(ErrorType_Syntax, U"Unexpected symbol ; in enumeration", U"<stdin>", token->x, token->y);
        }

        else if (token->type == TokenType_COMMA) {
            if (tokens->array[i-1].type == TokenType_COMMA) {
                raise(ErrorType_Syntax, U"Statement expected before ,", U"<stdin>", token->x, token->y);
            }
            i++;
            continue;
        }

        else if (token->type == TokenType_IDENTIFIER) {

            /* ASSIGNMENT   identifier = expression, */
            if (tokens->array[i].type == TokenType_IDENTIFIER &&
                tokens->array[i+1].type == TokenType_OPERATOR &&
                u32isequal((&(tokens->array[i+1]))->data, U"=")) {
                
                u32char *var = (&(tokens->array[i]))->data;

                TokenArray *slice = TokenArray_slice(tokens, i+2);
                TokenArray_append(slice, Token_new(TokenType_EOF, U""));
                Node *expr = parse_expr(slice);
                TokenArray_free(slice);

                NodeArray_append(node_array, NodeAssign_new(var, U"=", expr));

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
            raise(ErrorType_Syntax, U"Unexpected field in enumeration", U"<stdin>", token->x, token->y);
        }

        i++;
    }

    return NodeBody_new(node_array, i);
}


/**
 * @brief Parse a type genericizator
 * 
 * @param tokens 
 * @return Node* 
 */
Node *parse_generic(TokenArray *tokens) {
    size_t i = 0;
    size_t commas = 0;
    NodeArray *node_array = NodeArray_new(1);

    while (i < tokens->used) {
        Token *token = &(tokens->array[i]);
        
        if (token->type == TokenType_OPERATOR && u32isequal(token->data, U">")) {
            break;
        }
        else if (token->type == TokenType_COMMA) {
            i += 1;
            continue;
        }
        else if (token->type != TokenType_IDENTIFIER) {
            raise(ErrorType_Syntax, U"Expected type or >", U"<stdin>", token->x, token->y);
        }

        TokenArray *slice = TokenArray_slice(tokens, i);
        _last_token_count = _token_index+1;
        _token_index = 0;
        Node *factor = parse_expr_FACTOR(slice);
        if (factor->type == NodeType_VAR) {
            u32char *data = factor->variable;
            Node_free(factor);
            factor = NodePrimitive_new(data);
        }
        NodeArray_append(node_array, factor);
        TokenArray_free(slice);

        i += _token_index;
    }
    i += 1;

    return NodeGenType_new(node_array, i);
}


/**
 * @brief Parse a body
 * 
 * @param tokens Token array to parse
 * @return Node's pointer
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
                raise(ErrorType_Syntax, U"Unexpected }", U"<stdin>", token->x, token->y);
            }

            _body_count--;
            break;
        }

        else if (token->type == TokenType_EOF) {
            break;
        }

        else if (token->type == TokenType_NEXTSTM) {
            if (tokens->array[i-1].type == TokenType_NEXTSTM) {
                raise(ErrorType_Syntax, U"Statement expected before ;", U"<stdin>", token->x, token->y);
            }
            i++;
            continue;
        }

        else if (token->type == TokenType_IDENTIFIER) {

            if (u32isequal(token->data, U"import")) {

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
                         u32isequal(tokens->array[i+2].data, U"from")     &&
                         tokens->array[i+3].type == TokenType_IDENTIFIER &&
                         (tokens->array[i+4].type == TokenType_NEXTSTM   ||
                          tokens->array[i+4].type == TokenType_EOF)) {

                        NodeArray_append(node_array, NodeImportFrom_new(tokens->array[i+3].data, tokens->array[i+1].data));

                        i += 4;
                        continue;
                     }

                else {
                    raise(ErrorType_Syntax, U"Invalid import scheme", U"<stdin>", token->x, token->y);
                }
            }

            /* DECLERATION (NO INIT.)   type identifier; */
            else if (tokens->array[i+1].type == TokenType_IDENTIFIER &&
                     (tokens->array[i+2].type == TokenType_NEXTSTM ||
                      tokens->array[i+2].type == TokenType_EOF)) {

                Node *primitive = NodePrimitive_new(tokens->array[i].data);
                u32char *var = (&(tokens->array[i+1]))->data;

                NodeArray_append(node_array, NodeDecln_new(primitive, var));
                i += 3;
                continue;
            }

            /* DECLERATION   type identifier = expression; */
            else if ((&(tokens->array[i+1]))->type == TokenType_IDENTIFIER &&
                    (&(tokens->array[i+2]))->type == TokenType_OPERATOR   &&
                    u32isequal((&(tokens->array[i+2]))->data, U"=")) {
                    
                Node *primitive = NodePrimitive_new(tokens->array[i].data);
                u32char *var = (&(tokens->array[i+1]))->data;

                TokenArray *slice = TokenArray_slicet(tokens, i+3);
                TokenArray_append(slice, Token_new(TokenType_EOF, U""));
                Node *expr = parse_expr(slice);
                TokenArray_free(slice);

                NodeArray_append(node_array, NodeDecl_new(primitive, var, expr));

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

            /* GENERIC DECLERATION   type<type, ...> identifier[ = expression]; */
            else if(tokens->array[i+1].type == TokenType_OPERATOR &&
                    u32isequal(tokens->array[i+1].data, U"<")) {

                TokenArray *slice = TokenArray_slicet(tokens, i+2);
                TokenArray_append(slice, Token_new(TokenType_EOF, U""));
                Node *generic = parse_generic(slice);
                TokenArray_free(slice);

                i += generic->gentype_tokens+2;

                u32char *var = tokens->array[i].data;

                if (tokens->array[i].type == TokenType_IDENTIFIER) {

                    if (tokens->array[i+1].type == TokenType_NEXTSTM ||
                        tokens->array[i+1].type == TokenType_EOF) {


                        NodeArray_append(node_array, NodeDecln_new(generic, var));
                        i += 2;
                        continue;
                    }

                    else if (tokens->array[i+1].type == TokenType_OPERATOR &&
                            u32isequal(tokens->array[i+1].data, U"=")) {

                        TokenArray *sliceb = TokenArray_slicet(tokens, i+2);
                        TokenArray_append(sliceb, Token_new(TokenType_EOF, U""));
                        Node *exprz = parse_expr(sliceb);
                        TokenArray_free(sliceb);

                        NodeArray_append(node_array, NodeDecl_new(generic, var, exprz));

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
                    else {
                        raise(ErrorType_Syntax, U"Expected either = or ; after identifier", U"<stdin>",
                        tokens->array[i+1].x,
                        tokens->array[i+1].y);
                    }

                }
                else {
                    raise(ErrorType_Syntax, U"Identifier expected", U"<stdin>",
                          tokens->array[i].x,
                          tokens->array[i].y);
                }

                continue;

            }
            
            /* ASSIGNMENT   identifier = expression; */
            else if (tokens->array[i].type == TokenType_IDENTIFIER &&
                     tokens->array[i+1].type == TokenType_OPERATOR) {
                    
                    u32char *var = (&(tokens->array[i]))->data;

                    TokenArray *slice = TokenArray_slicet(tokens, i+2);
                    TokenArray_append(slice, Token_new(TokenType_EOF, U""));
                    Node *expr = parse_expr(slice);
                    TokenArray_free(slice);

                    u32char *op;
                    if (u32isequal(tokens->array[i+1].data, U"=")) {
                        op = U"=";
                    }
                    else if (u32isequal(tokens->array[i+1].data, U"+=")) {
                        op = U"+=";
                    }
                    else if (u32isequal(tokens->array[i+1].data, U"-=")) {
                        op = U"-=";
                    }
                    else if (u32isequal(tokens->array[i+1].data, U"*=")) {
                        op = U"*=";
                    }
                    else if (u32isequal(tokens->array[i+1].data, U"/=")) {
                        op = U"/=";
                    }
                    else if (u32isequal(tokens->array[i+1].data, U"^=")) {
                        op = U"^=";
                    }
                    else if (u32isequal(tokens->array[i+1].data, U"%=")) {
                        op = U"%=";
                    }
                    else {
                        raise(ErrorType_Syntax, U"Invalid assignment operator", U"<stdin>",
                              tokens->array[i+1].x, tokens->array[i+1].y);
                    }

                    NodeArray_append(node_array, NodeAssign_new(var, op, expr));

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
            else if (u32isequal(token->data, U"enum")) {

                u32char *name;
                if (tokens->array[i+1].type == TokenType_IDENTIFIER) {
                    name = tokens->array[i+1].data;
                }
                else {
                    raise(ErrorType_Syntax, U"Identifier expected after enum", U"<stdin>",
                          tokens->array[i+1].x,
                          tokens->array[i+1].y);
                }

                if (!(tokens->array[i+2].type == TokenType_LCURLY)) {
                    raise(ErrorType_Syntax, U"Expected }", U"<stdin>",
                          tokens->array[i+2].x,
                          tokens->array[i+2].y);
                }

                TokenArray *slice = TokenArray_slice(tokens, i+3);
                Node *body = parse_enum(slice);
                TokenArray_free(slice);
                i += body->body_tokens+4;

                if (!(tokens->array[i].type == TokenType_NEXTSTM ||
                      tokens->array[i].type == TokenType_EOF)) {

                    raise(ErrorType_Syntax, U"Expected ;", U"<stdin>",
                          tokens->array[i+2].x,
                          tokens->array[i+2].y);
                }

                NodeArray_append(node_array, NodeEnum_new(name, body));

                continue;
            }
            
            /* IF   if expression body */
            else if (u32isequal(token->data, U"if")) {

                TokenArray *slice = TokenArray_slicet(tokens, i+1);
                TokenArray_append(slice, Token_new(TokenType_EOF, U""));
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
            else if (u32isequal(token->data, U"elif")) {

                TokenArray *slice = TokenArray_slicet(tokens, i+1);
                TokenArray_append(slice, Token_new(TokenType_EOF, U""));
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
            else if (u32isequal(token->data, U"else")) {

                if (tokens->array[i+1].type != TokenType_LCURLY) {
                    raise(ErrorType_Syntax, U"Expected {", U"<stdin>",
                          tokens->array[i+1].x, tokens->array[i+1].y);
                }

                TokenArray *slice = TokenArray_slice(tokens, i+1);
                Node *body = parse_body(slice);
                TokenArray_free(slice);
                i += body->body_tokens+2;

                NodeArray_append(node_array, NodeElse_new(body));

                continue;
            }

            /* REPEAT   repeat expression body */
            else if (u32isequal(token->data, U"repeat")) {

                TokenArray *slice = TokenArray_slicet(tokens, i+1);
                TokenArray_append(slice, Token_new(TokenType_EOF, U""));
                Node *expr = parse_expr(slice);
                TokenArray_free(slice);
                i += _last_token_count;

                if (tokens->array[i].type != TokenType_LCURLY) {
                    raise(ErrorType_Syntax, U"Expected {", U"<stdin>",
                          tokens->array[i].x, tokens->array[i].y);
                }

                TokenArray *slice2 = TokenArray_slice(tokens, i+1);
                _body_count++;
                Node *body = parse_body(slice2);
                TokenArray_free(slice2);
                i += body->body_tokens+2;

                NodeArray_append(node_array, NodeRepeat_new(expr, body));

                continue;
            }

            /* WHILE   while expression body */
            else if (u32isequal(token->data, U"while")) {

                TokenArray *slice = TokenArray_slicet(tokens, i+1);
                TokenArray_append(slice, Token_new(TokenType_EOF, U""));
                Node *expr = parse_expr(slice);
                TokenArray_free(slice);
                i += _last_token_count;

                if (tokens->array[i].type != TokenType_LCURLY) {
                    raise(ErrorType_Syntax, U"Expected {", U"<stdin>",
                          tokens->array[i].x, tokens->array[i].y);
                }

                TokenArray *slice2 = TokenArray_slice(tokens, i+1);
                _body_count++;
                Node *body = parse_body(slice2);
                TokenArray_free(slice2);
                i += body->body_tokens+2;

                NodeArray_append(node_array, NodeWhile_new(expr, body));

                continue;
            }

            /* FOR   for identifier in iterable body */
            else if (u32isequal(token->data, U"for")) {

                if (tokens->array[i+1].type == TokenType_IDENTIFIER) {
                    if (tokens->array[i+2].type == TokenType_OPERATOR &&
                            u32isequal(tokens->array[i+2].data, U"in")) {

                        Node *var = NodeVar_new(tokens->array[i+1].data);
                        
                        TokenArray *slice = TokenArray_slicet(tokens, i+3);
                        TokenArray_append(slice, Token_new(TokenType_EOF, U""));
                        Node *expr = parse_expr(slice);
                        TokenArray_free(slice);
                        i += _last_token_count+2;

                        if (tokens->array[i].type != TokenType_LCURLY) {
                            raise(ErrorType_Syntax, U"Expected {", U"<stdin>",
                                tokens->array[i].x, tokens->array[i].y);
                        }

                        TokenArray *slice2 = TokenArray_slice(tokens, i+1);
                        _body_count++;
                        Node *body = parse_body(slice2);
                        TokenArray_free(slice2);
                        i += body->body_tokens+1;

                        NodeArray_append(node_array, NodeFor_new(var, expr, body));
                    }
                    else {
                        raise(ErrorType_Syntax, U"Missing in keyword", U"<stdin>", token->x, token->y);
                    }
                }
                else {
                    raise(ErrorType_Syntax, U"Non-identifier after for", U"<stdin>", token->x, token->y);
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


OpType get_optype(u32char *tokenval) {
    if (u32isequal(tokenval, U"+")) {
        return OpType_ADD;
    }
    else if (u32isequal(tokenval, U"-")) {
        return OpType_SUB;
    }
    else if (u32isequal(tokenval, U"*")) {
        return OpType_MUL;
    }
    else if (u32isequal(tokenval, U"/")) {
        return OpType_DIV;
    }
    else if (u32isequal(tokenval, U"^")) {
        return OpType_POW;
    }
    else if (u32isequal(tokenval, U"%")) {
        return OpType_MOD;
    }
    else if (u32isequal(tokenval, U"..")) {
        return OpType_RANGE;
    }
    else if (u32isequal(tokenval, U"and")) {
        return OpType_AND;
    }
    else if (u32isequal(tokenval, U"or")) {
        return OpType_OR;
    }
    else if (u32isequal(tokenval, U"xor")) {
        return OpType_XOR;
    }
    else if (u32isequal(tokenval, U"not")) {
        return OpType_NOT;
    }
    else if (u32isequal(tokenval, U"==")) {
        return OpType_EQ;
    }
    else if (u32isequal(tokenval, U"!=")) {
        return OpType_NEQ;
    }
    else if (u32isequal(tokenval, U"<")) {
        return OpType_LT;
    }
    else if (u32isequal(tokenval, U"<=")) {
        return OpType_LE;
    }
    else if (u32isequal(tokenval, U">")) {
        return OpType_GT;
    }
    else if (u32isequal(tokenval, U">=")) {
        return OpType_GE;
    }
    else if (u32isequal(tokenval, U"in")) {
        return OpType_IN;
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
            raise(ErrorType_Syntax, U"Subscripting with nothing", U"<stdin>",
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
            raise(ErrorType_Syntax, U"Expected ]", U"<stdin>",
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
                raise(ErrorType_Syntax, u32join(U"Unexpected symbol '", u32join(tokens->array[_token_index+1].data, U"' after function call")), U"<stdin>", 0, 0);
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
            raise(ErrorType_Syntax, U"Expected ;", U"<stdin>",
                    current_token(tokens)->x,
                    current_token(tokens)->y);
        }
    }

    return node;
}

/**
 * @brief Parse an expression
 * 
 * @param tokens Token array to parse
 * @return Node's pointer
 */
Node *parse_expr(TokenArray *tokens) {
    _token_index = 0;
    Node *expr = parse_expr_EXPR(tokens);
    _last_token_count = _token_index+1;
    _token_index = 0;
    return expr;
}

Node *parse_expr_FACTOR(TokenArray *tokens) {
    Token *token = current_token(tokens);

    /* Unary operator */
    if (token->type == TokenType_OPERATOR && (
        u32isequal(token->data, U"+") ||
        u32isequal(token->data, U"-") ||
        u32isequal(token->data, U"not"))) {

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
                raise(ErrorType_Syntax, U"Subscripting with nothing", U"<stdin>",
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
                raise(ErrorType_Syntax, U"Expected ]", U"<stdin>",
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
        u32char *intdata = current_token(tokens)->data;
        Node *integernode = NodeInteger_new(u32toint(current_token(tokens)->data, 10));

        next_token(tokens);
        if (current_token(tokens)->type == TokenType_PERIOD) {
            next_token(tokens);
            
            if (current_token(tokens)->type != TokenType_NUMERIC) {
                raise(ErrorType_Syntax, U"Can't subscript integer literal", U"<stdin>",
                current_token(tokens)->x,
                current_token(tokens)->y);
            }

            Node *floatnode = NodeFloat_new(u32tofloat(u32join(intdata, u32join(U".", current_token(tokens)->data))));
            next_token(tokens);
            return floatnode;
        }
        else {
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
                    raise(ErrorType_Syntax, u32join(U"Unexpected symbol '", u32join(tokens->array[_token_index+1].data, U"' after function calU")), U"<stdin>", 0, 0);
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
                raise(ErrorType_Syntax, U"Expected ;", U"<stdin>",
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
            raise(ErrorType_Syntax, U"Expression expected between parantheses", U"<stdin>", token->x, token->y);
        }

        Node *expr = parse_expr_EXPR(tokens);

        if (current_token(tokens)->type == TokenType_RPAREN) {
            next_token(tokens);
            return parse_subscript(tokens, expr);
        }
        else {
            raise(ErrorType_Syntax, U"Expected )", U"<stdin>", current_token(tokens)->x, current_token(tokens)->y);
        }
    }

    /* Array Initialization [ Expression, ... ] */
    else if (token->type == TokenType_LSQRB) {
        next_token(tokens);

        /* Instant close () */
        if (current_token(tokens)->type == TokenType_RPAREN) {
            raise(ErrorType_Syntax, U"Expression expected between square parantheses", U"<stdin>", token->x, token->y);
        }

        /* Expressions [expr1, expr2, ...] */
        NodeArray *content = NodeArray_new(1);
        
        NodeArray_append(content, parse_expr_EXPR(tokens));

        while (current_token(tokens)->type == TokenType_COMMA) {
            next_token(tokens);
            NodeArray_append(content, parse_expr_EXPR(tokens));
        }

        if (current_token(tokens)->type == TokenType_RSQRB) {
            next_token(tokens);
            return parse_subscript(tokens,
                   parse_child(tokens, NodeNArray_new(content, false)));
        }
        else {
            raise(ErrorType_Syntax, U"Expected ;", U"<stdin>",
                    current_token(tokens)->x,
                    current_token(tokens)->y);
        }
    }
}

Node *parse_expr_POW(TokenArray *tokens) {
    Node *left = parse_expr_FACTOR(tokens);

    if (current_token(tokens)->type == TokenType_OPERATOR) {
        while (u32isequal(current_token(tokens)->data, U"^") ||
               u32isequal(current_token(tokens)->data, U"%")) {

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
        while (u32isequal(current_token(tokens)->data, U"*")  ||
               u32isequal(current_token(tokens)->data, U"/")  ||
               u32isequal(current_token(tokens)->data, U"==") ||
               u32isequal(current_token(tokens)->data, U"!=") ||
               u32isequal(current_token(tokens)->data, U"<")  ||
               u32isequal(current_token(tokens)->data, U"<=") ||
               u32isequal(current_token(tokens)->data, U">")  ||
               u32isequal(current_token(tokens)->data, U">=")) {

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
        while (u32isequal(current_token(tokens)->data, U"+")   ||
               u32isequal(current_token(tokens)->data, U"-")   ||
               u32isequal(current_token(tokens)->data, U"..")  ||
               u32isequal(current_token(tokens)->data, U"and") ||
               u32isequal(current_token(tokens)->data, U"or")  ||
               u32isequal(current_token(tokens)->data, U"xor") ||
               u32isequal(current_token(tokens)->data, U"in")) {

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

            printf("failed tok: %s\n", utf32_to_utf8(Token_repr(current_token(tokens))));
        //if (!(current_token(tokens)->type == TokenType_OPERATOR && u32isequal(current_token(tokens)->data, U">"))) {
            raise(ErrorType_Syntax, U"Expected ;", U"<stdin>", current_token(tokens)->x, current_token(tokens)->y);
        //}
    }

    return left;
}