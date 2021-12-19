/*

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

*/

#pragma once
#ifndef IO_H
#define IO_H


char *read_file(char *filepath);

void write_file(char *filepath, char *content);

int create_file(char *filepath);

int remove_file(char *filepath);


#endif