"""

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust


  Dust script to help building
  ----------------------------

  Script assumes MinGW binaries are on PATH as a prerequisite.
  Just simply run the script with 'python build.py', (or 'py',
  'python3' depending on the platform.)

  By default, all cores of the machine are used to reduce compile
  time. You can use '-core' flag to adjust number of cores used
  manually. For example, 'python build.py -core1' uses only one
  core, '-core2' uses 2 core, '-core4' uses 4 core and suchlike.

  You can use flags '-O' flag to set compiler optimization.
  Compilation time increases as the optimization level go higher.
    -O0  -  Don't optimize (default)
    -O1  -  Optimize moderately
    -O2  -  Optimize more
    -O3  -  Optimize even more

"""

import os
import sys
import platform
import time
import multiprocessing
import subprocess


GH_REPO = "https://github.com/kadir014/Dust"
CPU_COUNT = multiprocessing.cpu_count()
FINAL_BUILD = "dust.exe" if platform.system() == "Windows" else "dust"

class ValidityError(Exception): pass
class OptionError(Exception): pass


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
  
  raise ValidityError("Could not verify source files" + \
                      "Try cloning the repo again from " + GH_REPO)


# Remove all object (.o) files from the previous compilation
def remove_object_files():
  for file in os.listdir(os.getcwd()):
    if file.endswith(".o"):
      os.remove(file)


# Organizes command line arguments and compiler options to
# simplify building
class OptionHandler:
  def __init__(self):
    self.cores        = CPU_COUNT # -corex
    self.optimization = 0         # -Ox
    self.gcc_args = []            # GCC arguments

    if platform.system() == "Windows": self.gcc_args.append("-lws2_32")

  def add_option(self, opt):
    if opt.startswith("-core"):
      try:
        self.cores = int(opt[5:])

      except ValueError:
        raise OptionError(f"integer expected after -core flag (-core0, -core4, etc..)")

    elif opt.startswith("-O"):
      try:
        self.optimization = int(opt[2:])
        if not (0 <= self.optimization <= 3):
          raise OptionError("invalid optimization level")

      except ValueError:
        raise OptionError(f"integer expected after -O flag (-O0, -O2, etc..)")

    else:
      raise OptionError(f"unknown option '{opt}'")

  def get_gcc_argstr(self):
    result = " " + " ".join(self.gcc_args)

    result += f" -O{self.optimization}"

    return result


class Compiler:
  def __init__(self, option_handler):
    self.option_handler = option_handler

    sources = (os.path.join("src", "cli.c"), os.path.join("src", "u8string.c"),
               os.path.join("src", "error.c"), os.path.join("src", "platform.c"),
               os.path.join("src", "parser.c"), os.path.join("src", "tokenizer.c"))

    if option_handler.cores == 1:
      self.targets = [source for source in sources]

    else:
      self.targets = [list() for _ in range(option_handler.cores)]

      i = 0
      for source in sources:
        self.targets[i].append(source)
        i += 1
        if i > len(self.targets)-1: i = 0

  # Compile on only one process
  def _compile_1proc(self):
    start_time = time.perf_counter()
    os.system(f"gcc -o dust {' '.join(self.targets)} -I./include/ {self.option_handler.get_gcc_argstr()}")
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

    # Link all object files to finish compiling
    os.system(f"gcc -o dust cli.o error.o parser.o platform.o tokenizer.o u8string.o {self.option_handler.get_gcc_argstr()}")
    end_time = time.perf_counter() - start_time
    remove_object_files()

    return end_time

  # Start compiling
  def compile(self):
    if os.path.exists(FINAL_BUILD): os.remove(FINAL_BUILD)

    if self.option_handler.cores == 1:
      return self._compile_1proc()
    else:
      return self._compile_multiproc()
    

if __name__ == "__main__":
  option_handler = OptionHandler()

  for arg in sys.argv[1:]:
    option_handler.add_option(arg)

  if option_handler.cores > CPU_COUNT:
    print(f"warning: given core count ({option_handler.cores})" + \
          f" is higher than your machine's core count ({CPU_COUNT})\n")

  print(f"Started compiling on {option_handler.cores} cores\n" +
        f"Optimization level: {option_handler.optimization}\n")

  compiler = Compiler(option_handler)

  end_time = compiler.compile()

  print(f"Dust succesfully built in {round(end_time, 1)} secs ({int(end_time*1000)} ms)")