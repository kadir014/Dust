/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust


  WARNING: Use `tests_run.py` to run tests
  
*/

#include <stdio.h>
#include <stdlib.h>
#include "dust/ansi.h"
#include "dust/u8string.h"
#include "dust/platform.h"
#include "dust/tokenizer.h"
#include "dust/parser.h"


u8char *expect_string(u8char *value, u8char *expect) {
    if (u8isequal(value, expect)) {
        return L"OK";
    }
    else {
        return u8join(L"Expected '", u8join(expect, u8join(L"' but got '", u8join(value, L"'\n"))));
    }
}

u8char *expect_int(int value, int expect) {
    if (value == expect) {
        return L"OK";
    }
    else {
        u8char *ws = malloc(sizeof(u8char)*100);
        swprintf(ws, 100, L"Expected '%d' but got '%d'", expect, value);
        return ws;
    }
}

u8char *expect_float(double value, double expect) {
    if (value == expect) {
        return L"OK";
    }
    else {
        u8char *ws = malloc(sizeof(u8char)*100);
        swprintf(ws, 100, L"Expected '%f' but got '%f'", expect, value);
        return ws;
    }
}

u8char *expect_true(int value) {
    return expect_int(value, 1);
}


int _TESTS   = 0;
int _FAILS   = 0;
int _IGNORES = 0;

u8char *test_tokenizer__Token_new() {
    _TESTS++;

    Token *token1 = Token_new(TokenType_IDENTIFIER, L"hello");
    Token *token2 = Token_new(TokenType_OPERATOR, L"+");
    Token *token3 = Token_new(TokenType_NEXTSTM, L"");

    u8char *test1 = expect_string(token1->data, L"hello");
    u8char *test2 = expect_int   (token1->type, TokenType_IDENTIFIER);
    u8char *test3 = expect_string(token2->data, L"+");
    u8char *test4 = expect_int   (token2->type, TokenType_OPERATOR);
    u8char *test5 = expect_string(token3->data, L"");
    u8char *test6 = expect_int   (token3->type, TokenType_NEXTSTM);

    Token_free(token1);
    Token_free(token2);
    Token_free(token3);

    if      (!u8isequal(test1, L"OK")) {_FAILS++; return test1;}
    else if (!u8isequal(test2, L"OK")) {_FAILS++; return test2;}
    else if (!u8isequal(test3, L"OK")) {_FAILS++; return test3;}
    else if (!u8isequal(test4, L"OK")) {_FAILS++; return test4;}
    else if (!u8isequal(test5, L"OK")) {_FAILS++; return test5;}
    else if (!u8isequal(test6, L"OK")) {_FAILS++; return test6;}
    else                               {return L"OK";}
}

u8char *test_tokenizer__Token_repr() {
    _TESTS++;

    Token *token1 = Token_new(TokenType_IDENTIFIER, L"hello");
    Token *token2 = Token_new(TokenType_OPERATOR, L"+");
    Token *token3 = Token_new(TokenType_NEXTSTM, L"");

    u8char *test1 = expect_string(Token_repr(token1), L"TokenType_IDENTIFIER   hello");
    u8char *test2 = expect_string(Token_repr(token2), L"TokenType_OPERATOR     +");
    u8char *test3 = expect_string(Token_repr(token3), L"TokenType_NEXTSTM      ");

    Token_free(token1);
    Token_free(token2);
    Token_free(token3);

    if      (!u8isequal(test1, L"OK")) {_FAILS++; return test1;}
    else if (!u8isequal(test2, L"OK")) {_FAILS++; return test2;}
    else if (!u8isequal(test3, L"OK")) {_FAILS++; return test3;}
    else                               {return L"OK";}
}

