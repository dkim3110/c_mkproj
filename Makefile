CC := gcc
SRCDIR := src
BUILDDIR := build
TARGET := mkproj
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPS := $(OBJECTS:.o=.d)
CFLAGS := -O0 -Wall -Werror
INC := -I include
PREFIX ?= /usr/local

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@$(CC) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " Building..."
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -MMD -MP -c -o $@ $<

debug:
	@$(MAKE) clean
	@$(MAKE) CFLAGS="$(CFLAGS) -g" all

clean:
	@echo " Cleaning..."
	@$(RM) -r $(BUILDDIR)/* $(TARGET)

install: $(TARGET)
	@echo " Installing to $(PREFIX)/bin..."
	@mkdir -p $(PREFIX)/bin
	@install -m 755 $(TARGET) $(PREFIX)/bin/$(TARGET)

uninstall:
	@echo " Uninstalling from $(PREFIX)/bin..."
	@$(RM) $(PREFIX)/bin/$(TARGET)

-include $(DEPS)

.PHONY: all clean debug install uninstall
