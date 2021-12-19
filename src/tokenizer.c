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


/**
 * @param type Type of the token
 * @param data Token's data
 * @param xy Corresponding position of token in file
 */
typedef struct {
    TokenType type;
    u32char *data;
    int x, y;
} Token;


/**
 * @brief Create a new token
 * 
 * @param type Type of the token
 * @param data String data that token holds
 * @return Token's pointer
 */
Token *Token_new(TokenType type, u32char *data) {
    Token *token = (Token *)malloc(sizeof(Token));
    
    token->type = type;
    token->data = data;
    token->x = 0;
    token->y = 0;

    return token;
}

/**
 * @brief Release all resources used by the token
 * 
 * @param token Token to free
 */
void Token_free(Token *token) {
    free(token->data);
    free(token);
}

/**
 * @brief Represent token as string
 * 
 * @param token Token to return a repr. string of
 * @return String representation
 */
u32char *Token_repr(Token *token) {
    switch (token->type) {
        case TokenType_IDENTIFIER:
            return u32join(U"TokenType_IDENTIFIER   ", token->data);

        case TokenType_STRING:
            return u32join(U"TokenType_STRING       ", token->data);

        case TokenType_OPERATOR:
            return u32join(U"TokenType_OPERATOR     ", token->data);

        case TokenType_NUMERIC:
            return u32join(U"TokenType_NUMERIC      ", token->data);

        case TokenType_COMMA:
            return u32join(U"TokenType_COMMA        ", token->data);

        case TokenType_PERIOD:
            return u32join(U"TokenType_PERIOD       ", token->data);

        case TokenType_LPAREN:
            return u32join(U"TokenType_LPAREN       ", token->data);

        case TokenType_RPAREN:
            return u32join(U"TokenType_RPAREN       ", token->data);

        case TokenType_LCURLY:
            return u32join(U"TokenType_LCURLY       ", token->data);

        case TokenType_RCURLY:
            return u32join(U"TokenType_RCURLY       ", token->data);

        case TokenType_LSQRB:
            return u32join(U"TokenType_LSQRB        ", token->data);

        case TokenType_RSQRB:
            return u32join(U"TokenType_RSQRB        ", token->data);

        case TokenType_NEXTSTM:
            return u32join(U"TokenType_NEXTSTM      ", token->data);

        case TokenType_EOF:
            return u32join(U"TokenType_EOF          ", token->data);
    }
}


/**
 * @param array Token array
 * @param size Default size
 * @param used Length of the array
 */
typedef struct {
    Token *array;
    size_t size;
    size_t used;
} TokenArray;

/**
 * @brief Create a new token array
 * 
 * @param def_size  Initial size of the array
 * @return Token array's pointer
 */
TokenArray *TokenArray_new(size_t def_size) {
    TokenArray *token_array = (TokenArray *)malloc(sizeof(TokenArray));

    token_array->array = malloc(def_size * sizeof(Token));
    token_array->used = 0;
    token_array->size = def_size;

    return token_array;
}

/**
 * @brief Release all resources used by the token array
 * 
 * @param token_array Token array to free
 */
void TokenArray_free(TokenArray *token_array) {
    free(token_array->array);
    token_array->array = NULL;
    token_array->used = 0;
    token_array->size = 0;
    free(token_array);
}

/**
 * @brief Append a token to token array
 * 
 * @param token_array Token array to append to
 * @param token Token to append
 */
void TokenArray_append(TokenArray *token_array, Token *token) {
    if (token_array->used == token_array->size) {
        token_array->size *= 2;
        token_array->array = realloc(token_array->array, token_array->size * sizeof(Token));
    }

    token_array->array[token_array->used++] = *token;
}

/**
 * @brief Get a slice of the token array
 * 
 * @param token_array Token array to slice
 * @param index Index to start slicing from
 * @return Sliced token array's pointer
 */
TokenArray *TokenArray_slice(TokenArray *token_array, int index) {
    TokenArray *slice_array = TokenArray_new(1);

    while (index < token_array->used) {
        TokenArray_append(slice_array, &(token_array->array[index]));
        index++;
    }

    return slice_array;
}

/**
 * @brief Get a slice of the token array
 * 
 * @param token_array Token array to slice
 * @param index Index to start slicing from
 * @return Sliced token array's pointer
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

/**
 * @brief Represent token array as string
 * 
 * @param token_array Token array to return a repr. string of
 * @return Representation string
 */
u32char *TokenArray_repr(TokenArray *token_array) {
    unsigned short i = 0;
    u32char *finalstr = U"";
    for (i = 0; i < token_array->used; ++i) {
        finalstr = u32join(u32join(finalstr, Token_repr( &(token_array->array[i]) )), U"\n");
    }

    return finalstr;
}


/**
 * @brief Helper function to tokenize
 */
