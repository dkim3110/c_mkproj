# `mkproj` | Automatic Project Maker

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A lightweight command-line utility to generate C project scaffolding.

## Description

`mkproj` is a lightweight, zero-dependency command-line utility written in C that automates the scaffolding of new C programming projects. It offers different initialization modes ranging from a barebones setup to a fully structured layout, alongside an auto-dependency tracking `Makefile`.

Compatible with Linux and Windows (via MSYS2/MinGW or WSL)

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

# For Windows, move the compiled 'mkproj.exe' to a directory included in your system's 'PATH', or simply run as '.\mkproj project'
# If using MSYS2/MinGW, run the above without 'sudo'
```

## Usage

- Navigate to the directory where you want to create your new project.
- Run the program using your desired project name, and an optional mode flag.
  - Note that project names cannot begin with a hyphen (`-`) to prevent conflicts with flag parsing.

### Flags

You can set flags by including it in the arguments. Order doesn't matter, but only accepts one flag.

---
**BARE:** a minimal setup for quick scripts and tests.
```
mkproj project_name --bare
```
```Makefile
project_name/
├── Makefile
├── main.c
└── README.md
```
---
**DEFAULT:** creates a standard project structure.
```
mkproj project_name [--default]
```
```Makefile
project_name/
├── build/
├── include/
├── src/
│   └── main.c
├── Makefile
└── README.md
```
---
**PLUS:** includes additional directories to `--default` for larger projects.
```
mkproj project_name --plus
```
```Makefile
project_name/
├── bin/
├── build/
├── include/
├── lib/
├── src/
│   └── main.c
├── tests
│   └── test_main.c
├── Makefile
└── README.md
```
---
**FULL:** creates a modified version of [Jack Wetherell](https://github.com/JackWetherell)'s [c-project-structure](https://github.com/JackWetherell/c-project-structure.git)
```
mkproj project_name --full
```
```Makefile
project_name/
├── bin/
├── build/
├── data/
│   ├── raw/
│   ├── interim/
│   ├── input/
│   └── output/
├── docs/
├── include/
├── lib/
├── src/
│   └── main.c
├── tests/
│   └── test_main.c
├── .gitignore
├── Makefile
└── README.md
```
---
You can use `--help` to see the exact folder structures whenever you want.
```
mkproj --help
```

### Makefile

All templates come with a `Makefile`. Running `make` creates a binary named `run` (or `bin/run` if the template has a `bin` directory). You can change the name by editing the `TARGET` variable. Additionally, any `.h` files included in `.c` files are automatically tracked using `-MMD -MP`.

**Commands**

```Makefile
make
# Builds 'run' ('bin/run') from 'src/*.c'.

make clean
# Cleans 'build/' and removes binary.

make debug
# Runs 'make clean', then rebuilds with '-g'.

make test
# Builds 'bin/tester' from 'tests/*.c', links it against the library objects (everything in 'build/' except 'main.o'), and runs it.
# Only available on projects made with '--plus' or beyond.
```


## Credits

See [CREDITS.md](CREDITS.md)

## License

**MIT** - see [LICENSE.md](LICENSE.md).

## Acknowledgments

- Inspired by [Jack Wetherell](https://github.com/JackWetherell)'s [c-project-structure](https://github.com/JackWetherell/c-project-structure.git).
- README template by [DomPizzie](https://github.com/DomPizzie). You can see it [here](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc).