u8char *test_tokenizer__TokenArray_new() {
    _TESTS++;

    TokenArray *token_array = TokenArray_new(1);

    TokenArray_append(token_array, Token_new(TokenType_IDENTIFIER, L"hello"));
    TokenArray_append(token_array, Token_new(TokenType_OPERATOR, L"+"));
    TokenArray_append(token_array, Token_new(TokenType_NEXTSTM, L""));

    u8char *test1 = expect_string(token_array->array[0].data, L"hello");
    u8char *test2 = expect_int   (token_array->array[0].type, TokenType_IDENTIFIER);
    u8char *test3 = expect_string(token_array->array[1].data, L"+");
    u8char *test4 = expect_int   (token_array->array[1].type, TokenType_OPERATOR);
    u8char *test5 = expect_string(token_array->array[2].data, L"");
    u8char *test6 = expect_int   (token_array->array[2].type, TokenType_NEXTSTM);
    u8char *test7 = expect_int   (token_array->used, 3);

    TokenArray_free(token_array);

    if      (!u8isequal(test1, L"OK")) {_FAILS++; return test1;}
    else if (!u8isequal(test2, L"OK")) {_FAILS++; return test2;}
    else if (!u8isequal(test3, L"OK")) {_FAILS++; return test3;}
    else if (!u8isequal(test4, L"OK")) {_FAILS++; return test4;}
    else if (!u8isequal(test5, L"OK")) {_FAILS++; return test5;}
    else if (!u8isequal(test6, L"OK")) {_FAILS++; return test6;}
    else if (!u8isequal(test7, L"OK")) {_FAILS++; return test7;}
    else                               {return L"OK";}
}

u8char *test_tokenizer__TokenArray_append() {
    _TESTS++;

    TokenArray *token_array = TokenArray_new(1);

    u8char *test1 = expect_int   (token_array->used, 0);

    TokenArray_append(token_array, Token_new(TokenType_NEXTSTM, L""));
    u8char *test2 = expect_int   (token_array->used, 1);

    TokenArray_append(token_array, Token_new(TokenType_NEXTSTM, L""));
    u8char *test3 = expect_int   (token_array->used, 2);

    TokenArray_append(token_array, Token_new(TokenType_NEXTSTM, L""));
    TokenArray_append(token_array, Token_new(TokenType_NEXTSTM, L""));
    TokenArray_append(token_array, Token_new(TokenType_NEXTSTM, L""));
    u8char *test4 = expect_int   (token_array->used, 5);

    TokenArray_free(token_array);

    if      (!u8isequal(test1, L"OK")) {_FAILS++; return test1;}
    else if (!u8isequal(test2, L"OK")) {_FAILS++; return test2;}
    else if (!u8isequal(test3, L"OK")) {_FAILS++; return test3;}
    else if (!u8isequal(test4, L"OK")) {_FAILS++; return test4;}
    else                               {return L"OK";}
}

