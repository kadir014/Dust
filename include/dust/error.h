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


void raise(ErrorType type, u8char *message, u8char *source, int x, int y);


#endif