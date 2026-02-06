CC := gcc
CFLAGS := -std=c17 -pedantic -Wall -Wextra -Iinclude
LDFLAGS := -lm -Llib -lraylib

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

$(OBJECTS): $(SOURCE)
	$(CC) $(CFLAGS) -O2 -c $^ -o $@

$(DBG_OBJECTS): $(SOURCE)
	$(CC) $(CFLAGS) -g -c $^ -o $@

compile_commands.json:
	make clean
	bear -- make $(EXE)

.PHONY: all release debug run clean bear

all: release debug
release: $(EXE)
debug: $(DBG)

run: $(DBG)
	LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:$${PWD}/lib $(DBG)

clean:
	@rm -f $(OBJECTS) $(DBG_OBJECTS)
	@rm -f $(EXE) $(DBG)

bear: compile_commands.json
