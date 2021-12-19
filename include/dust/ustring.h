/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#pragma once
#ifndef USTRING_H
#define USTRING_H


#include <stdint.h>
#include <stdbool.h>

typedef uint32_t u32char;


size_t u32len(u32char *str);

char *u8push(char *str, char chr);

u32char *u32push(u32char *str, u32char chr);
u32char *u32pushl(u32char *str, u32char chr);

char *utf32_to_utf8(u32char *str);

char *utf32_to_ascii(u32char *str);

u32char *utf8_to_utf32(char *str);

u32char *ascii_to_utf32(char *str);

bool u32isempty(u32char *str);

bool u32isequal(u32char *str1, u32char *str2);

bool u32isidentifier(u32char *str);

bool u32startswith(u32char *str, u32char *substr);

bool u32endswith(u32char *str, u32char *substr);

bool u32contains(u32char *str, u32char *substr);

void u32concat(u32char *dest, u32char *src);

void u32copy(u32char *dest, u32char *src);

long u32find(u32char *src, u32char *str);

long u32findchr(u32char *src, u32char chr);

long u32rfind(u32char *src, u32char *str);

long u8rfind(char *src, char *str);

long u32rfindchr(u32char *src, u32char chr);

size_t u32count(u32char *str, u32char *substr);

size_t u32countchr(u32char *str, u32char chr);

u32char *u32join(u32char *str1, u32char *str2);

u32char *u32replace(u32char *str, u32char *oldstr, u32char *newstr);

u32char *u32strip(u32char *str);

u32char *u32slice(u32char *str, size_t start, size_t end);

u32char *u32fill(u32char *dest, u32char *str, size_t amount);

bool u32cisalnum(u32char chr);

bool u32cisdigit(u32char chr);

bool u32cisxdigit(u32char chr);

bool u32cisbdigit(u32char chr);

bool u32cisspace(u32char chr);

bool u32isdigit(u32char *str);

bool u32isxdigit(u32char *str);

bool u32isbdigit(u32char *str);

char *u8readfile(char *filepath);

u32char *u32readfile(char *filepath);

int u32toint(u32char *str, int base);

double u32tofloat(u32char *str);

#endif