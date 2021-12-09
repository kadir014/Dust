"""

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust


  Dust tester script
  -------------------------------------------------------
  This Python script is used to test various Dust library
  functions using `tests.c`
  This script must be run in the same directory as Dust. 

"""

import os
import time
import platform
import subprocess


if platform.system() == "Windows":
    binaryfile = "tests.exe"
    binaryrun = "tests"
else:
    binaryfile = "tests"
    binaryrun = "./tests"


print("Running tests...")

if os.path.exists(binaryfile): os.remove(binaryfile)

if platform.system() == "Windows":
    os.system("gcc -o tests tests.c src/ustring.c src/error.c src/platform.c src/tokenizer.c src/parser.c -I./include/ -lws2_32")
else:
    os.system("gcc -o tests tests.c src/ustring.c src/error.c src/platform.c src/tokenizer.c src/parser.c -I./include/ -lm")

start = time.perf_counter()
out = subprocess.check_output(binaryrun).decode("utf-8").replace("\r", "")
end = time.perf_counter() - start

if os.path.exists(binaryfile): os.remove(binaryfile)


max_len = 26
outs = out.split("\n")
for i, l in enumerate(outs):
    if len(l) > max_len: max_len = len(l)

    if l.startswith("[PASSED]"):
        outs[i] = "[\033[92mPASSED\033[0m]" + l[8:]

    elif l.startswith("[FAILED]"):
        outs[i] = "[\033[91mFAILED\033[0m]" + l[8:]

    elif l.startswith("tests:"):
        test_count = int(l[6:])

    elif l.startswith("fails:"):
        fail_count = int(l[6:])

outs = outs[:-3]

if platform.system == "Windows":
    os.system(" ")
print("\033[1F\033[1F")
print(f"Ran {test_count} tests in {round(end, 3)} secs")
print(f"{fail_count} {('fail', 'fails')[bool(fail_count-1)]}")
print("-" * max_len)
print("\n".join(outs))