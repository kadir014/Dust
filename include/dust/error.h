/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H


#include "dust/u8string.h"


typedef enum {
    ErrorType_Syntax,
} ErrorType;


int ERROR_ANSI;

void raise_ansi(ErrorType type, u8char *message, u8char *source, int x, int y);

void raise_noansi(ErrorType type, u8char *message, u8char *source, int x, int y);

void raise(ErrorType type, u8char *message, u8char *source, int x, int y);


#endif