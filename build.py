"""

  This file is a part of the Dust Programming Language
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/Dust


  Dust builder script
  ----------------------------
  This Python script is used to build (compile) Dust and
  distribute cross-platform packages.
  This script must be run in the same directory as Dust.

  a) Building Dust
  ----------------------------
  Script assumes MinGW binaries are on PATH as a prerequisite.
  Just simply run the script with 'python build.py', (or 'py',
  'python3' depending on the platform.)

  By default, all cores of the machine are used to reduce compile
  time. You can use '-j' flag to adjust number of cores used
  manually. For example, 'python build.py -j1' uses only one
  core, '-j2' uses 2 core, '-j4' uses 4 core and suchlike.

  You can use flags '-O' flag to set compiler optimization.
  Compilation time increases as the optimization level go higher.
    -O0  -  Don't optimize (default)
    -O1  -  Optimize moderately
    -O2  -  Optimize more
    -O3  -  Optimize even more

  All files created during building are cleaned afterwards by
  default, but one can use `--clean` flag to remove them manually.

  b) Distributing Dust
  ----------------------------
  Use '--package' flag to distribute Dust package.
  Any dependency will be downloaded on the fly.

  Optional flags are:
    --package-dist   -  Just archive Dust
    --package-clean  -  Clean remaining files from previous
                        packaging (This shouldn't be used
                        normally because all extra files are
                        removed by default.)

"""

import os
import sys
import platform
import pathlib
import time
import multiprocessing
import subprocess


GH_REPO = "https://github.com/kadir014/Dust"
CPU_COUNT = multiprocessing.cpu_count()
FINAL_BUILD = "dust.exe" if platform.system() == "Windows" else "dust"

DUST_PATH = pathlib.Path(os.getcwd())

SOURCE_FILES = [
    DUST_PATH / "src" / "cli.c",
    DUST_PATH / "src" / "tokenizer.c",
    DUST_PATH / "src" / "ustring.c",
    DUST_PATH / "src" / "error.c",
    DUST_PATH / "src" / "parser.c",
    DUST_PATH / "src" / "platform.c"
]

INCLUDE_FILES = [
    DUST_PATH / "include" / "dust" / "tokenizer.h",
    DUST_PATH / "include" / "dust" / "ustring.h",
    DUST_PATH / "include" / "dust" / "error.h",
    DUST_PATH / "include" / "dust" / "ansi.h",
    DUST_PATH / "include" / "dust" / "parser.h",
    DUST_PATH / "include" / "dust" / "platform.h"
]

class ValidityError(Exception): pass
class OptionError(Exception): pass


# Remove all object (.o) files from the previous compilation
def remove_object_files():
    for file in os.listdir(os.getcwd()):
        if file.endswith(".o"):
            os.remove(file)


class OptionHandler:
    """
    Organizes command line arguments and compiler options to simplify building
    """
    def __init__(self):
        self.cores        = CPU_COUNT # -jx
        self.optimization = 0         # -Ox
        self.clean        = False     # --clean
        
        self.package       = False # --package
        self.package_dist  = False # --package-dist
        self.package_clean = False

        self.gcc_args = [] # GCC arguments

        if platform.system() == "Windows": self.gcc_args.append("-lws2_32")
        else: self.gcc_args.append("-lm")

    def add_option(self, opt: str):
        if opt.startswith("-j"):
            try:
                self.cores = int(opt[2:])

            except ValueError:
                raise OptionError(f"integer expected after -j flag (-j0, -j4, etc..)")

        elif opt.startswith("-O"):
            try:
                self.optimization = int(opt[2:])
                if not (0 <= self.optimization <= 3):
                    raise OptionError("invalid optimization level")

            except ValueError:
                raise OptionError(f"integer expected after -O flag (-O0, -O2, etc..)")

        elif opt.startswith("--clean"):
            self.clean = True

        elif opt.startswith("--package"):
            self.package = True

            if opt == "--package-dist":
                self.package_dist = True

        elif opt == "--package-clean":
            self.package_clean = True

        else:
            raise OptionError(f"unknown option '{opt}'")

    def get_gcc_argstr(self):
        argstr = f" {' '.join(self.gcc_args)} -O{self.optimization}"
        return argstr


class Compiler:
    """
    Compiles Dust source code
    """
    def __init__(self, option_handler: OptionHandler):
        self.option_handler = option_handler

        if option_handler.cores == 1:
            self.targets = [str(source) for source in SOURCE_FILES]

        else:
            self.targets = [list() for _ in range(option_handler.cores)]

            i = 0
            for source in SOURCE_FILES:
                self.targets[i].append(str(source))
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
        os.system(f"gcc -o dust tokenizer.o ustring.o {self.option_handler.get_gcc_argstr()}")
    
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

    # Clean build remainings
    if option_handler.clean:
        remove_object_files()
        print("Succesfully cleaned building remaining files.")
        sys.exit(0)

    # Clean packaging remainings
    if option_handler.package_clean:
        print("Succesfully cleaned packaging remaining files.")
        sys.exit(0)

    # Package Dust
    # TODO
    if option_handler.package:
        pass

    # Build Dust
    else:
        if option_handler.cores > CPU_COUNT:
            print(f"warning: given core count ({option_handler.cores})" + \
                  f" is higher than your machine's core count ({CPU_COUNT})\n")

        print(f"Started compiling on {option_handler.cores} cores\n" +
            f"Optimization level: {option_handler.optimization}\n")

        compiler = Compiler(option_handler)

        end_time = compiler.compile()

        print(f"Dust succesfully built in {round(end_time, 1)} secs ({int(end_time*1000)} ms)")