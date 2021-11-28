/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#pragma once
#ifndef TOKENIZER_H
#define TOKENIZER_H


#include <stdlib.h>
#include <dust/ustring.h>

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
} TokenType;

typedef struct {
    TokenType type;
    u32char *data;
    int x, y;
} Token;

Token *Token_new(TokenType type, u32char *data);

void Token_free(Token *token);

u32char *Token_repr(Token *token);

typedef struct {
    Token *array;
    size_t size;
    size_t used;
} TokenArray;

TokenArray *TokenArray_new(size_t def_size);

void TokenArray_free(TokenArray *token_array);

void TokenArray_append(TokenArray *token_array, Token *token);

TokenArray *TokenArray_slice(TokenArray *token_array, int index);

TokenArray *TokenArray_slicet(TokenArray *token_array, int index);

u32char *TokenArray_repr(TokenArray *token_array);

TokenArray *tokenize(u32char *raw);

TokenArray *tokenize_file(char *filepath);


#endif