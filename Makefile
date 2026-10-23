CC ?= gcc
CWARNINGS := -Wall -Wextra
override CPPFLAGS += -MMD -MP -I include

SRCDIR := src
SRCEXT := c
TARGET := mkproj
BUILDDIR := build
PREFIX := /usr/local

SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)' | sort)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

DEPS := $(OBJECTS:.o=.d)
CFLAGS := -O3 $(CWARNINGS)

all: $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@printf "[BUILDING]: "
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	@printf "[LINKING]: "
	$(CC) $^ -o $@

clean:
	@printf "[CLEANING]: "
	rm -rf $(BUILDDIR) $(TARGET)

install: $(TARGET)
	@printf "[INSTALLING]: "
	@mkdir -p $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin/$(TARGET)

uninstall:
	@printf "[UNINSTALLING]: "
	$(RM) $(PREFIX)/bin/$(TARGET)

-include $(DEPS)

.PHONY: all clean install uninstall
