# Configuration

SRC_DIR   := src
BIN_DIR   := bin
BUILD_DIR := build

EXE := $(BIN_DIR)/carcassonne
DBG := $(EXE)_dbg
TEST:= $(EXE)_test

CC := gcc
CFLAGS := -std=c17 -pedantic -Wall -Werror -Wextra -D_FORTIFY_SOURCE=3 -fdiagnostics-color -Iinclude
MKFLAGS := -MMD -MP
LDFLAGS := -Llib -l:libraylib.a -lm # raylib est lié statiquement

BEAR := $(shell command -v bear)

VALGRIND := $(shell command -v valgrind)
ifneq ($(strip $(VALGRIND)),)
VGFLAGS := -q --leak-check=full --show-leak-kinds=all --suppressions=.vg_suppress
endif

SHELL:= bash
.SHELLFLAGS += -o pipefail

CPUS := $(shell nproc) # nombre de coeurs utilisé pour la compilation parallèle
MAKEFLAGS += --no-print-directory
ifneq (${MAKELEVEL},0)
	MAKEFLAGS += -j $(CPUS) -l $(CPUS) -O
endif

# Commmandes

.PHONY: default all release debug test run runvg clean bear help
default: debug

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
	./$(TEST)

run: debug
	@printf "\x1b[95mexecution de $<:\x1b[0;0m\n"
	@$(DBG)

runvg: debug
	@printf "\x1b[95mexecution de $< avec valgrind:\x1b[0;0m\n"
	$(VALGRIND) $(VGFLAGS) ./$(DBG)


clean:
	@rm -rf $(BUILD_DIR)/*
	@rm -f  $(BIN_DIR)/*

bear: compile_commands.json

help:
	@echo "Commandes disponibles:"
	@echo "  all     - Lancer toutes les cibles (release, debug, test)"
	@echo "  release - Compiler avec optimisation"
	@echo "  debug   - Compiler avec symboles de debugage"
	@echo "  test    - Compiler et lancer les tests unitaires"
	@echo "  run     - Executer le programme (compile debug si besoin)"
	@echo "  runvg   - Executer le programme en version debug avec valgrind"
	@echo "  clean   - Supprimer les artefacts de compilation et les executables"
	@echo "  bear    - Generer compile_commands.json (nécessite bear)"
	@echo "  help    - Afficher ce message d'aide"

# Recherche des fichiers

sources     := $(shell find $(SRC_DIR) -type f -name '*.c')
objects	    := $(sources:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
dbg_objects := $(objects:.o=_dbg.o)
test_objects:= $(objects:.o=_test.o)

depends     := $(objects:.o=.d)
dbg_depends := $(dbg_objects:.o=.d)
test_depends:= $(test_objects:.o=.d)

# Compilation

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(EXE): $(objects) | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(DBG): $(dbg_objects) | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST): $(test_objects) | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(objects): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(MKFLAGS) -c $< -o $@ -O2

$(dbg_objects): $(BUILD_DIR)/%_dbg.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(MKFLAGS) -c $< -o $@ -O1 -g

$(test_objects): $(BUILD_DIR)/%_test.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(MKFLAGS) -c $< -o $@ -O1 -g -DRUN_UNIT_TESTS

compile_commands.json: $(sources)
	@printf "Generation de \x1b[94m$@\x1b[0m..\n"
ifeq ($(strip $(BEAR)),)
	@printf "\x1b[91merr: \x1b[0mpas d'executable 'bear' trouvé.\n"
else
	@$(BEAR) -- ${MAKE} -B debug | sed 's/^/  /'
endif

# Inclure les makefiles générés par le compilateur

-include $(depends) $(dbg_depends) $(test_depends)
