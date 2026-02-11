/* framework de test simple:
 * les fonctions de test sont ecrites dans ce fichier avec une signature de
 * type `bool f(void)` et ajouté à la liste "unit_tests".
 * à l'appel de lancer_tests(), tous les tests sont executés en montrant le nombre
 * d'echecs et de reussites à l'utilisateur. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*
#include "model.h"
#include "vue.h"
#include "controller.h"
*/

typedef struct _Test {
    _Bool (*run)(void);
    const char *name;
} Test;
#define TEST(FUNCTION) { FUNCTION, #FUNCTION } // creer un Test à partir du nom d'une fonction

// ===========================
// ==== fonctions de test ====
// ===========================

bool foo(void)
{
    return true;
}

bool bar(void)
{
    return false;
}

bool baz(void)
{
    return true;
}

// ajout à la liste de tests à executer
Test unit_tests[] = {
    TEST(foo),
    TEST(bar),
    TEST(baz),
};

// ===========================
// ==== fin des fonctions ====
// ===========================

#define len(ARRAY) sizeof(ARRAY)/sizeof(ARRAY[0]) // maybe move these to their own header file?
_Noreturn void lancer_tests(void) // TODO: make output prettier
{
    int passed = 0;
    int failed  = 0;

    Test t;
    bool result;

    fprintf(stderr, "lancement de la suite de tests:\n");
    for (size_t i = 0; i < len(unit_tests); i++) {
        t = unit_tests[i];
        fprintf(stderr, "  \x1b[0;1m%s\x1b[0;0m:", t.name);
        result = t.run();

        if (result) {
            fprintf(stderr, "  ok\n");
            ++passed;
        } else {
            fprintf(stderr, "  \x1b[91mechec\x1b[0m\n");
            ++failed;
        }
    }

    fprintf(stderr, "resultats:\n");

    if (failed > 0) {
        fprintf(stderr, "  %d ok, \x1b[91m%d echec(s)\x1b[0m.. exiting\n", passed, failed);
        exit(EXIT_FAILURE);
    } else {
        fprintf(stderr, "  %d ok, %d echecs.. exiting\n", passed, failed);
        exit(EXIT_SUCCESS);
    }
}
