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
    u8char *data;
    int x, y;
} Token;

/*
  Create a new Token and return its pointer

  TokenType type  ->  Type of the Token
  char *data      ->  String data the Token holds
*/
Token *Token_new(TokenType type, u8char *data) {
    Token *t = (Token *)malloc(sizeof(Token));
    
    t->type = type;
    t->data = data;
    t->x = 0;
    t->y = 0;

    return t;
}

/*
  Release all resources used by the Token

  Token *token  ->  Token to free
*/
void Token_free(Token *token) {
    token->type = 0;
    free(token->data);
    token->x = 0;
    token->y = 0;
    free(token);
}

/*
  Return a string representation of Token

  Token *token  ->  Token to return a repr. string of
*/
u8char *Token_repr(Token *token) {
    switch (token->type) {
        case TokenType_IDENTIFIER:
            return u8join(L"TokenType_IDENTIFIER   ", token->data);

        case TokenType_STRING:
            return u8join(L"TokenType_STRING       ", token->data);

        case TokenType_OPERATOR:
            return u8join(L"TokenType_OPERATOR     ", token->data);

        case TokenType_NUMERIC:
            return u8join(L"TokenType_NUMERIC      ", token->data);

        case TokenType_COMMA:
            return u8join(L"TokenType_COMMA        ", token->data);

        case TokenType_PERIOD:
            return u8join(L"TokenType_PERIOD       ", token->data);

        case TokenType_LPAREN:
            return u8join(L"TokenType_LPAREN       ", token->data);

        case TokenType_RPAREN:
            return u8join(L"TokenType_RPAREN       ", token->data);

        case TokenType_LCURLY:
            return u8join(L"TokenType_LCURLY       ", token->data);

        case TokenType_RCURLY:
            return u8join(L"TokenType_RCURLY       ", token->data);

        case TokenType_LSQRB:
            return u8join(L"TokenType_LSQRB        ", token->data);

        case TokenType_RSQRB:
            return u8join(L"TokenType_RSQRB        ", token->data);

        case TokenType_NEXTSTM:
            return u8join(L"TokenType_NEXTSTM      ", token->data);

        case TokenType_EOF:
            return u8join(L"TokenType_EOF          ", token->data);
    }
}


typedef struct {
    Token *array;
    size_t size;
    size_t used;
} TokenArray;

/*
  Create a new TokenArray and return its pointer

  unsigned short def_size  ->  Initial size of the array
*/
TokenArray *TokenArray_new(size_t def_size) {
    TokenArray *a = (TokenArray *)malloc(sizeof(TokenArray));

    a->array = malloc(def_size * sizeof(Token));
    a->used = 0;
    a->size = def_size;
}

/*
  Release all resources used by the TokenArray

  TokenArray *token_array  ->  TokenArray to free
*/
void TokenArray_free(TokenArray *token_array) {
    free(token_array->array);
    token_array->array = NULL;
    token_array->used = 0;
    token_array->size = 0;
    free(token_array);
}

/*
  Append a Token to TokenArray

  TokenArray *token_array  ->  TokenArray to append to
  Token *Token             ->  Token to append
*/
void TokenArray_append(TokenArray *token_array, Token *token) {
    if (token_array->used == token_array->size) {
        token_array->size *= 2;
        token_array->array = realloc(token_array->array, token_array->size * sizeof(Token));
    }

    token_array->array[token_array->used++] = *token;
}

/*
  Append a sliced copy of TokenArray

  TokenArray *token_array  ->  TokenArray to slice
  int index            ->  Index to slice from
*/
TokenArray *TokenArray_slice(TokenArray *token_array, int index) {
    TokenArray *slice_array = TokenArray_new(1);

    while (index < token_array->used) {
        TokenArray_append(slice_array, &(token_array->array[index]));
        index++;
    }

    return slice_array;
}

/*
  Append a sliced copy of TokenArray

  TokenArray *token_array  ->  TokenArray to slice
  int index            ->  Index to slice from
*/
TokenArray *TokenArray_slicet(TokenArray *token_array, int index) {
    TokenArray *slice_array = TokenArray_new(1);

    while (index < token_array->used) {
        if (token_array->array[index].type == TokenType_NEXTSTM ||
            token_array->array[index].type == TokenType_EOF) break;
        TokenArray_append(slice_array, &(token_array->array[index]));
        index++;
    }

    return slice_array;
}

