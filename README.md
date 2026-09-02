# `mkproj` | Automatic Project Maker

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A lightweight command-line utility to generate C project scaffolding.

## Description

`mkproj` is a lightweight, zero-dependency command-line utility written in C that automates the scaffolding of new C programming projects. It offers different initialization modes ranging from a barebones setup to a fully structured layout, alongside an auto-dependency tracking `Makefile`.

Compatible with Linux and Windows (via MSYS2/MinGW or WSL).

*`mkproj` was originally made for personal use and is presented as is.*

## Requirements

- GCC
  - If you have Clang, override with `make CC=clang`. 
- GNU Make

## Installation

1. Build
```bash
git clone https://github.com/dkim3110/c_mkproj.git
cd c_mkproj
make
```

2. Deploy
```bash
# To uninstall, run 'sudo make uninstall'
sudo make install

# For Windows, move the compiled 'mkproj.exe' to a directory included in your system's 'PATH',
# or simply run as '.\mkproj'

# If using MSYS2/MinGW, run the above without 'sudo'
```

## Usage

- Navigate to the directory where you want to create your new project.
- Run the program using your desired project name, and an optional mode flag.
  - Note that project names cannot begin with a hyphen (`-`) to prevent conflicts with flag parsing.
 
### Example

### Flags

You can set flags by including it in the arguments. Order doesn't matter, but it will only accept one flag.

---
**BARE:** a minimal setup for quick scripts and tests.
```
-b, --bare
```
```bash
$ mkproj project_name --bare
$ tree -a project_name
project_name
├── Makefile
├── README.md
└── main.c

1 directory, 3 files
```
---
**DEFAULT:** a standard project structure.
```
-d, --default, [no flags]
```
```bash
$ mkproj project_name
$ tree -a project_name
project_name
├── Makefile
├── README.md
├── build
├── include
└── src
    └── main.c

4 directories, 3 files
```
---
**PLUS:** includes additional directories to `--default` for larger projects.
```
-p, --plus
```
```bash
$ mkproj project_name --plus
$ tree -a project_name
project_name
├── Makefile
├── README.md
├── bin
├── build
├── include
├── lib
├── src
│   └── main.c
└── tests
    └── test_main.c

7 directories, 4 files
```
---
**FULL:** a modified version of [Jack Wetherell](https://github.com/JackWetherell)'s [c-project-structure](https://github.com/JackWetherell/c-project-structure.git)
```
-f, --full
```
```bash
$ mkproj project_name --full
$ tree -a project_name
project_name
├── .gitignore
├── Makefile
├── README.md
├── bin
├── build
├── data
│   ├── input
│   ├── interim
│   ├── output
│   └── raw
├── docs
├── include
├── lib
├── src
│   └── main.c
└── tests
    └── test_main.c

13 directories, 5 files
```
---
You can use `-h` or `--help` to see the exact folder structures whenever you want.
```
$ mkproj --help

USAGE:
 mkproj [flag] project_name
 mkproj project_name [flag]

OPTIONS:
 -h, --help
                Prints this help message; overrides all arguments

 -b, --bare
                Creates project in bare mode:
                        project_name
                        |--main.c
                        |--Makefile
                        `--README.md

 -d, --default, [no flags]
                Creates project in default mode:
                        project_name
                        |--build
                        |--include
                        |--src
                        |  `--main.c
                        |--Makefile
                        `--README.md

 -p, --plus
                Creates project in plus mode:
                        project_name
                        |--bin
                        |--build
                        |--include
                        |--lib
                        |--src
                        |  `--main.c
                        |--tests
                        |  `--test_main.c
                        |--Makefile
                        `--README.md

 -f, --full
                Creates project in full mode:
                        project_name
                        |--bin
                        |--build
                        |--data
                        |  `--raw
                        |  `--interim
                        |  `--input
                        |  `--output
                        |--docs
                        |--include
                        |--lib
                        |--src
                        |  `--main.c
                        |--tests
                        |  `--test_main.c
                        |--.gitignore
                        |--Makefile
                        `--README.md
```

### Makefile

All templates come with a dedicated `Makefile`. Running `make` creates a binary named `run` (or `bin/run` if the template has a `bin` directory). You can change the name by editing the `TARGET` variable. Additionally, any `.h` files included in `.c` files are automatically tracked using `-MMD -MP`.

**Commands**

```Makefile
make
# Builds program executable.

make clean
# Cleans 'build/' if it exists and removes executable.

make debug
# Runs 'make clean', then rebuilds with '-g'.

make test
# Builds 'bin/tester' from 'tests/*.c', links it against the library objects (everything in 'build/' except 'main.o'),
# and runs it. Only available on templates '--plus' and beyond.
```

## Credits

See [CREDITS.md](CREDITS.md)

## License

**MIT** - see [LICENSE.md](LICENSE.md).

## Acknowledgments

- Inspired by [Jack Wetherell](https://github.com/JackWetherell)'s [c-project-structure](https://github.com/JackWetherell/c-project-structure.git).
- README template by [DomPizzie](https://github.com/DomPizzie). You can see it [here](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc).
