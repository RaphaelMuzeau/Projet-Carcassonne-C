CC := gcc
CFLAGS := -std=c17 -pedantic -Wall -Wextra -fdiagnostics-color -Iinclude
LDFLAGS := -lm -Llib -lraylib
BEAR := $(shell command -v bear)

CPUS := $(shell nproc) # nombre de coeurs utilisé pour la compilation parallèle
MAKEFLAGS += --no-print-directory
ifneq ($(MAKELEVEL),0)
	MAKEFLAGS += -j $(CPUS) -l $(CPUS) -O
endif

EXE := bin/carcassonne
DBG := $(EXE)_dbg

SOURCE      := $(wildcard src/*.c)
OBJECTS	    := $(SOURCE:src/%.c=obj/%.o)
DBG_OBJECTS := $(OBJECTS:.o=_dbg.o)

.PHONY: default
default: debug

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(DBG): $(DBG_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJECTS): obj/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -O2

$(DBG_OBJECTS): obj/%_dbg.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -g

compile_commands.json: $(SOURCE)
	@printf "Generation de \x1b[94m$@\x1b[0m..\n"
ifeq ($(strip $(BEAR)),)
	@printf "\x1b[91merr: \x1b[0mpas d'executable 'bear' trouvé.\n"
else
	@$(BEAR) -- ${MAKE} -B debug | sed 's/^/  /'
endif

.PHONY: all release debug run clean bear

all: debug release
release:
	@printf "Compilation de \x1b[93m$@ \x1b[0m($(EXE))...\n"
	@${MAKE} $(EXE) | sed 's/^/  /'
debug:
	@printf "Compilation de \x1b[93m$@ \x1b[0m($(DBG))...\n"
	@${MAKE} $(DBG) | sed 's/^/  /'

run: debug
	@printf "\x1b[95mexecution de debug:\x1b[0;0m\n"
	LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:$${PWD}/lib $(DBG)

clean:
	@rm -f obj/*.o
	@rm -f $(EXE) $(DBG)

bear: compile_commands.json