/*
  Return a string representation of TokenArray

  TokenArray *token_array  ->  TokenArray to return a repr. string of
*/
u8char *TokenArray_repr(TokenArray *token_array) {
    unsigned short i = 0;
    u8char *finalstr = L"";
    for (i = 0; i < token_array->used; ++i) {
        finalstr = u8join(u8join(finalstr, Token_repr( &(token_array->array[i]) )), L"\n");
    }

    return finalstr;
}


/*
  Helper function to tokenize
*/
void tokenize_append(Token* token, TokenArray *tokens, int x, int y) {
    u8char *t = u8strip(token->data);

    if (u8isdigit(t) == 1) {
        token->type = TokenType_NUMERIC;
        token->data = t;
        token->x = x;
        token->y = y;
        TokenArray_append(tokens, token);
    }

    else if (u8isequal(t, L"(") == 1 || u8isequal(t, L")") == 1 || u8isequal(t, L"[") == 1 ||
             u8isequal(t, L"]") == 1 || u8isequal(t, L"{") == 1 || u8isequal(t, L"}") == 1) {
                
                switch (t[0]) {
                        case L'(': token->type = TokenType_LPAREN; break;
                        case L')': token->type = TokenType_RPAREN; break;
                        case L'[': token->type = TokenType_LSQRB;  break;
                        case L']': token->type = TokenType_RSQRB;  break;
                        case L'{': token->type = TokenType_LCURLY; break;
                        case L'}': token->type = TokenType_RCURLY; break;
                    }
                token->data = t;
                token->x = x;
                token->y = y;

                TokenArray_append(tokens, token);
    }

    else if (wcslen(t) > 0) {
        if (u8isidentifier(t) == 0) {
            printf("%d\n", u8isidentifier(t));
            printf("%lc %d\n", t[0], !!iswalnum(t[0]));
            printf("%lc %d\n", t[1], !!iswalnum(t[1]));
            printf("%lc %d\n", t[2], !!iswalnum(t[2]));
            printf("%lc %d\n", t[3], !!iswalnum(t[3]));
            printf("%lc %d\n", t[4], !!iswalnum(t[4]));
            printf("%lc %d\n", t[5], !!iswalnum(t[5]));
            u8char *errmsg = u8join(u8join(L"Invalid identifier '", t), L"'");
            raise(ErrorType_Syntax, errmsg, L"<raw>", x, y);
        }

        token->type = TokenType_IDENTIFIER;
        
        if (u8isequal(t, L"and") == 1 || u8isequal(t, L"or")  == 1 ||
            u8isequal(t, L"xor") == 1 || u8isequal(t, L"not") == 1) {
                token->type = TokenType_OPERATOR;
            }

        token->data = t;
        token->x = x;
        token->y = y;

        TokenArray_append(tokens, token);
    }
}