/*
  TODO:
  - test NodeEnum_new
  - test NodeBody_new
  - test NodeIf_new
  - test NodeElif_new
  - test NodeElse_new
  - test NodeRepeat_new
  - test NodeWhile_new
  - test NodeFor_new
*/
u8char *test_parser__Node_new() {
    _TESTS++;

    Node *node1  = NodeInteger_new(5);
    Node *node2  = NodeFloat_new(3.2);
    Node *node3  = NodeString_new(L"hello");
    Node *node4  = NodeVar_new(L"somevar");
    Node *node5  = NodeDecl_new(DeclType_INT32, L"intvar", NodeInteger_new(2));
    Node *node6  = NodeAssign_new(L"floatvar", NodeFloat_new(1.7));
    Node *node7  = NodeBinOp_new(OpType_ADD, node1, node2);
    Node *node8  = NodeUnaryOp_new(OpType_SUB, node3);
    Node *node9  = NodeImport_new(L"module");
    Node *node10 = NodeImportFrom_new(L"module", L"member");
    Node *node11 = NodeSubscript_new(node3, node1);
    Node *node12 = NodeChild_new(node1, node2);

    u8char *test1  = expect_int   (node1->integer, 5);
    u8char *test2  = expect_int   (node1->type, NodeType_INTEGER);
    u8char *test3  = expect_float (node2->floating, 3.2);
    u8char *test4  = expect_int   (node2->type, NodeType_FLOAT);
    u8char *test5  = expect_string(node3->string, L"hello");
    u8char *test6  = expect_int   (node3->type, NodeType_STRING);
    u8char *test7  = expect_string(node4->variable, L"somevar");
    u8char *test8  = expect_int   (node4->type, NodeType_VAR);
    u8char *test9  = expect_string(node5->decl_var, L"intvar");
    u8char *test10 = expect_int   (node5->decl_expr->integer, 2);
    u8char *test11 = expect_int   (node5->type, NodeType_DECL);
    u8char *test12 = expect_string(node6->assign_var, L"floatvar");
    u8char *test13 = expect_float (node6->assign_expr->floating, 1.7);
    u8char *test14 = expect_int   (node6->type, NodeType_ASSIGN);
    u8char *test15 = expect_int   (node7->bin_optype, OpType_ADD);
    u8char *test16 = expect_int   (node7->type, NodeType_BINOP);
    u8char *test17 = expect_int   (node8->unary_optype, OpType_SUB);
    u8char *test18 = expect_int   (node8->type, NodeType_UNARYOP);
    u8char *test19 = expect_string(node9->import_module, L"module");
    u8char *test20 = expect_int   (node9->type, NodeType_IMPORT);
    u8char *test21 = expect_string(node10->import_module, L"module");
    u8char *test22 = expect_string(node10->import_member, L"member");
    u8char *test23 = expect_int   (node10->type, NodeType_IMPORTF);

    Node_free(node1);
    Node_free(node2);
    Node_free(node3);
    Node_free(node4);
    Node_free(node5);
    Node_free(node6);
    Node_free(node7);
    Node_free(node8);
    Node_free(node9);
    Node_free(node10);
    Node_free(node11);
    Node_free(node12);

    if      (!u8isequal(test1, L"OK"))  {_FAILS++; return test1;}
    else if (!u8isequal(test2, L"OK"))  {_FAILS++; return test2;}
    else if (!u8isequal(test3, L"OK"))  {_FAILS++; return test3;}
    else if (!u8isequal(test4, L"OK"))  {_FAILS++; return test4;}
    else if (!u8isequal(test5, L"OK"))  {_FAILS++; return test5;}
    else if (!u8isequal(test6, L"OK"))  {_FAILS++; return test6;}
    else if (!u8isequal(test7, L"OK"))  {_FAILS++; return test7;}
    else if (!u8isequal(test8, L"OK"))  {_FAILS++; return test8;}
    else if (!u8isequal(test9, L"OK"))  {_FAILS++; return test9;}
    else if (!u8isequal(test10, L"OK")) {_FAILS++; return test10;}
    else if (!u8isequal(test11, L"OK")) {_FAILS++; return test11;}
    else if (!u8isequal(test12, L"OK")) {_FAILS++; return test12;}
    else if (!u8isequal(test13, L"OK")) {_FAILS++; return test13;}
    else if (!u8isequal(test14, L"OK")) {_FAILS++; return test14;}
    else if (!u8isequal(test15, L"OK")) {_FAILS++; return test15;}
    else if (!u8isequal(test16, L"OK")) {_FAILS++; return test16;}
    else if (!u8isequal(test17, L"OK")) {_FAILS++; return test17;}
    else if (!u8isequal(test18, L"OK")) {_FAILS++; return test18;}
    else if (!u8isequal(test19, L"OK")) {_FAILS++; return test19;}
    else if (!u8isequal(test20, L"OK")) {_FAILS++; return test20;}
    else if (!u8isequal(test21, L"OK")) {_FAILS++; return test21;}
    else if (!u8isequal(test22, L"OK")) {_FAILS++; return test22;}
    else if (!u8isequal(test23, L"OK")) {_FAILS++; return test23;}
    else                                {return L"OK";}
}

