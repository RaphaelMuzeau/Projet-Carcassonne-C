CC := gcc
CFLAGS := -std=c17 -pedantic -Wall -Wextra -Iinclude
LDFLAGS := -lm -Llib -lraylib

EXE := bin/carcassonne
DBG := $(EXE)_dbg

SOURCE      := $(wildcard src/*.c)
OBJECTS	    := $(SOURCE:src/%.c=obj/%.o)
DBG_OBJECTS := $(OBJECTS:.o=_dbg.o)

.PHONY: default
default: debug

$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(DBG): $(DBG_OBJECTS)
	$(CC) $(DBG_OBJECTS) $(LDFLAGS) -o $@

$(OBJECTS): $(SOURCE)
	$(CC) $(CFLAGS) -O2 -c $^ -o $@

$(DBG_OBJECTS): $(SOURCE)
	$(CC) $(CFLAGS) -g -c $^ -o $@

.PHONY: all release debug
all: release debug
release: $(EXE)
debug: $(DBG)

.PHONY: run
run: $(DBG)
	LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:$${PWD}/lib $(DBG)

.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(DBG_OBJECTS)
	@rm -f $(EXE) $(DBG)

.PHONY: bear
bear: compile_commands.json

compile_commands.json:
	make clean
	bear -- make $(EXE)