void tokenize_append(Token* token, TokenArray *tokens, int x, int y) {
    u32char *t = u32replace(u32strip(token->data), U"\n", U"");

    // Decimal integer literal
    if (u32isdigit(t)) {
        token->type = TokenType_NUMERIC;
        token->data = t;
        token->x = x;
        token->y = y;
        TokenArray_append(tokens, token);
    }

    // Hexedecimal integer literal
    else if (t[0] == U'0' && t[1] == U'x' && u32isxdigit(u32slice(t, 2, u32len(t)))) {
        token->type = TokenType_NUMERIC;
        token->data = t;
        token->x = x;
        token->y = y;
        TokenArray_append(tokens, token);
    }

    // Binary integer literal
    else if (t[0] == U'0' && t[1] == U'b' && u32isbdigit(u32slice(t, 2, u32len(t)))) {
        token->type = TokenType_NUMERIC;
        token->data = t;
        token->x = x;
        token->y = y;
        TokenArray_append(tokens, token);
    }

    else if (u32isequal(t, U"(") || u32isequal(t, U")") || u32isequal(t, U"[") ||
             u32isequal(t, U"]") || u32isequal(t, U"{") || u32isequal(t, U"}")) {
                
                switch (t[0]) {
                        case U'(': token->type = TokenType_LPAREN; break;
                        case U')': token->type = TokenType_RPAREN; break;
                        case U'[': token->type = TokenType_LSQRB;  break;
                        case U']': token->type = TokenType_RSQRB;  break;
                        case U'{': token->type = TokenType_LCURLY; break;
                        case U'}': token->type = TokenType_RCURLY; break;
                    }
                token->data = t;
                token->x = x;
                token->y = y;

                TokenArray_append(tokens, token);
    }

    else if (u32len(t) > 0) {
        //! what is this part?
        // if (!u32isidentifier(t)) {
        //     printf(U"%d\n", u32isidentifier(t));
        //     printf(U"%lc %d\n", t[0], !!iswalnum(t[0]));
        //     printf(U"%lc %d\n", t[1], !!iswalnum(t[1]));
        //     printf(U"%lc %d\n", t[2], !!iswalnum(t[2]));
        //     printf(U"%lc %d\n", t[3], !!iswalnum(t[3]));
        //     printf(U"%lc %d\n", t[4], !!iswalnum(t[4]));
        //     printf(U"%lc %d\n", t[5], !!iswalnum(t[5]));
        //     u32char *errmsg = u32join(u32join(U"Invalid identifier '", t), U"'");
        //     raise(ErrorType_Syntax, errmsg, U"<raw>", x, y);
        // }

        token->type = TokenType_IDENTIFIER;
        
        if (u32isequal(t, U"and") || u32isequal(t, U"or")  ||
            u32isequal(t, U"xor") || u32isequal(t, U"not") ||
            u32isequal(t, U"in")) {
                token->type = TokenType_OPERATOR;
            }

        token->data = t;
        token->x = x;
        token->y = y;

        TokenArray_append(tokens, token);
    }
}

/**
 * @brief Tokenize a source code of string
 * 
 * @param raw String to tokenize
 * @return Token array's pointer
 */
