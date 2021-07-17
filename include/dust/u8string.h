/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#ifndef U8STRING_H
#define U8STRING_H


typedef wchar_t u8char;
typedef char byte;


void u8winterminal();

size_t u8len(u8char *str);

size_t u8size(u8char *str);

u8char *u8join(u8char *str1, u8char *str2);

u8char *u8strip(u8char *str);

u8char *u8upper(u8char *str);

u8char *u8lower(u8char *str);

u8char *u8char_to_u8string(u8char *chr, u8char *dest);

int u8find(u8char *str, u8char *substr);

byte u8contains(u8char *str, u8char *substr);

byte u8startswith(u8char *str, u8char *substr);

byte u8endswith(u8char *str, u8char *substr);

byte u8isequal(u8char *str1, u8char *str2);

byte u8isdigit(u8char *str);

byte u8isidentifier(u8char *str);

byte u8isempty(u8char *str);


#endif