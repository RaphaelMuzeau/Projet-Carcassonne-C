/* framework de test simple:
 * les fonctions de test sont ecrites dans ce fichier avec une signature de
 * type `bool f(void)` et ajouté à la liste "unit_tests".
 * à l'appel de lancer_tests(), tous les tests sont executés en montrant le nombre
 * d'echecs et de reussites à l'utilisateur. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "libca.h"
#include <limits.h>
#include "grille.h"
#include "pile.h"
#include "varstring.h"
#include "vec.h"

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

bool test_vec_creer(void)
{
    Vec v = creer_vec();

    if (v.tableau != NULL) return false;
    if (v.capacite != 0) return false;
    if (v.decy != 0) return false;

    detruire_vec(v);
    return true;
}

bool test_vec_get_null(void)
{
    Vec v = creer_vec();

    if (vget(v,  0) != NULL) return false;
    if (vget(v,  5) != NULL) return false;
    if (vget(v, -5) != NULL) return false;
    if (vget(v, INT_MAX) != NULL) return false;
    if (vget(v, INT_MIN) != NULL) return false;

    // le vecteur ne doit pas avoir changé
    if (v.tableau != NULL) return false;
    if (v.capacite != 0) return false;
    if (v.decy != 0) return false;

    detruire_vec(v);
    return true;
}

bool test_vec_set_get(void)
{
    Vec v = creer_vec();
    Tuile t0 = init_tuile();

    // acces simple
    vset(&v, t0, 0);
    if (v.tableau == NULL) return false;
    if (v.capacite == 0) return false;
    if (v.capacite != VEC_REALLOC_NB) return false;
    if (v.decy != 0) return false;

    if (v.tableau[0] != t0) return false;
    if (vget(v, 0) != t0) return false;

    // acces complexes
    Tuile t1 = init_tuile();
    vset(&v, t1, -50);

    if (v.capacite % VEC_REALLOC_NB != 0) return false;
    if (v.tableau[  0 + v.decy] != t0) return false;
    if (v.tableau[-50 + v.decy] != t1) return false;
    if (vget(v,   0) != t0) return false;
    if (vget(v, -50) != t1) return false;

    Tuile t2 = init_tuile();
    vset(&v, t2, 50);

    if (v.capacite % VEC_REALLOC_NB != 0) return false;
    if (v.tableau[  0 + v.decy] != t0) return false;
    if (v.tableau[-50 + v.decy] != t1) return false;
    if (v.tableau[ 50 + v.decy] != t2) return false;
    if (vget(v,   0) != t0) return false;
    if (vget(v, -50) != t1) return false;
    if (vget(v,  50) != t2) return false;

    Tuile t3 = init_tuile();
    vset(&v, t3, -51);

    if (v.capacite % VEC_REALLOC_NB != 0) return false;
    if (v.tableau[  0 + v.decy] != t0) return false;
    if (v.tableau[-50 + v.decy] != t1) return false;
    if (v.tableau[ 50 + v.decy] != t2) return false;
    if (v.tableau[-51 + v.decy] != t3) return false;
    if (vget(v,   0) != t0) return false;
    if (vget(v, -50) != t1) return false;
    if (vget(v,  50) != t2) return false;
    if (vget(v, -51) != t3) return false;

    // verifier l'initialisation correcte des cases libres
    for (int i = -49; i < 0; i++) {
        if (v.tableau[i + v.decy] != NULL) return false;
        if (vget(v, i) != NULL) return false;
    }
    for (int i = 1; i < 50; i++) {
        if (v.tableau[i + v.decy] != NULL) return false;
        if (vget(v, i) != NULL) return false;
    }

    // acces hors du tableau marchent encore
    if (vget(v, 5000) != NULL) return false;
    if (vget(v, -5000) != NULL) return false;

    detruire_vec(v);
    return true;
}

bool test_vec2D_creer(void)
{
    Vec2D g = creer_vec2D();

    if (g.tableau != NULL) return false;
    if (g.decx != 0) return false;
    if (g.capacite != 0) return false;

    detruire_vec2D(g);
    return true;
}

bool test_vec2D_get_null(void)
{
    Vec2D g = creer_vec2D();

    if (get(g, 0,  0) != NULL) return false;
    if (get(g, 5,  0) != NULL) return false;
    if (get(g, -5, 0) != NULL) return false;
    if (get(g, 0,  0) != NULL) return false;
    if (get(g, 0,  5) != NULL) return false;
    if (get(g, 0, -5) != NULL) return false;

    if (get(g, INT_MAX, 0) != NULL) return false;
    if (get(g, INT_MIN, 0) != NULL) return false;
    if (get(g, INT_MIN, INT_MAX) != NULL) return false;
    if (get(g, INT_MAX, INT_MIN) != NULL) return false;

    // le vecteur ne doit pas avoir changé
    if (g.tableau != NULL) return false;
    if (g.decx != 0) return false;
    if (g.capacite != 0) return false;

    detruire_vec2D(g);
    return true;
}

bool test_vec2D_set_get(void)
{
    Vec2D g = creer_vec2D();
    Tuile t0 = init_tuile();

    // acces simple
    set(&g, t0, 0, 0);
    if (g.tableau == NULL) return false;
    if (g.capacite == 0) return false;
    if (g.capacite != VEC2D_REALLOC_NB) return false;
    if (g.decx != 0) return false;

    if (g.tableau[0].tableau[0] != t0) return false;
    if (get(g, 0, 0) != t0) return false;

    // acces complexes
    Tuile t1 = init_tuile();
    set(&g, t1, -50, -50);

    if (g.capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;

    Tuile t2 = init_tuile();
    set(&g, t2, 50, -50);

    if (g.capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;
    if (get(g,  50, -50) != t2) return false;

    Tuile t3 = init_tuile();
    set(&g, t3, -50, 50);

    if (g.capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;
    if (get(g,  50, -50) != t2) return false;
    if (get(g, -50,  50) != t3) return false;

    Tuile t4 = init_tuile();
    set(&g, t4, 50, 50);

    if (g.capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;
    if (get(g,  50, -50) != t2) return false;
    if (get(g, -50,  50) != t3) return false;
    if (get(g,  50,  50) != t4) return false;

    // verifier l'initialisation correcte des vecteurs vides
    for (int i = -49; i < 0; i++) {
        if (g.tableau[i + g.decx].tableau != NULL) return false;
        if (g.tableau[i + g.decx].capacite != 0) return false;
        if (g.tableau[i + g.decx].decy != 0) return false;
    }
    for (int i = 1; i < 50; i++) {
        if (g.tableau[i + g.decx].tableau != NULL) return false;
        if (g.tableau[i + g.decx].capacite != 0) return false;
        if (g.tableau[i + g.decx].decy != 0) return false;
    }

    // acces hors du tableau marchent encore
    if (get(g, -5000, -5000) != NULL) return false;
    if (get(g,  5000, -5000) != NULL) return false;
    if (get(g, -5000,  5000) != NULL) return false;
    if (get(g,  5000,  5000) != NULL) return false;

    detruire_vec2D(g);
    return true;
}

bool test_grille_placer_tuile(void)
{
    Vec2D grille = creer_vec2D();
    Tuile t = init_tuile();

    // placement sans connexion
    if (placer_tuile(grille, 1, 1, t)) return false;
    if (placer_tuile(grille, 1, 0, t)) return false;
    if (placer_tuile(grille, 0, 1, t)) return false;
    if (placer_tuile(grille, 1, 9, t)) return false;
    if (placer_tuile(grille, 9, 1, t)) return false;
    if (get(grille, 1, 1) == t) return false;
    if (get(grille, 1, 0) == t) return false;
    if (get(grille, 0, 1) == t) return false;
    if (get(grille, 1, 9) == t) return false;
    if (get(grille, 9, 1) == t) return false;

    set(&grille, t, 1, 1);

    // tuile occupée
    if (placer_tuile(grille, 1, 1, t)) return false;

    // placement compatible
    t = init_tuile();
    if (!placer_tuile(grille, 0, 1, t)) return false;
    if (get(grille, 0, 1)  != t) return false;
    t = init_tuile();
    if (!placer_tuile(grille, 1, 0, t)) return false;
    if (get(grille, 1, 0)  != t) return false;
    t = init_tuile();
    if (!placer_tuile(grille, 2, 1, t)) return false;
    if (get(grille, 2, 1) != t) return false;
    t = init_tuile();
    if (!placer_tuile(grille, 1, 2, t)) return false;
    if (get(grille, 1, 2) != t) return false;

    // placement incompatible
    t = init_tuile();
    t->nord = Z_VILLE; t->sud = Z_VILLE; t->est = Z_VILLE; t->ouest = Z_VILLE;
    set(&grille, t, 5, 5);

    t = init_tuile();
    if (placer_tuile(grille, 0, 1, t)) return false;
    if (get(grille, 0, 1)  == t) return false;
    if (placer_tuile(grille, 1, 0, t)) return false;
    if (get(grille, 1, 0)  == t) return false;
    if (placer_tuile(grille, 2, 1, t)) return false;
    if (get(grille, 2, 1)  == t) return false;
    if (placer_tuile(grille, 1, 2, t)) return false;
    if (get(grille, 1, 2) == t) return false;

    free(t);

    detruire_vec2D(grille);
    return true;
}

bool test_inserer_tuile(void)
{
    int i;
    bool result_test = true;
    Pile p = creer_pile(1);

    Tuile t = ca_alloc(1, sizeof(struct _Tuile));
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
    Tuile t = ca_alloc(1, sizeof(struct _Tuile));
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

bool test_varstring_ajout_char(void)
{
    VarString s = creer_varstring();

    ajouter_char(&s, 'h');
    ajouter_char(&s, 'e');
    ajouter_char(&s, 'l');
    ajouter_char(&s, 'l');
    ajouter_char(&s, 'o');

    if (strcmp(s.texte, "hello")) return false;
    if (s.len != strlen(s.texte)) return false;
    if (s.capacite < strlen(s.texte) + 1) return false;

    detruire_varstring(s);
    return true;
}

bool test_varstring_ajout_chaine(void)
{
    VarString s = creer_varstring();

    ajouter_chaine(&s, "hello");

    if (strcmp(s.texte, "hello")) return false;
    if (s.len != strlen(s.texte)) return false;
    if (s.capacite < strlen(s.texte) + 1) return false;

    ajouter_chaine(&s, " world");

    if (strcmp(s.texte, "hello world")) return false;
    if (s.len != strlen(s.texte)) return false;
    if (s.capacite < strlen(s.texte) + 1) return false;

    detruire_varstring(s);
    return true;
}

bool test_varstring_vider(void)
{
    VarString s = creer_varstring();

    ajouter_chaine(&s, "hello");
    vider_varstring(&s);

    if (*s.texte != '\0') return false;
    if (s.len != 0) return false;

    detruire_varstring(s);
    return true;
}

bool test_varstring_ajouter_null(void)
{
    VarString s = creer_varstring();

    ajouter_chaine(&s, "hello");
    size_t expected_len = s.len;
    size_t expected_cap = s.capacite;

    ajouter_chaine(&s, NULL);
    ajouter_char(&s, '\0');

    if (strcmp(s.texte, "hello")) return false;
    if (s.len != expected_len) return false;
    if (s.capacite != expected_cap) return false;

    detruire_varstring(s);
    return true;
}

bool test_varstring_null(void)
{
    // pourrait declencher un crash
    ajouter_char(NULL, '\0');
    ajouter_chaine(NULL, NULL);
    vider_varstring(NULL);
    return true;
}

// ajout à la liste de tests à executer
Test unit_tests[] = {
    TEST(test_tuile_creer),
    TEST(test_tuile_compatibilite),
    TEST(test_vec_creer),
    TEST(test_vec_get_null),
    TEST(test_vec_set_get),
    TEST(test_vec2D_creer),
    TEST(test_vec2D_get_null),
    TEST(test_vec2D_set_get),
    TEST(test_grille_placer_tuile),
    TEST(test_init_pile),
    TEST(test_inserer_tuile),
    TEST(test_recup_tuile),
    TEST(test_varstring_ajout_char),
    TEST(test_varstring_ajout_chaine),
    TEST(test_varstring_vider),
    TEST(test_varstring_ajouter_null),
    TEST(test_varstring_null),
};

// ===========================
// ==== fin des fonctions ====
// ===========================

#define len(ARRAY) sizeof(ARRAY)/sizeof(ARRAY[0])
_Noreturn void lancer_tests(void)
{
    int passed = 0;
    int failed = 0;

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
