CC := gcc
CFLAGS := -std=c17 -pedantic -Wall -Werror -Wextra -fdiagnostics-color -Iinclude
LDFLAGS := -lm -Llib -lraylib
BEAR := $(shell command -v bear)
SHELL:= bash

CPUS := $(shell nproc) # nombre de coeurs utilisé pour la compilation parallèle
MAKEFLAGS += --no-print-directory
ifneq ($(MAKELEVEL),0)
	MAKEFLAGS += -j $(CPUS) -l $(CPUS) -O
endif

EXE := bin/carcassonne
DBG := $(EXE)_dbg
TEST:= $(EXE)_test

SOURCE      := $(wildcard src/*.c)
OBJECTS	    := $(SOURCE:src/%.c=obj/%.o)
DBG_OBJECTS := $(OBJECTS:.o=_dbg.o)
TEST_OBJECTS:= $(OBJECTS:.o=_test.o)

.PHONY: default
default: debug

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(DBG): $(DBG_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(TEST): $(TEST_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJECTS): obj/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -O2

$(DBG_OBJECTS): obj/%_dbg.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -g

$(TEST_OBJECTS): obj/%_test.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -g -DRUN_UNIT_TESTS

compile_commands.json: $(SOURCE)
	@printf "Generation de \x1b[94m$@\x1b[0m..\n"
ifeq ($(strip $(BEAR)),)
	@printf "\x1b[91merr: \x1b[0mpas d'executable 'bear' trouvé.\n"
else
	@$(BEAR) -- ${MAKE} -B debug | sed 's/^/  /'
endif

.PHONY: all release debug test run clean bear help

all: debug release test
release:
	@printf "Compilation de \x1b[93m$@ \x1b[0m($(EXE))...\n"
	@${MAKE} $(EXE) | sed 's/^/  /'
debug:
	@printf "Compilation de \x1b[93m$@ \x1b[0m($(DBG))...\n"
	@${MAKE} $(DBG) | sed 's/^/  /'
test:
	@printf "Compilation de \x1b[93m$@ \x1b[0m($(TEST))...\n"
	@${MAKE} $(TEST) | sed 's/^/  /'
	@LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:$${PWD}/lib $(TEST)

run: debug
	@printf "\x1b[95mexecution de debug:\x1b[0;0m\n"
	LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:$${PWD}/lib $(DBG)

clean:
	@rm -f obj/*.o
	@rm -f $(EXE) $(DBG) $(TEST)

bear: compile_commands.json

help:
	@echo "Commandes disponibles:"
	@echo "  all     - Lancer toutes les cibles (release, debug, test)"
	@echo "  release - Compiler avec optimisation"
	@echo "  debug   - Compiler avec symboles de debugage"
	@echo "  test    - Compiler et lancer les tests unitaires"
	@echo "  bear    - Generer compile_commands.json (nécessite bear)"
	@echo "  run     - Executer le programme (compile debug si besoin)"
	@echo "  clean   - Supprimer les artefacts de compilation et les executables"
	@echo "  help    - Afficher ce message d'aide"
