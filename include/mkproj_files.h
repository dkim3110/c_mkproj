#ifndef MKPROJ_FILES_H_
#define MKPROJ_FILES_H_

// == MAKEFILE =============================================================
#define MAKEFILE_BARE                                                          \
  "CC := gcc\n"                                                                \
  "TARGET := run\n"                                                            \
  "SRCEXT := c\n"                                                              \
  "CFLAGS := -O0 -Wall -Wextra -Werror\n"                                      \
  "\n"                                                                         \
  "all: $(TARGET)\n"                                                           \
  "\n"                                                                         \
  "$(TARGET): *.$(SRCEXT)\n"                                                   \
  "\t@echo \" Building...\"\n"                                                 \
  "\t@$(CC) $(CFLAGS) $^ -o $@\n"                                              \
  "\n"                                                                         \
  "debug:\n"                                                                   \
  "\t@$(MAKE) clean\n"                                                         \
  "\t@$(MAKE) CFLAGS=\"$(CFLAGS) -g\" all\n"                                   \
  "\n"                                                                         \
  "clean:\n"                                                                   \
  "\t@echo \" Cleaning...\"\n"                                                 \
  "\t@$(RM) $(TARGET)\n"                                                       \
  "\n"                                                                         \
  ".PHONY: all clean debug\n"

#define MAKEFILE_DEFAULT                                                       \
  "CC := gcc\n"                                                                \
  "SRCDIR := src\n"                                                            \
  "BUILDDIR := build\n"                                                        \
  "TARGET := run\n"                                                            \
  "SRCEXT := c\n"                                                              \
  "SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')\n"           \
  "OBJECTS := $(patsubst "                                                     \
  "$(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))\n"                      \
  "DEPS := $(OBJECTS:.o=.d)\n"                                                 \
  "CFLAGS := -O0 -Wall -Wextra -Werror\n"                                      \
  "INC := -I include\n"                                                        \
  "\n"                                                                         \
  "all: $(TARGET)\n"                                                           \
  "\n"                                                                         \
  "$(TARGET): $(OBJECTS)\n"                                                    \
  "\t@echo \" Linking...\"\n"                                                  \
  "\t@$(CC) $^ -o $@\n"                                                        \
  "\n"                                                                         \
  "$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)\n"                                   \
  "\t@echo \" Building...\"\n"                                                 \
  "\t@mkdir -p $(dir $@)\n"                                                    \
  "\t@$(CC) $(CFLAGS) $(INC) -MMD -MP -c $< -o $@\n"                           \
  "\n"                                                                         \
  "debug:\n"                                                                   \
  "\t@$(MAKE) clean\n"                                                         \
  "\t@$(MAKE) CFLAGS=\"$(CFLAGS) -g\" all\n"                                   \
  "\n"                                                                         \
  "clean:\n"                                                                   \
  "\t@echo \" Cleaning...\"\n"                                                 \
  "\t@find $(BUILDDIR) -type f -delete\n"                                      \
  "\t@$(RM) $(TARGET)\n"                                                       \
  "\n"                                                                         \
  "-include $(DEPS)\n"                                                         \
  "\n"                                                                         \
  ".PHONY: all clean debug\n"

#define MAKEFILE_FULL                                                          \
  "CC := gcc\n"                                                                \
  "SRCDIR := src\n"                                                            \
  "BUILDDIR := build\n"                                                        \
  "TESTDIR := tests\n"                                                         \
  "TARGET := bin/run\n"                                                        \
  "TESTTARGET := bin/tester\n"                                                 \
  "SRCEXT := c\n"                                                              \
  "SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')\n"           \
  "TESTSOURCES := $(wildcard $(TESTDIR)/"                                      \
  "*.$(SRCEXT))\n"                                                             \
  "OBJECTS := $(patsubst "                                                     \
  "$(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))\n"                      \
  "DEPS := $(OBJECTS:.o=.d)\n"                                                 \
  "LIBOBJECTS := $(filter-out $(BUILDDIR)/main.o,$(OBJECTS))\n"                \
  "CFLAGS := -O0 -Wall -Wextra -Werror\n"                                      \
  "LIB := -L lib\n"                                                            \
  "INC := -I include\n"                                                        \
  "\n"                                                                         \
  "all: $(TARGET)\n"                                                           \
  "\n"                                                                         \
  "$(TARGET): $(OBJECTS)\n"                                                    \
  "\t@echo \" Linking...\"\n"                                                  \
  "\t@mkdir -p $(dir $@)\n"                                                    \
  "\t@$(CC) $^ -o $@ $(LIB)\n"                                                 \
  "\n"                                                                         \
  "$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)\n"                                   \
  "\t@echo \" Building...\"\n"                                                 \
  "\t@mkdir -p $(dir $@)\n"                                                    \
  "\t@$(CC) $(CFLAGS) $(INC) -MMD -MP -c $< -o $@\n"                           \
  "\n"                                                                         \
  "debug:\n"                                                                   \
  "\t@$(MAKE) clean\n"                                                         \
  "\t@$(MAKE) CFLAGS=\"$(CFLAGS) -g\" all\n"                                   \
  "\n"                                                                         \
  "clean:\n"                                                                   \
  "\t@echo \" Cleaning...\"\n"                                                 \
  "\t@find $(BUILDDIR) -type f -delete\n"                                      \
  "\t@$(RM) $(TARGET) $(TESTTARGET)\n"                                         \
  "\n"                                                                         \
  "$(TESTTARGET): $(TESTSOURCES) $(LIBOBJECTS)\n"                              \
  "\t@echo \" Building tests...\"\n"                                           \
  "\t@echo \" Running tests...\"\n"                                            \
  "\t@mkdir -p $(dir $@)\n"                                                    \
  "\t@$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LIB)\n"                                \
  "\n"                                                                         \
  "test: $(TESTTARGET)\n"                                                      \
  "\t@./$(TESTTARGET)\n"                                                       \
  "\t@echo \"\""                                                               \
  "\n"                                                                         \
  "-include $(DEPS)\n"                                                         \
  "\n"                                                                         \
  ".PHONY: all clean debug test\n"
