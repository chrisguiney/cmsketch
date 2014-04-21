CFLAGS=-std=c99 -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
LIBS=-ldl -lm
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET=build/libsketch.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

BINARY=cmsketch
# The Target Build
all: $(SO_TARGET) $(BINARY) tests

dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

#$(TARGET): CFLAGS += -fPIC 
#$(TARGET): build $(OBJECTS)
#		ar rcs $@ $(OBJECTS)
#		ranlib $@

$(SO_TARGET): build $(OBJECTS)
		$(CC) $(CFLAGS) -shared $(LIBS) -o $@ $(OBJECTS)

$(BINARY): $(SO_TARGET)
	$(CC) $(CFLAGS) -v -o build/$(BINARY) $(LIBS) $(SO_TARGET)

build:
		@mkdir -p build
		@mkdir -p bin

# The Unit Tests
.PHONY: tests
tests: CFLAGS += $(SO_TARGET)
tests: $(TESTS)
		sh ./tests/runtests.sh

valgrind:
		VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)

# The Cleaner
clean:
		rm -rf build $(OBJECTS) $(TESTS)
		rm -f tests/tests.log
		find . -name "*.gc*" -exec rm {} \;
		rm -rf `find . -name "*.dSYM" -print`

# The Install
install: all
		install -d $(DESTDIR)/$(PREFIX)/lib/
		install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
		@echo Files with potentially dangerous functions.
		@egrep $(BADFUNCS) $(SOURCES) || true