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


print("Running tests...")

if os.path.exists("tests.exe"): os.remove("tests.exe")

if platform.system() == "Windows":
    os.system("gcc -o tests tests.c src/ustring.c src/error.c src/platform.c src/tokenizer.c src/parser.c -I./include/ -lws2_32")
else:
    os.system("gcc -o tests tests.c src/ustring.c src/error.c src/platform.c src/tokenizer.c src/parser.c -I./include/")

start = time.perf_counter()
out = subprocess.check_output("tests").decode("utf-8").replace("\r", "")
end = time.perf_counter() - start

if os.path.exists("tests.exe"): os.remove("tests.exe")


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

# outs = out.split("\n")
# test_count = outs[0]
# fail_count = outs[1]
# ignored_count = outs[2]
# out = outs[3:][:-1]

# _out = []

# for index, line in enumerate(out):
#     i = line.find("[")

#     if i == -1: continue

#     if "OK" in line:
#         out[index] = line[:i] + "[\u001b[92mPASSED\u001b[0m]"

#     else:
#         err = line[i+1:len(line)-1]
#         if err.startswith("Expected") and err[-1] != "'": err += "'"
#         out[index] = line[:i] + "[\u001b[91mFAILED\u001b[0m] " + err

#     _out.append(out[index])


# if os.path.exists("tests.exe"): os.remove("tests.exe")

# os.system(" ")

# if test_count == "1": _tests_str = "test"
# else: _tests_str = "tests"
# if fail_count == "1": _fails_str = "Failure"
# else: _fails_str = "Failures"
# print(f"Ran {test_count} {_tests_str} in {round(end, 3)} secs")
# print(f"{fail_count} {_fails_str} {ignored_count} Ignored")
# print("---------------------------------------")
# print("\n".join(_out))