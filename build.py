"""

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust

"""

import os
import sys
import platform
import time
import multiprocessing as mp
import subprocess


GH_REPO = "https://github.com/kadir014/Dust"
CPU_COUNT = mp.cpu_count()
FINAL_BUILD = "dust.exe" if platform.system() == "Windows" else "dust"
OPTIONS = "-lws2_32" if platform.system() == "Windows" else ""
OPTIMIZATION_LEVEL = 0

class ValidityError(Exception): pass


# Verify source and include files before anything else
# All files must be in the same directory as the build script
if not (os.path.exists(os.path.join("src", "cli.c"))                   and \
        os.path.exists(os.path.join("src", "error.c"))                 and \
        os.path.exists(os.path.join("src", "parser.c"))                and \
        os.path.exists(os.path.join("src", "platform.c"))              and \
        os.path.exists(os.path.join("src", "tokenizer.c"))             and \
        os.path.exists(os.path.join("src", "u8string.c"))              and \
        os.path.exists(os.path.join("include", "dust", "ansi.h"))      and \
        os.path.exists(os.path.join("include", "dust", "error.h"))     and \
        os.path.exists(os.path.join("include", "dust", "parser.h"))    and \
        os.path.exists(os.path.join("include", "dust", "platform.h"))  and \
        os.path.exists(os.path.join("include", "dust", "tokenizer.h")) and \
        os.path.exists(os.path.join("include", "dust", "u8string.h"))):
  
  raise ValidityError("Could not verify source files\n" +
                      "Try cloning the repo again from " + GH_REPO)


def remove_object_files():
  if os.path.exists("cli.o"):       os.remove("cli.o")
  if os.path.exists("error.o"):     os.remove("error.o")
  if os.path.exists("platform.o"):  os.remove("platform.o")
  if os.path.exists("parser.o"):    os.remove("parser.o")
  if os.path.exists("tokenizer.o"): os.remove("tokenizer.o")
  if os.path.exists("u8string.o"):  os.remove("u8string.o")


def add_option(option):
  global OPTIONS
  OPTIONS += " " + option


class Compiler:
  def __init__(self, procs):
    self.procs = procs
    sources = (os.path.join("src", "cli.c"), os.path.join("src", "u8string.c"),
               os.path.join("src", "error.c"), os.path.join("src", "platform.c"),
               os.path.join("src", "parser.c"), os.path.join("src", "tokenizer.c"))

    if self.procs == 1:
      self.targets = [source for source in sources]

    else:
      self.targets = [list() for _ in range(procs)]

      i = 0
      for source in sources:
        self.targets[i].append(source)
        i += 1
        if i > len(self.targets)-1: i = 0

  # Compile on only one process
  def _compile_1proc(self):
    start_time = time.perf_counter()
    os.system(f"gcc -o dust {' '.join(self.targets)} -I./include/ {OPTIONS}")
    return time.perf_counter() - start_time

  # Compile on multiple processes
  def _compile_multiproc(self):
    remove_object_files()
    subprocs = []

    start_time = time.perf_counter()
    for sources in self.targets:
      subprocs.append(subprocess.Popen(("gcc", "-c", *sources, "-I./include/")))

    for s in subprocs:
      s.communicate()

    os.system(f"gcc -o dust cli.o error.o parser.o platform.o tokenizer.o u8string.o {OPTIONS}")
    end_time = time.perf_counter() - start_time
    remove_object_files()

    return end_time

  # Start compiling
  def compile(self):
    if os.path.exists(FINAL_BUILD): os.remove(FINAL_BUILD)

    if self.procs == 1:
      return self._compile_1proc()
    else:
      return self._compile_multiproc()
    

if __name__ == "__main__":
  if   "-core1" in sys.argv: CORES = 1
  elif "-core2" in sys.argv: CORES = 2
  elif "-core3" in sys.argv: CORES = 3
  elif "-core4" in sys.argv: CORES = 4
  elif "-core5" in sys.argv: CORES = 5
  elif "-core6" in sys.argv: CORES = 6
  else:                      CORES = CPU_COUNT

  if   "-O0" in sys.argv: OPTIMIZATION_LEVEL = 0
  elif "-O1" in sys.argv: OPTIMIZATION_LEVEL = 1
  elif "-O2" in sys.argv: OPTIMIZATION_LEVEL = 2
  elif "-O3" in sys.argv: OPTIMIZATION_LEVEL = 3

  add_option(("-O0", "-O1", "-O2", "-O3")[OPTIMIZATION_LEVEL])

  print(f"Started compiling on {CORES} cores\n" +
        f"(default is {CPU_COUNT}, use -corex option to change it)\n" +
        f"Optimization level: {OPTIMIZATION_LEVEL}\n")

  compiler = Compiler(CORES)

  end_time = compiler.compile()

  print(f"Dust succesfully built in {round(end_time, 1)} secs ({int(end_time*1000)} ms)")