/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

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
    u8char *test3 = expect_string(token_array->array[0].data, L"+");
    u8char *test4 = expect_int   (token_array->array[0].type, TokenType_OPERATOR);
    u8char *test5 = expect_string(token_array->array[0].data, L"");
    u8char *test6 = expect_int   (token_array->array[0].type, TokenType_NEXTSTM);
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

int main() {
    u8char *result_Token_new         = test_tokenizer__Token_new();
    u8char *result_Token_repr        = test_tokenizer__Token_repr();
    u8char *result_TokenArray_new    = test_tokenizer__TokenArray_new();
    u8char *result_TokenArray_append = test_tokenizer__TokenArray_append();

    u8char *out = L"%d\n%d\n%d\n";

    out = u8join(out, u8join(u8join(L"tokenizer.c: Token_new         [", u8join(result_Token_new, L"]")), L"\n"));
    out = u8join(out, u8join(u8join(L"tokenizer.c: Token_repr        [", u8join(result_Token_repr, L"]")), L"\n"));
    out = u8join(out, u8join(u8join(L"tokenizer.c: TokenArray_new    [", u8join(result_TokenArray_new, L"]")), L"\n"));
    out = u8join(out, u8join(u8join(L"tokenizer.c: TokenArray_append [", u8join(result_TokenArray_append, L"]")), L"\n"));

    wprintf(out, _TESTS, _FAILS, _IGNORES);

    return 0;
}