// ============================================================= MAKEFILE ==

// == TEXT FILE ============================================================
#define README_CONTENTS                                                        \
  "# Project Title\n"                                                          \
  "\n"                                                                         \
  "Simple overview of use/purpose.\n"                                          \
  "\n"                                                                         \
  "## Description\n"                                                           \
  "\n"                                                                         \
  "An in-depth paragraph about your project and overview of use.\n"            \
  "\n"                                                                         \
  "## Getting Started\n"                                                       \
  "\n"                                                                         \
  "### Dependencies\n"                                                         \
  "\n"                                                                         \
  "* Describe any prerequisites, libraries, OS version, etc., needed before "  \
  "installing program.\n"                                                      \
  "\n"                                                                         \
  "### Installing\n"                                                           \
  "\n"                                                                         \
  "* How/where to download your program\n"                                     \
  "* Any modifications needed to be made to files/folders\n"                   \
  "\n"                                                                         \
  "### Executing program\n"                                                    \
  "\n"                                                                         \
  "* How to run the program\n"                                                 \
  "* Step-by-step bullets\n"                                                   \
  "```\n"                                                                      \
  "code blocks for commands\n"                                                 \
  "```\n"                                                                      \
  "\n"                                                                         \
  "## Help\n"                                                                  \
  "\n"                                                                         \
  "Any advise for common problems or issues.\n"                                \
  "```\n"                                                                      \
  "command to run if program contains helper info\n"                           \
  "```\n"                                                                      \
  "\n"                                                                         \
  "## Authors\n"                                                               \
  "\n"                                                                         \
  "Contributors names and contact info\n"                                      \
  "\n"                                                                         \
  "## License\n"                                                               \
  "\n"                                                                         \
  "This project is licensed under the [NAME HERE] License - see the "          \
  "LICENSE.md file for details\n"                                              \
  "\n"                                                                         \
  "## Acknowledgments\n"                                                       \
  "\n"                                                                         \
  "Inspiration, code snippets, etc\n"                                          \
  "* README template by [DomPizzie](https://gist.github.com/DomPizzie). You "  \
  "can see it "                                                                \
  "[here](https://gist.github.com/DomPizzie/"                                  \
  "7a5ff55ffa9081f2de27c315f5018afc)."

#define GITIGNORE_CONTENTS                                                     \
  "# Ignore the build and lib dirs\n"                                          \
  "build/"                                                                     \
  "*\n"                                                                        \
  "lib/"                                                                       \
  "*\n"                                                                        \
  "\n"                                                                         \
  "# Ignore any executables\n"                                                 \
  "bin/"                                                                       \
  "*\n"                                                                        \
  "\n"                                                                         \
  "# Ignore intermediates from -save-temps and dependency files from -MMD\n"   \
  "*.i\n"                                                                      \
  "*.s\n"                                                                      \
  "*.o\n"                                                                      \
  "*.d\n"
// ============================================================ TEXT FILE ==

// == C FILE ===============================================================
#define MAIN_C_CONTENTS                                                        \
  "#include <stdio.h>\n"                                                       \
  "\n"                                                                         \
  "int main(/"                                                                 \
  "* int argc, char *argv[] */) {\n"                                           \
  "\tprintf(\"Hello, World!\\n\");"                                            \
  "\n"                                                                         \
  "\treturn 0;\n"                                                              \
  "}\n"

#define TEST_MAIN_C_CONTENTS                                                   \
  "#include <stdio.h>\n"                                                       \
  "\n"                                                                         \
  "int main(/"                                                                 \
  "* int argc, char *argv[] */) {\n"                                           \
  "\tprintf(\"This is a test!\\n\");"                                          \
  "\n"                                                                         \
  "\treturn 0;\n"                                                              \
  "}\n"
// =============================================================== C FILE ==

#endif // MKPROJ_FILES_H_
