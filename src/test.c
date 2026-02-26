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

bool test_tuile_creer(void)
{
    Tuile t = init_tuile();

    if (t->milieu != Z_PRE) return false;
    if (t->nord != Z_PRE)   return false;
    if (t->sud != Z_PRE)    return false;
    if (t->est != Z_PRE)    return false;
    if (t->ouest != Z_PRE)  return false;
    if (t->meeple) return false;

    free(t);
    return true;
}

bool test_tuile_compatibilite(void)
{
    Tuile depart  = init_tuile();
    Tuile arrivee = init_tuile();

    depart->nord  = Z_ROUTE;
    depart->sud   = Z_PRE;
    depart->est   = Z_VILLE;
    depart->ouest = Z_BLASON;

    arrivee->sud   = Z_ROUTE;
    arrivee->nord  = Z_PRE;
    arrivee->ouest = Z_VILLE;
    arrivee->est   = Z_VILLE;

    if (!compatibilite_tuile(depart, arrivee, D_NORD))  return false;
    if (!compatibilite_tuile(depart, arrivee, D_SUD))   return false;
    if (!compatibilite_tuile(depart, arrivee, D_EST))   return false;
    if (!compatibilite_tuile(depart, arrivee, D_OUEST)) return false;
    if (!compatibilite_tuile(arrivee, depart, D_NORD))  return false;
    if (!compatibilite_tuile(arrivee, depart, D_SUD))   return false;
    if (!compatibilite_tuile(arrivee, depart, D_EST))   return false;
    if (!compatibilite_tuile(arrivee, depart, D_OUEST)) return false;

    pivot_90(arrivee);

    if (compatibilite_tuile(depart, arrivee, D_NORD))  return false;
    if (compatibilite_tuile(depart, arrivee, D_SUD))   return false;
    if (compatibilite_tuile(depart, arrivee, D_EST))   return false;
    if (compatibilite_tuile(depart, arrivee, D_OUEST)) return false;
    if (compatibilite_tuile(arrivee, depart, D_NORD))  return false;
    if (compatibilite_tuile(arrivee, depart, D_SUD))   return false;
    if (compatibilite_tuile(arrivee, depart, D_EST))   return false;
    if (compatibilite_tuile(arrivee, depart, D_OUEST)) return false;

    free(depart);
    free(arrivee);
    return true;
}

bool test_grille_creer(void)
{
    Grille grille = init_grille(10);

    if (grille.taille != 10) return false;

    for(int i = 0; i < grille.taille; i++)
        for(int j = 0; j < grille.taille; j++)
            if (grille.tableau[i][j] != NULL)
                return false;


    destruction_grille(grille);
    return true;
}

bool test_grille_est_vide(void)
{
    Grille grille = init_grille(10);

    // cases allouées sont vides
    for(int i = 0; i < grille.taille; i++)
        for(int j = 0; j < grille.taille; j++)
            if (!est_vide(grille, i, j))
                return false;

    // cases hors champs
    if (!est_vide(grille, -11, 0)) return false;
    if (!est_vide(grille, 11, 0)) return false;
    if (!est_vide(grille, 0, -11)) return false;
    if (!est_vide(grille, 0, 11)) return false;

    // case presente
    grille.tableau[1][2] = init_tuile();
    if (est_vide(grille, 1, 2)) return false;

    destruction_grille(grille);
    return true;
}

bool test_grille_placer_tuile(void)
{
    Grille grille = init_grille(10);
    Tuile t = init_tuile();

    // placement hors champs
    if (placer_tuile(grille, -11, 0, t)) return false;
    if (placer_tuile(grille, 11, 0, t)) return false;
    if (placer_tuile(grille, 0, -11, t)) return false;
    if (placer_tuile(grille, 0, 11, t)) return false;

    // placement sans connexion
    if (placer_tuile(grille, 1, 1, t)) return false;
    if (placer_tuile(grille, 1, 0, t)) return false;
    if (placer_tuile(grille, 0, 1, t)) return false;
    if (placer_tuile(grille, 1, 9, t)) return false;
    if (placer_tuile(grille, 9, 1, t)) return false;
    if (grille.tableau[1][1] == t) return false;
    if (grille.tableau[1][0] == t) return false;
    if (grille.tableau[0][1] == t) return false;
    if (grille.tableau[1][9] == t) return false;
    if (grille.tableau[9][1] == t) return false;

    grille.tableau[1][1] = t;

    // tuile occupée
    if (placer_tuile(grille, 1, 1, t)) return false;

    // placement compatible
    t = init_tuile();
    if (!placer_tuile(grille, 0, 1, t)) return false;
    if (grille.tableau[0][1] != t) return false;
    t = init_tuile();
    if (!placer_tuile(grille, 1, 0, t)) return false;
    if (grille.tableau[1][0] != t) return false;
    t = init_tuile();
    if (!placer_tuile(grille, 2, 1, t)) return false;
    if (grille.tableau[2][1] != t) return false;
    t = init_tuile();
    if (!placer_tuile(grille, 1, 2, t)) return false;
    if (grille.tableau[1][2] != t) return false;

    // placement incompatible
    t = init_tuile();
    t->nord = Z_VILLE; t->sud = Z_VILLE; t->est = Z_VILLE; t->ouest = Z_VILLE;
    grille.tableau[5][5] = t;

    t = init_tuile();
    if (placer_tuile(grille, 0, 1, t)) return false;
    if (grille.tableau[0][1] == t) return false;
    if (placer_tuile(grille, 1, 0, t)) return false;
    if (grille.tableau[1][0] == t) return false;
    if (placer_tuile(grille, 2, 1, t)) return false;
    if (grille.tableau[2][1] == t) return false;
    if (placer_tuile(grille, 1, 2, t)) return false;
    if (grille.tableau[1][2] == t) return false;
    free(t);

    destruction_grille(grille);
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
    TEST(test_tuile_creer),
    TEST(test_tuile_compatibilite),
    TEST(test_grille_creer),
    TEST(test_grille_est_vide),
    TEST(test_grille_placer_tuile),
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