/*
  Tokenize/lex a source code of string

  char *raw  ->  String to tokenize
*/
TokenArray *tokenize(u8char *raw){
    TokenArray *tokens = TokenArray_new(1);
    Token *token = Token_new(TokenType_EOF, L"");

    int x = 0;
    int y = 0;
    u8char chr = L'\0';
    int i = 0;
    u8char string_type = L'\0';

    while (i < wcslen(raw) && raw[i] != EOF) {
        chr = raw[i];

        if (chr == L'"' || chr == L'\'') {
            string_type = chr;
            chr = L'\0';
            token->data = L"";

            while (chr != string_type) {
                i++;
                x++;

                if (i > wcslen(raw)) {
                    raise(ErrorType_Syntax, L"String not closed", L"<raw>", x, y);
                }

                if (raw[i] == string_type) break;
                chr = raw[i];
                u8char chrstr[2];
                token->data = u8join(token->data, u8char_to_u8string(&chr, chrstr));
            }

            token->type = TokenType_STRING;
            token->x = x - wcslen(token->data) - 1;
            token->y = y;

            TokenArray_append(tokens, token);
            token = Token_new(TokenType_EOF, L"");

            i++;
            x++;
            continue;
        }

        if (chr == L'\n') {
            x = 0;
            y++;
        }

        else if (chr == L' ') {
            if (wcslen(token->data) > 0) {
                tokenize_append(token, tokens, x, y);
                token = Token_new(TokenType_EOF, L"");
            }
            i++;
            x++;
            continue;
        }

        else if (chr == L'+' || chr == L'-' || chr == L'*' ||
                 chr == L'/' || chr == L'^' || chr == L'=' ||
                 chr == L'>' || chr == L'<' || chr == L'!') {

                    if (wcslen(token->data) > 0) {
                        tokenize_append(token, tokens, x, y);
                        token = Token_new(TokenType_EOF, L"");
                    }

                    token->type = TokenType_OPERATOR;
                    u8char chrstr[2];
                    token->data = u8join(L"", u8char_to_u8string(&chr, chrstr));
                    token->x = x;
                    token->y = y;
                    
                    TokenArray_append(tokens, token);
                    token = Token_new(TokenType_EOF, L"");

                    i++;
                    x++;
                    continue;
                }

        else if (chr == L'(' || chr == L')' || chr == L'[' ||
                 chr == L']' || chr == L'{' || chr == L'}') {

                    if (wcslen(token->data) > 0) {
                        tokenize_append(token, tokens, x, y);
                        token = Token_new(TokenType_EOF, L"");
                    }

                    switch (chr) {
                        case L'(': token->type = TokenType_LPAREN; break;
                        case L')': token->type = TokenType_RPAREN; break;
                        case L'[': token->type = TokenType_LSQRB;  break;
                        case L']': token->type = TokenType_RSQRB;  break;
                        case L'{': token->type = TokenType_LCURLY; break;
                        case L'}': token->type = TokenType_RCURLY; break;
                    }
                    u8char chrstr[2];
                    token->data = u8join(L"", u8char_to_u8string(&chr, chrstr));
                    token->x = x;
                    token->y = y;

                    TokenArray_append(tokens, token);
                    token = Token_new(TokenType_EOF, L"");

                    i++;
                    x++;
                    continue;
                 }

        else if (chr == L',') {
            if (wcslen(token->data) > 0) {
                tokenize_append(token, tokens, x, y);
                token = Token_new(TokenType_EOF, L"");
            }

            token->type = TokenType_COMMA;
            u8char chrstr[2];
            token->data = u8join(L"", u8char_to_u8string(&chr, chrstr));
            token->x = x;
            token->y = y;

            TokenArray_append(tokens, token);
            token = Token_new(TokenType_EOF, L"");

            i++;
            x++;
            continue;
        }

        else if (chr == L'.') {
            if (wcslen(token->data) > 0) {
                tokenize_append(token, tokens, x, y);
                token = Token_new(TokenType_EOF, L"");
            }

            if (raw[i+1] == L'.') {
                token->type = TokenType_OPERATOR;
                token->data = L"..";
                i++;
            }
            else {
                token->type = TokenType_PERIOD;
                u8char chrstr[2];
                token->data = u8join(L"", u8char_to_u8string(&chr, chrstr));
            }
            token->x = x;
            token->y = y;

            TokenArray_append(tokens, token);
            token = Token_new(TokenType_EOF, L"");

            i++;
            x++;
            continue;
        }

        else if (chr == L';') {
            if (wcslen(token->data) > 0) {
                tokenize_append(token, tokens, x, y);
                token = Token_new(TokenType_EOF, L"");
            }

            token->type = TokenType_NEXTSTM;
            token->data = L"";
            token->x = x;
            token->y = y;

            TokenArray_append(tokens, token);
            token = Token_new(TokenType_EOF, L"");

            i++;
            x++;
            continue;
        }

        u8char chrstr[2];
        token->data = u8join(token->data, u8char_to_u8string(&chr, chrstr));
        x++;
        i++;
    }
    
    // Change last NEXTSTM token to EOF token
    if (tokens->array[tokens->used - 1].type == TokenType_NEXTSTM) {
        tokens->array[tokens->used - 1] = *Token_new(TokenType_EOF, L"");
    }
    // Add EOF token if necessary
    else if (tokens->array[tokens->used - 1].type == TokenType_RCURLY) {
        TokenArray_append(tokens, Token_new(TokenType_EOF, L""));
    }

    return tokens;
}

/*
  Tokenize/lex a source code in file

  char *filepath  ->  Path of the file to tokenize
*/
TokenArray *tokenize_file(char *filepath) {
    u8char *buffer;
    long length;
    FILE *f = fopen(filepath, "r,ccs=UTF-8");
    fwide(f, 1);

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length*sizeof(u8char));

        if (buffer) {
            //fread(buffer, sizeof(u8char), length, f);
            while (fgetws(buffer, length, f) != NULL) {}
        }

        fclose(f);
    }

    if (buffer) {
        TokenArray *token_array = tokenize(buffer);
        free(buffer);
        return token_array;
    }
}