TokenArray *tokenize(u32char *raw){
    TokenArray *tokens = TokenArray_new(1);
    //raw = u32replace(u32strip(raw), U"\n", U"");
    //raw = u32strip(raw);
    if (u32len(raw) == 0) return tokens;
    Token *token = Token_new(TokenType_EOF, U"");

    int x = 0;
    int y = 0;
    u32char chr = U'\0';
    int i = 0;
    u32char string_type = U'\0';

    while (i < u32len(raw) && raw[i] != EOF) {
        chr = raw[i];

        if (chr == U'"' || chr == U'\'') {
            string_type = chr;
            chr = U'\0';
            token->data = U"";

            while (chr != string_type) {
                i++;
                x++;

                if (i > u32len(raw)) {
                    raise(ErrorType_Syntax, U"String not closed", U"<stdin>", x, y);
                }

                if (raw[i] == string_type) break;
                chr = raw[i];
                token->data = u32pushl(token->data, chr);
            }

            token->type = TokenType_STRING;
            token->x = x - u32len(token->data) - 1;
            token->y = y;

            TokenArray_append(tokens, token);
            token = Token_new(TokenType_EOF, U"");

            i++;
            x++;
            continue;
        }

        if (chr == U'\n') {
            x = 0;
            y++;
            i++;
            continue;
        }

        else if (chr == U' ') {
            if (u32len(token->data) > 0) {
                tokenize_append(token, tokens, x, y);
                token = Token_new(TokenType_EOF, U"");
            }
            i++;
            x++;
            continue;
        }

        else if (chr == U'/' && raw[i+1] == U'/') {
            while (raw[i] != U'\n') i++;

            i++;
            x = 0;
            y += 1;
            continue;
        }

        else if (chr == U'/' && raw[i+1] == U'*') {
            i+=2;
            while (!(raw[i] == U'*' && raw[i+1] == U'/')) i++;

            i+=2;
            x++;
            continue;
        }

        else if (chr == U'+' || chr == U'-' || chr == U'*' ||
                 chr == U'/' || chr == U'^' || chr == U'=' ||
                 chr == U'>' || chr == U'<' || chr == U'!' ||
                 chr == U'%') {

            if (u32len(token->data) > 0) {
                tokenize_append(token, tokens, x, y);
                token = Token_new(TokenType_EOF, U"");
            }

            if (chr == U'=' && raw[i+1] == U'=') {
                token->data = U"==";
                i++;
            }
            else if (chr == U'+' && raw[i+1] == U'=') {
                token->data = U"+=";
                i++;
            }
            else if (chr == U'-' && raw[i+1] == U'=') {
                token->data = U"-=";
                i++;
            }
            else if (chr == U'*' && raw[i+1] == U'=') {
                token->data = U"*=";
                i++;
            }
            else if (chr == U'/' && raw[i+1] == U'=') {
                token->data = U"/=";
                i++;
            }
            else if (chr == U'^' && raw[i+1] == U'=') {
                token->data = U"^=";
                i++;
            }
            else if (chr == U'<' && raw[i+1] == U'=') {
                token->data = U"<=";
                i++;
            }
            else if (chr == U'>' && raw[i+1] == U'=') {
                token->data = U">=";
                i++;
            }
            else if (chr == U'!' && raw[i+1] == U'=') {
                token->data = U"!=";
                i++;
            }
            else if (chr == U'%' && raw[i+1] == U'=') {
                token->data = U"%=";
                i++;
            }
            else {
                token->data = u32pushl(token->data, chr);
                //token->data = u32join(U"", u32char_to_u32string(&chr, chrstr));
            }

            token->type = TokenType_OPERATOR;
            token->x = x;
            token->y = y;
            
            TokenArray_append(tokens, token);
            token = Token_new(TokenType_EOF, U"");

            i++;
            x++;
            continue;
        }

        else if (chr == U'(' || chr == U')' || chr == U'[' ||
                 chr == U']' || chr == U'{' || chr == U'}') {

                    if (u32len(token->data) > 0) {
                        tokenize_append(token, tokens, x, y);
                        token = Token_new(TokenType_EOF, U"");
                    }

                    switch (chr) {
                        case U'(': token->type = TokenType_LPAREN; break;
                        case U')': token->type = TokenType_RPAREN; break;
                        case U'[': token->type = TokenType_LSQRB;  break;
                        case U']': token->type = TokenType_RSQRB;  break;
                        case U'{': token->type = TokenType_LCURLY; break;
                        case U'}': token->type = TokenType_RCURLY; break;
                    }
                    token->data = u32pushl(token->data, chr);
                    token->x = x;
                    token->y = y;

                    TokenArray_append(tokens, token);
                    token = Token_new(TokenType_EOF, U"");

                    i++;
                    x++;
                    continue;
                 }

        else if (chr == U',') {
            if (u32len(token->data) > 0) {
                tokenize_append(token, tokens, x, y);
                token = Token_new(TokenType_EOF, U"");
            }

            token->type = TokenType_COMMA;
            token->data = u32pushl(token->data, chr);
            token->x = x;
            token->y = y;

            TokenArray_append(tokens, token);
            token = Token_new(TokenType_EOF, U"");

            i++;
            x++;
            continue;
        }

        else if (chr == U'.') {
            if (u32len(token->data) > 0) {
                tokenize_append(token, tokens, x, y);
                token = Token_new(TokenType_EOF, U"");
            }

            if (raw[i+1] == U'.') {
                token->type = TokenType_OPERATOR;
                token->data = U"..";
                i++;
            }
            else {
                token->type = TokenType_PERIOD;
                token->data = u32pushl(token->data, chr);
            }
            token->x = x;
            token->y = y;

            TokenArray_append(tokens, token);
            token = Token_new(TokenType_EOF, U"");

            i++;
            x++;
            continue;
        }

        else if (chr == U';') {
            if (u32len(token->data) > 0) {
                tokenize_append(token, tokens, x, y);
                token = Token_new(TokenType_EOF, U"");
            }

            token->type = TokenType_NEXTSTM;
            token->data = U"";
            token->x = x;
            token->y = y;

            TokenArray_append(tokens, token);
            token = Token_new(TokenType_EOF, U"");

            i++;
            x++;
            continue;
        }

        token->data = u32pushl(token->data, chr);
        x++;
        i++;
    }

    if (u32len(token->data) > 0) {
        tokenize_append(token, tokens, x, y);
    }
    
    // Change last NEXTSTM token to EOF token
    if (tokens->array[tokens->used - 1].type == TokenType_NEXTSTM) {
        tokens->array[tokens->used - 1] = *Token_new(TokenType_EOF, U"");
    }
    // Add EOF token if necessary
    else if (tokens->array[tokens->used - 1].type == TokenType_RCURLY) {
        TokenArray_append(tokens, Token_new(TokenType_EOF, U""));
    }
    else {
        raise(ErrorType_Syntax, U"Expected ;", U"<stdin>",
           tokens->array[tokens->used - 1].x,
           tokens->array[tokens->used - 1].y);
    }

    return tokens;
}

/**
 * @brief Tokenize a source code in file
 * 
 * @param filepath Path of the file to tokenize
 * @return Token array's pointer
 */
TokenArray *tokenize_file(char *filepath) {
    u32char *filecontent = u32readfile(filepath);

    TokenArray *token_array = tokenize(filecontent);
    free(filecontent);
    return token_array;
}