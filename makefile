CC := gcc
CFLAGS := -std=c17 -pedantic -Wall -Wextra -Iinclude
LDFLAGS := -lm -Llib -lraylib
BEAR := $(shell command -v bear && echo --)

EXE := bin/carcassonne
DBG := $(EXE)_dbg

SOURCE      := $(wildcard src/*.c)
OBJECTS	    := $(SOURCE:src/%.c=obj/%.o)
DBG_OBJECTS := $(OBJECTS:.o=_dbg.o)

.PHONY: default
default: all

$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(DBG): $(DBG_OBJECTS)
	$(CC) $(DBG_OBJECTS) $(LDFLAGS) -o $@

$(OBJECTS): obj/%.o : src/%.c
	$(CC) $(CFLAGS) -O2 -c $< -o $@

$(DBG_OBJECTS): obj/%_dbg.o : src/%.c
	$(CC) $(CFLAGS) -g -c $< -o $@

.PHONY: all release debug run clean bear

all: debug release
release:
	@printf "\x1b[0;1mCompilation de \x1b[33m$@ \x1b[32m($(EXE))\x1b[0;1m...\x1b[0;0m\n"
	@$(BEAR) ${MAKE} --no-print-directory -j $(EXE) | sed 's/^/  /'
	@printf '\n'
debug:
	@printf "\x1b[0;1mCompilation de \x1b[33m$@ \x1b[32m($(DBG))\x1b[0;1m...\x1b[0;0m\n"
	@$(BEAR) ${MAKE} --no-print-directory -j $(DBG) | sed 's/^/  /'
	@printf '\n'

run: debug
	@printf "\x1b[95;1mexecution de debug:\x1b[0;0m\n"
	LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:$${PWD}/lib $(DBG)

clean:
	@rm -f $(OBJECTS) $(DBG_OBJECTS)
	@rm -f $(EXE) $(DBG)
