/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#pragma once
#ifndef INFO_H
#define INFO_H


#define __STR(x) #x
#define STR(x) __STR(x)


#define DUST_VERSION_MAJOR 0
#define DUST_VERSION_MINOR 0
#define DUST_VERSION_PATCH 17
#define DUST_VERSION_STR            \
        STR(DUST_VERSION_MAJOR) "." \
        STR(DUST_VERSION_MINOR) "." \
        STR(DUST_VERSION_PATCH)


#if defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)
#define COMPILER "GCC"
#define COMPILER_VERSION_STR    \
        STR(__GNUC__) "."       \
        STR(__GNUC_MINOR__) "." \
        STR(__GNUC_PATCHLEVEL__)

#elif defined(__clang__)
#define COMPILER "CLANG"
#define COMPILER_VERSION_STR     \
        STR(__clang_major_) "."  \
        STR(__clang_minor__) "." \
        STR(__clang_patchlevel__)

#elif defined(_MSC_VER)
#define COMPILER "MSVC"
#define COMPILER_VERSION_STR STR(_MSC_VER)

#endif


#endif