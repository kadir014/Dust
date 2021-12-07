/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust


  WARNING: Dust Transpiler (transpiler.c & dust/transpiler.h)
  is still an experimental part of Dust, and may change or
  get removed in the future

*/

#pragma once
#ifndef TRANSPILER_H
#define TRANSPILER_H


#include "dust/ustring.h"
#include "dust/parser.h"

void transpile(NodeArray *node_array);

u32char *translate_expr(Node *node);

u32char *translate_op(OpType op);

u32char *translate_decl(Node *node);


#endif