u8char *test_parser__NodeArray_new() {
    _TESTS++;

    NodeArray *node_array = NodeArray_new(1);

    NodeArray_append(node_array, NodeInteger_new(5));
    NodeArray_append(node_array, NodeString_new(L"hello"));
    NodeArray_append(node_array, NodeVar_new(L"somevar"));

    u8char *test1 = expect_int   (node_array->array[0].integer, 5);
    u8char *test2 = expect_int   (node_array->array[0].type, NodeType_INTEGER);
    u8char *test3 = expect_string(node_array->array[1].string, L"hello");
    u8char *test4 = expect_int   (node_array->array[1].type, NodeType_STRING);
    u8char *test5 = expect_string(node_array->array[2].variable, L"somevar");
    u8char *test6 = expect_int   (node_array->array[2].type, NodeType_VAR);
    u8char *test7 = expect_int   (node_array->used, 3);

    NodeArray_free(node_array);

    if      (!u8isequal(test1, L"OK")) {_FAILS++; return test1;}
    else if (!u8isequal(test2, L"OK")) {_FAILS++; return test2;}
    else if (!u8isequal(test3, L"OK")) {_FAILS++; return test3;}
    else if (!u8isequal(test4, L"OK")) {_FAILS++; return test4;}
    else if (!u8isequal(test5, L"OK")) {_FAILS++; return test5;}
    else if (!u8isequal(test6, L"OK")) {_FAILS++; return test6;}
    else if (!u8isequal(test7, L"OK")) {_FAILS++; return test7;}
    else                               {return L"OK";}
}

u8char *test_parser__NodeArray_append() {
    _TESTS++;

    NodeArray *node_array = NodeArray_new(1);

    u8char *test1 = expect_int   (node_array->used, 0);

    NodeArray_append(node_array, NodeInteger_new(0));
    u8char *test2 = expect_int   (node_array->used, 1);

    NodeArray_append(node_array, NodeInteger_new(0));
    u8char *test3 = expect_int   (node_array->used, 2);

    NodeArray_append(node_array, NodeInteger_new(0));
    NodeArray_append(node_array, NodeInteger_new(0));
    NodeArray_append(node_array, NodeInteger_new(0));
    u8char *test4 = expect_int   (node_array->used, 5);

    NodeArray_free(node_array);

    if      (!u8isequal(test1, L"OK")) {_FAILS++; return test1;}
    else if (!u8isequal(test2, L"OK")) {_FAILS++; return test2;}
    else if (!u8isequal(test3, L"OK")) {_FAILS++; return test3;}
    else if (!u8isequal(test4, L"OK")) {_FAILS++; return test4;}
    else                               {return L"OK";}
}

