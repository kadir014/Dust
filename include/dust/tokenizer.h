/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum {
    TokenType_IDENTIFIER,
    TokenType_STRING,
    TokenType_OPERATOR,
    TokenType_NUMERIC,
    TokenType_COMMA,
    TokenType_PERIOD,
    TokenType_LPAREN,
    TokenType_RPAREN,
    TokenType_LCURLY,
    TokenType_RCURLY,
    TokenType_LSQRB,
    TokenType_RSQRB,
    TokenType_NEXTSTM,
    TokenType_EOF,
} TokenTypeD;

typedef struct {
    TokenTypeD type;
    wchar_t *data;
    int x, y;
} Token;

Token *Token_new(TokenTypeD type, wchar_t *data);

void Token_free(Token *token);

wchar_t *Token_repr(Token *token);

typedef struct {
    Token *array;
    unsigned short size;
    unsigned short used;
} TokenArray;

TokenArray *TokenArray_new(unsigned short def_size);

void TokenArray_free(TokenArray *token_array);

void TokenArray_append(TokenArray *token_array, Token *token);

wchar_t *TokenArray_repr(TokenArray *token_array);

TokenArray *tokenize(wchar_t *raw);

TokenArray *tokenize_file(char *filepath);

#endif