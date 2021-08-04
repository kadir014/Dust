"""

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

"""


import os
import platform
import time

# Delete previously compiled binary
if platform.system() == "Windows": binaryfile = "dust.exe"
else: binaryfile = "dust"
if os.path.exists(binaryfile): os.remove(binaryfile)

print("Started compiling...\n")
start = time.time()

if platform.system() == "Windows":
    #TODO: Add windows flags
    os.system("gcc -o dust src/cli.c src/u8string.c src/error.c src/platform.c src/tokenizer.c src/parser.c -I./include/")
else:
    os.system("gcc -o dust src/cli.c src/u8string.c src/error.c src/platform.c src/tokenizer.c src/parser.c -I./include/")

# Compiled succesfully
if os.path.exists(binaryfile):
  end = time.time() - start
  print(f"Compiled in {round(end, 3)} secs ({int(end*1000)} ms)")