/*
  TODO:
  - test u8fill
  - u8strip segfaults
*/
u8char *test_u8string() {
    _TESTS++;

    u8char *test1  = expect_int   (u8len(L"hello"), 5);
    u8char *test2  = expect_int   (u8size(L"hello"), 5*sizeof(u8char));
    u8char *test3  = expect_string(u8join(L"hello", L"world"), L"helloworld");
    //u8char *test4  = expect_string(u8strip(L"  abc  "), L"abc");
    u8char *test5  = expect_string(u8upper(L"hello"), L"HELLO");
    u8char *test6  = expect_string(u8lower(L"HELLO"), L"hello");
    u8char *test7  = expect_int   (u8find(L"helloworld", L"llo"), 3);
    u8char *test8  = expect_true  (u8contains(L"hello", L"llo"));
    u8char *test9  = expect_true  (u8startswith(L"hello", L"he"));
    u8char *test10 = expect_true  (u8endswith(L"world", L"ld"));
    u8char *test11 = expect_true  (u8isequal(L"abc", L"abc"));
    u8char *test12 = expect_true  (u8isdigit(L"012345"));
    u8char *test13 = expect_true  (u8isidentifier(L"_DUST0"));
    u8char *test14 = expect_true  (u8isempty(L"   "));
    u8char *test15 = expect_string(u8replace(L"hello", L"l", L"z"), L"hezzo");
    u8char *test16 = expect_string(u8slice(L"helloworld", 4, 9), L"world");
    u8char *test17 = expect_string(ctou8("hello"), L"hello");

    if      (!u8isequal(test1, L"OK"))  {_FAILS++; return test1;}
    else if (!u8isequal(test2, L"OK"))  {_FAILS++; return test2;}
    else if (!u8isequal(test3, L"OK"))  {_FAILS++; return test3;}
    //else if (!u8isequal(test4, L"OK"))  {_FAILS++; return test4;}
    else if (!u8isequal(test5, L"OK"))  {_FAILS++; return test5;}
    else if (!u8isequal(test6, L"OK"))  {_FAILS++; return test6;}
    else if (!u8isequal(test7, L"OK"))  {_FAILS++; return test7;}
    else if (!u8isequal(test8, L"OK"))  {_FAILS++; return test8;}
    else if (!u8isequal(test9, L"OK"))  {_FAILS++; return test9;}
    else if (!u8isequal(test10, L"OK")) {_FAILS++; return test10;}
    else if (!u8isequal(test11, L"OK")) {_FAILS++; return test11;}
    else if (!u8isequal(test12, L"OK")) {_FAILS++; return test12;}
    else if (!u8isequal(test13, L"OK")) {_FAILS++; return test13;}
    else if (!u8isequal(test14, L"OK")) {_FAILS++; return test14;}
    else if (!u8isequal(test15, L"OK")) {_FAILS++; return test15;}
    else if (!u8isequal(test16, L"OK")) {_FAILS++; return test16;}
    else if (!u8isequal(test17, L"OK")) {_FAILS++; return test17;}
    else                                {return L"OK";}
}

int main() {
    u8char *result_Token_new         = test_tokenizer__Token_new();
    u8char *result_Token_repr        = test_tokenizer__Token_repr();
    u8char *result_TokenArray_new    = test_tokenizer__TokenArray_new();
    u8char *result_TokenArray_append = test_tokenizer__TokenArray_append();
    u8char *result_Node_new          = test_parser__Node_new();
    u8char *result_NodeArray_new     = test_parser__NodeArray_new();
    u8char *result_NodeArray_append  = test_parser__NodeArray_append();
    u8char *result_u8string          = test_u8string();

    u8char *out = L"%d\n%d\n%d\n";

    out = u8join(out, u8join(u8join(L"tokenizer.c: Token_new         [", u8join(result_Token_new, L"]")), L"\n"));
    out = u8join(out, u8join(u8join(L"tokenizer.c: Token_repr        [", u8join(result_Token_repr, L"]")), L"\n"));
    out = u8join(out, u8join(u8join(L"tokenizer.c: TokenArray_new    [", u8join(result_TokenArray_new, L"]")), L"\n"));
    out = u8join(out, u8join(u8join(L"tokenizer.c: TokenArray_append [", u8join(result_TokenArray_append, L"]")), L"\n"));
    out = u8join(out, u8join(u8join(L"parser.c:    Node*_new         [", u8join(result_Node_new, L"]")), L"\n"));
    out = u8join(out, u8join(u8join(L"parser.c:    NodeArray_new     [", u8join(result_NodeArray_new, L"]")), L"\n"));
    out = u8join(out, u8join(u8join(L"parser.c:    NodeArray_append  [", u8join(result_NodeArray_append, L"]")), L"\n"));
    out = u8join(out, u8join(u8join(L"u8string.c:  *                 [", u8join(result_u8string, L"]")), L"\n"));

    wprintf(out, _TESTS, _FAILS, _IGNORES);

    free(out);

    return 0;
}