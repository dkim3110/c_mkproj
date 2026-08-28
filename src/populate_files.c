#include "mkproj.h"

void populate_main(FILE *fptr) {
  fprintf(fptr,
    "#include <stdio.h>\n"
    "\n"
    "int main(int argc, char *argv[]) {\n"
    "  printf(\"Hello, World!\");\n"
    "\n"
    "  return 0;\n"
    "}\n");
}

void populate_md(FILE *fptr, md_type_t file_type) {
  switch (file_type) {
    case README:
      fprintf(fptr,
        "# Project Title\n"
        "\n"
        "Simple overview of use/purpose.\n"
        "\n"
        "## Description\n"
        "\n"
        "An in-depth paragraph about your project and overview of use.\n"
        "\n"
        "## Getting Started\n"
        "\n"
        "### Dependencies\n"
        "\n"
        "* Describe any prerequisites, libraries, OS version, etc., needed before installing program.\n"
        "\n"
        "### Installing\n"
        "\n"
        "* How/where to download your program\n"
        "* Any modifications needed to be made to files/folders\n"
        "\n"
        "### Executing program\n"
        "\n"
        "* How to run the program\n"
        "* Step-by-step bullets\n"
        "```\n"
        "code blocks for commands\n"
        "```\n"
        "\n"
        "## Help\n"
        "\n"
        "Any advise for common problems or issues.\n"
        "```\n"
        "command to run if program contains helper info\n"
        "```\n"
        "\n"
        "## Authors\n"
        "\n"
        "Contributors names and contact info\n"
        "\n"
        "## License\n"
        "\n"
        "This project is licensed under the [NAME HERE] License - see the LICENSE.md file for details\n"
        "\n"
        "## Acknowledgments\n"
        "\n"
        "Inspiration, code snippets, etc\n"
      );
      break;
    case LICENSE:
      fprintf(fptr,
        "MIT License\n"
        "\n"
        "Copyright (c) %c%c%c%c [Your Name]\n"
        "\n"
        "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
        "of this software and associated documentation files (the \"Software\"), to deal\n"
        "in the Software without restriction, including without limitation the rights\n"
        "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
        "copies of the Software, and to permit persons to whom the Software is\n"
        "furnished to do so, subject to the following conditions:\n"
        "\n"
        "The above copyright notice and this permission notice shall be included in all\n"
        "copies or substantial portions of the Software.\n"
        "\n"
        "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
        "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
        "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
        "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
        "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
        "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
        "SOFTWARE.\n",
        __DATE__[7], __DATE__[8], __DATE__[9], __DATE__[10] 
      );
      break;
    default:
      fprintf(stderr, "-error: failed to fill %s", (file_type == README) ? "README.md" : "LICENSE.md");
      break;
  }
}

void populate_makefile(FILE *fptr, file_mode_t mode) {
  switch (mode) {
    case FULL:
      fprintf(fptr,
        "CC := gcc\n"
        "SRCDIR := src\n"
        "BUILDDIR := build\n"
        "TARGET := bin/run\n"
        "SRCEXT := c\n"
        "SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')\n"
        "OBJECTS := $(patsubst $(SRCDIR)/%%,$(BUILDDIR)/%%,$(SOURCES:.$(SRCEXT)=.o))\n"
        "DEPS := $(OBJECTS:.o=.d)\n"
        "CFLAGS := -O0 -Wall -Wextra -Werror\n"
        "LIB := -L lib\n"
        "INC := -I include\n"
        "\n"
        "all: $(TARGET)\n"
        "\n"
        "$(TARGET): $(OBJECTS)\n"
        "\t@echo \" Linking...\"\n"
        "\t@mkdir -p $(dir $@)\n"
        "\t@$(CC) $^ -o $@ $(LIB)\n"
        "\n"
        "$(BUILDDIR)/%%.o: $(SRCDIR)/%%.$(SRCEXT)\n"
        "\t@echo \" Building...\"\n"
        "\t@mkdir -p $(dir $@)\n"
        "\t@$(CC) $(CFLAGS) $(INC) -MMD -MP -c -o $@ $<\n"
        "\n"
        "debug:\n"
        "\t@$(MAKE) clean\n"
        "\t@$(MAKE) CFLAGS=\"$(CFLAGS) -g\" all\n"
        "\n"
        "clean:\n"
        "\t@echo \" Cleaning...\"\n"
        "\t@$(RM) -r $(BUILDDIR) $(TARGET)\n"
        "\n"
        "-include $(DEPS)\n"
        "\n"
        "run: all\n"
        "\t@echo \" Running $(TARGET)...\"\n"
        "\t@./$(TARGET)\n"
        "\n"
        ".PHONY: all clean debug run\n"
      );
      break;
    case DEFAULT:
      fprintf(fptr,
        "CC := gcc\n"
        "SRCDIR := src\n"
        "BUILDDIR := build\n"
        "TARGET := run\n"
        "SRCEXT := c\n"
        "SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')\n"
        "OBJECTS := $(patsubst $(SRCDIR)/%%,$(BUILDDIR)/%%,$(SOURCES:.$(SRCEXT)=.o))\n"
        "DEPS := $(OBJECTS:.o=.d)\n"
        "CFLAGS := -O0 -Wall -Wextra -Werror\n"
        "INC := -I include\n"
        "\n"
        "all: $(TARGET)\n"
        "\n"
        "$(TARGET): $(OBJECTS)\n"
        "\t@echo \" Linking...\"\n"
        "\t@$(CC) $^ -o $@\n"
        "\n"
        "$(BUILDDIR)/%%.o: $(SRCDIR)/%%.$(SRCEXT)\n"
        "\t@echo \" Building...\"\n"
        "\t@mkdir -p $(dir $@)\n"
        "\t@$(CC) $(CFLAGS) $(INC) -MMD -MP -c -o $@ $<\n"
        "\n"
        "debug:\n"
        "\t@$(MAKE) clean\n"
        "\t@$(MAKE) CFLAGS=\"$(CFLAGS) -g\" all\n"
        "\n"
        "clean:\n"
        "\t@echo \" Cleaning...\"\n"
        "\t@$(RM) -r $(BUILDDIR) $(TARGET)\n"
        "\n"
        "-include $(DEPS)\n"
        "\n"
        "run: all\n"
        "\t@echo \" Running $(TARGET)...\"\n"
        "\t@./$(TARGET)\n"
        "\n"
        ".PHONY: all clean debug run\n"
      );
    default:
      break;
  }
}
