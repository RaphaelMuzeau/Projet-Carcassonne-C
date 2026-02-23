/* framework de test simple:
 * les fonctions de test sont ecrites dans ce fichier avec une signature de
 * type `bool f(void)` et ajouté à la liste "unit_tests".
 * à l'appel de lancer_tests(), tous les tests sont executés en montrant le nombre
 * d'echecs et de reussites à l'utilisateur. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grille.h"
#include "pile.h"

typedef struct _Test {
    bool (*run)(void);
    const char *name;
} Test;
#define TEST(FUNCTION) { FUNCTION, #FUNCTION } // creer un Test à partir du nom d'une fonction

// ===========================
// ==== fonctions de test ====
// ===========================

bool test_init_grille(void)
{
    Grille grille = init_grille(10);

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++)
            if (grille.tableau[i][j] != NULL)
                return false;
    }

    return true;
}

bool test_init_pile(void)
{
    Pile p = creer_pile(10);

    if (!pile_vide(p)) {
        return false;
    }

    if (p.nb_element_max != 10)
        return false;

    return true;
}

bool test_inserer_tuile(void)
{
    int i;
    bool result_test = true;
    Pile p = creer_pile(1);

    Tuile t = malloc(sizeof(struct _Tuile));
    t->sud = Z_ABBAYE;
    t->est = Z_ABBAYE;
    t->nord = Z_ABBAYE;
    t->milieu = Z_ABBAYE;
    t->ouest = Z_ABBAYE;

    for (i = 0; i < p.nb_element_max ; i++) {
        if (!inserer_tuile(&p, t)) {
            result_test = false;
            break;
        }
    }

    if (inserer_tuile(&p, t))
        result_test = false;

    detruire_pile(&p);

    return result_test;
}

bool test_recup_tuile(void)
{
    Pile p = creer_pile(5);
    Tuile t = malloc(sizeof(struct _Tuile));
    t->sud = Z_ABBAYE;
    t->est = Z_ABBAYE;
    t->nord = Z_ABBAYE;
    t->milieu = Z_ABBAYE;
    t->ouest = Z_ABBAYE;

    inserer_tuile(&p, t);
    if (recup_tuile(&p) != t || p.nb_element != 0)
        return false;

    if (recup_tuile(&p) != NULL)
        return false;

    return true;
}

// ajout à la liste de tests à executer
Test unit_tests[] = {
    TEST(test_init_grille),
    TEST(test_init_pile),
    TEST(test_inserer_tuile),
    TEST(test_recup_tuile),
};

// ===========================
// ==== fin des fonctions ====
// ===========================

#define len(ARRAY) sizeof(ARRAY)/sizeof(ARRAY[0])
_Noreturn void lancer_tests(void)
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
