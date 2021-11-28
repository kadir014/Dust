/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#pragma once
#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H


#include "dust/ustring.h"


typedef enum {
    ErrorType_Syntax,
} ErrorType;


extern int ERROR_ANSI;

void raise_ansi(ErrorType type, u32char *message, u32char *source, int x, int y);

void raise_noansi(ErrorType type, u32char *message, u32char *source, int x, int y);

void raise(ErrorType type, u32char *message, u32char *source, int x, int y);

void raise_internal(u32char *message);


#endif