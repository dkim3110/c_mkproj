# `mkproj` | Automatic Project Maker

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A lightweight command-line utility to generate C project scaffolding.

## Description

`mkproj` is a lightweight, zero-dependency command-line utility written in C that automates the scaffolding of new C programming projects. It offers different initialization modes ranging from a barebones setup to a fully structured layout with `src`, `build`, `include`, and `lib` directories, alongside an auto-dependency tracking `Makefile`.

Compatible with Linux and Windows (via MSYS2/MinGW or WSL)

## Requirements

- `GCC`
  - If you have Clang, override with `make CC=clang`. 
- GNU Make (to build this tool and to use the generated project Makefiles)

## Installation

1. Build (Linux and Windows)
```bash
git clone https://github.com/dkim3110/c_mkproj.git
cd c_mkproj
make
```

2. Deploy (Linux)
```bash
sudo make install
# To uninstall, run 'sudo make uninstall'
```

*For Windows, move the compiled `mkproj.exe` to a directory included in your system's `PATH`. If using MSYS2/MinGW, simply ditch `sudo` and run the above.*

## Usage

- Navigate to the directory where you want to create your new project.
- Run the program using your desired project name, and an optional mode flag.
  - Note that the name of the project cannot start with a `-`.

### Flags

**Bare:** a minimal setup for quick scripts and tests.
```
mkproj project_name --bare
```
```
project_name/
├── LICENSE.md
├── main.c
└── README.md
```
---
**Default:** creates a standard project structure.
```
mkproj project_name [--default]
```
```
project_name/
├── build/
├── include/
├── src/
│   └── main.c
├── LICENSE.md
├── Makefile
└── README.md
```
---
**Full:** includes additional `bin` and `lib` directories to `--default` for larger, more complex projects.
```
mkproj project_name --full
```
```
project_name/
├── bin/
├── build/
├── include/
├── lib/
├── src/
│   └── main.c
├── LICENSE.md
├── Makefile
└── README.md
```
---
You can use `--help` to see the exact folder structures whenever you want.
```
mkproj --help
```

### Makefile

Both `--default` and `--full` will generate a Makefile. Running `make` creates a binary named `run` in `--default`, or `bin/run` in `--full`.

## License

**MIT** - see [LICENSE.md](LICENSE.md).

## Acknowledgments

- Inspired by [JackWetherel](https://github.com/JackWetherell)'s [c-project-structure](https://github.com/JackWetherell/c-project-structure.git).
- README template by [DomPizzie](https://gist.github.com/DomPizzie). You can see it [here](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc).
