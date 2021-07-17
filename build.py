"""

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

"""


import os
import platform


if platform.system() == "Windows":
    #TODO: Add windows flags
    os.system("gcc -o dust src/cli.c src/u8string.c src/tokenizer.c -I./include/")
else:
    os.system("gcc -o dust src/cli.c src/u8string.c src/tokenizer.c -I./include/")