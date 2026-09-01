# Based on original Makefile by Jack Wetherell, licensed under MIT. Modified by Daniel Inhoi Kim.

CC := gcc
SRCDIR := src
BUILDDIR := build
TARGET := mkproj
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPS := $(OBJECTS:.o=.d)
CFLAGS := -O1 -Wall -Wextra -Werror
INC := -I include
PREFIX ?= /usr/local

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@$(CC) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " Building..."
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -MMD -MP $< -c -o $@

debug:
	@$(MAKE) clean
	@$(MAKE) CFLAGS="$(CFLAGS) -g" all

clean:
	@echo " Cleaning..."
	@find $(BUILDDIR) -type f -delete
	@$(RM) $(TARGET)

install: $(TARGET)
	@echo " Installing to $(PREFIX)/bin..."
	@mkdir -p $(PREFIX)/bin
	@install -m 755 $(TARGET) $(PREFIX)/bin/$(TARGET)

uninstall:
	@echo " Uninstalling from $(PREFIX)/bin..."
	@$(RM) $(PREFIX)/bin/$(TARGET)

-include $(DEPS)

.PHONY: all clean debug install uninstall
