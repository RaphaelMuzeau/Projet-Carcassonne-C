/* framework de test simple:
 * les fonctions de test sont ecrites dans ce fichier avec une signature de
 * type `bool f(void)` et ajouté à la liste "unit_tests".
 * à l'appel de lancer_tests(), tous les tests sont executés en montrant le nombre
 * d'echecs et de reussites à l'utilisateur. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "grille.h"
#include "pile.h"
#include "tuile.h"
#include "varstring.h"
#include "vec.h"
#include "csv.h"

typedef struct _Test {
    bool (*run)(void);
    const char *name;
} Test;
#define TEST(FUNCTION) { FUNCTION, #FUNCTION } // creer un Test à partir du nom d'une fonction

// ===========================
// ==== fonctions de test ====
// ===========================

bool test_tuile_creer(void)
{
    Tuile t = creer_tuile();

    if (t->milieu != Z_PRE) return false;
    if (t->nord != Z_PRE)   return false;
    if (t->sud != Z_PRE)    return false;
    if (t->est != Z_PRE)    return false;
    if (t->ouest != Z_PRE)  return false;
    if (t->meeple) return false;

    free(t);
    return true;
}

bool test_tuile_pivot90(void)
{
    Tuile t = creer_tuile();

    t->milieu = Z_ABBAYE;
    t->nord = Z_VILLE;
    t->sud = Z_PRE;
    t->est = Z_ROUTE;
    t->ouest = Z_BLASON;

    pivot_90(t);

    if (t->milieu != Z_ABBAYE) return false;
    if (t->nord   != Z_BLASON) return false;
    if (t->est    != Z_VILLE)  return false;
    if (t->sud    != Z_ROUTE)  return false;
    if (t->ouest  != Z_PRE)    return false;

    free(t);
    return true;
}

bool test_tuile_compatibilite(void)
{
    Tuile depart  = creer_tuile();
    Tuile arrivee = creer_tuile();

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

bool test_tuile_generer(void)
{
    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        Tuile tmp = generer_tuile();

        if (tmp->est == Z_VILLAGE)   return false;
        if (tmp->sud == Z_VILLAGE)   return false;
        if (tmp->nord == Z_VILLAGE)  return false;
        if (tmp->ouest == Z_VILLAGE) return false;

        if (tmp->est == Z_ABBAYE)    return false;
        if (tmp->sud == Z_ABBAYE)    return false;
        if (tmp->nord == Z_ABBAYE)   return false;
        if (tmp->ouest == Z_ABBAYE)  return false;

        if (tmp->milieu == Z_ABBAYE) {
            if (tmp->est != Z_ROUTE && tmp->est != Z_PRE)     return false;
            if (tmp->sud != Z_ROUTE && tmp->sud != Z_PRE)     return false;
            if (tmp->nord != Z_ROUTE && tmp->nord != Z_PRE)   return false;
            if (tmp->ouest != Z_ROUTE && tmp->ouest != Z_PRE) return false;
        }

        if (tmp->milieu == Z_ROUTE || tmp->milieu == Z_VILLE) {
            int cmpt = 0;
            if (tmp->est == tmp->milieu) cmpt++;
            if (tmp->sud == tmp->milieu) cmpt++;
            if (tmp->nord == tmp->milieu) cmpt++;
            if (tmp->ouest == tmp->milieu) cmpt++;

            if(cmpt < 2) return false;
        }

        free(tmp);
    }
    return true;
}

bool test_pile_creer(void)
{
    Pile p = creer_pile(10, false);

    if (!pile_vide(p))          return false;
    if (p.nb_element_max != 10) return false;
    if (p.nb_element != 0)      return false;
    if (p.gen_aleatoire)        return false;

    detruire_pile(&p);
    return true;
}

bool test_pile_creer_aleatoire(void)
{
    Pile p = creer_pile(10, true);

    if(!pile_pleine(p))          return false;
    if (p.nb_element != 10)      return false;
    if (p.nb_element_max != 10)  return false;
    if (!p.gen_aleatoire)        return false;
    if (p.tab != NULL)           return false;

    detruire_pile(&p);
    return true;
}

bool test_pile_inserer_tuile(void)
{
    Pile p = creer_pile(10, false);
    Tuile t = NULL;

    for (int i = 0; i < p.nb_element_max ; i++) {
        t = creer_tuile();
        if (!inserer_tuile(&p, t)) return false;
        if (p.nb_element != i+1) return false;
    }

    if (p.nb_element != p.nb_element_max) return false;
    if (inserer_tuile(&p, t)) return false;

    detruire_pile(&p);
    return true;
}

bool test_pile_recuperer_tuile_non_aleatoire(void)
{
    Pile p = creer_pile(5, false);
    Tuile t = creer_tuile();

    inserer_tuile(&p, t);
    if (recup_tuile(&p) != t) return false;
    if (p.nb_element != 0) return false;
    if (recup_tuile(&p) != NULL) return false;

    free(t);
    detruire_pile(&p);
    return true;
}

bool test_pile_recuperer_tuile_aleatoire(void)
{
    int i = 10;
    Pile p = creer_pile(10, true);

    if (p.nb_element_max != i) return false;
    if (!p.gen_aleatoire)      return false;
    if (p.nb_element != i)     return false;

    while (i != 0) {
        Tuile t = recup_tuile(&p);
        free(t);
        i--;
        if (p.nb_element != i) return false;
    }

    if (p.nb_element != 0) return false;
    detruire_pile(&p);
    return true;
}

bool test_vec_creer(void)
{
    Vec v = creer_vec();

    if (v._tableau != NULL) return false;
    if (v._capacite != 0) return false;
    if (v._decy != 0) return false;

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
    if (v._tableau != NULL) return false;
    if (v._capacite != 0) return false;
    if (v._decy != 0) return false;

    detruire_vec(v);
    return true;
}

bool test_vec_set_get(void)
{
    Vec v = creer_vec();
    Tuile t0 = creer_tuile();

    // acces simple
    vset(&v, t0, 0);
    if (v._tableau == NULL) return false;
    if (v._capacite == 0) return false;
    if (v._capacite != VEC_REALLOC_NB) return false;
    if (v._decy != 0) return false;

    if (v._tableau[0] != t0) return false;
    if (vget(v, 0) != t0) return false;

    // acces complexes
    Tuile t1 = creer_tuile();
    vset(&v, t1, -50);

    if (v._capacite % VEC_REALLOC_NB != 0) return false;
    if (v._tableau[  0 + v._decy] != t0) return false;
    if (v._tableau[-50 + v._decy] != t1) return false;
    if (vget(v,   0) != t0) return false;
    if (vget(v, -50) != t1) return false;

    Tuile t2 = creer_tuile();
    vset(&v, t2, 50);

    if (v._capacite % VEC_REALLOC_NB != 0) return false;
    if (v._tableau[  0 + v._decy] != t0) return false;
    if (v._tableau[-50 + v._decy] != t1) return false;
    if (v._tableau[ 50 + v._decy] != t2) return false;
    if (vget(v,   0) != t0) return false;
    if (vget(v, -50) != t1) return false;
    if (vget(v,  50) != t2) return false;

    Tuile t3 = creer_tuile();
    vset(&v, t3, -51);

    if (v._capacite % VEC_REALLOC_NB != 0) return false;
    if (v._tableau[  0 + v._decy] != t0) return false;
    if (v._tableau[-50 + v._decy] != t1) return false;
    if (v._tableau[ 50 + v._decy] != t2) return false;
    if (v._tableau[-51 + v._decy] != t3) return false;
    if (vget(v,   0) != t0) return false;
    if (vget(v, -50) != t1) return false;
    if (vget(v,  50) != t2) return false;
    if (vget(v, -51) != t3) return false;

    // verifier l'initialisation correcte des cases libres
    for (int i = -49; i < 0; i++) {
        if (v._tableau[i + v._decy] != NULL) return false;
        if (vget(v, i) != NULL) return false;
    }
    for (int i = 1; i < 50; i++) {
        if (v._tableau[i + v._decy] != NULL) return false;
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

    if (g._tableau != NULL) return false;
    if (g._decx != 0) return false;
    if (g._capacite != 0) return false;

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
    if (g._tableau != NULL) return false;
    if (g._decx != 0) return false;
    if (g._capacite != 0) return false;

    detruire_vec2D(g);
    return true;
}

bool test_vec2D_set_get(void)
{
    Vec2D g = creer_vec2D();
    Tuile t0 = creer_tuile();

    // acces simple
    set(&g, t0, 0, 0);
    if (g._tableau == NULL) return false;
    if (g._capacite == 0) return false;
    if (g._capacite != VEC2D_REALLOC_NB) return false;
    if (g._decx != 0) return false;

    if (g._tableau[0]._tableau[0] != t0) return false;
    if (get(g, 0, 0) != t0) return false;

    // acces complexes
    Tuile t1 = creer_tuile();
    set(&g, t1, -50, -50);

    if (g._capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;

    Tuile t2 = creer_tuile();
    set(&g, t2, 50, -50);

    if (g._capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;
    if (get(g,  50, -50) != t2) return false;

    Tuile t3 = creer_tuile();
    set(&g, t3, -50, 50);

    if (g._capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;
    if (get(g,  50, -50) != t2) return false;
    if (get(g, -50,  50) != t3) return false;

    Tuile t4 = creer_tuile();
    set(&g, t4, 50, 50);

    if (g._capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;
    if (get(g,  50, -50) != t2) return false;
    if (get(g, -50,  50) != t3) return false;
    if (get(g,  50,  50) != t4) return false;

    // verifier l'initialisation correcte des vecteurs vides
    for (int i = -49; i < 0; i++) {
        if (g._tableau[i + g._decx]._tableau != NULL) return false;
        if (g._tableau[i + g._decx]._capacite != 0) return false;
        if (g._tableau[i + g._decx]._decy != 0) return false;
    }
    for (int i = 1; i < 50; i++) {
        if (g._tableau[i + g._decx]._tableau != NULL) return false;
        if (g._tableau[i + g._decx]._capacite != 0) return false;
        if (g._tableau[i + g._decx]._decy != 0) return false;
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
    Tuile t = creer_tuile();

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
    t = creer_tuile();
    if (!placer_tuile(grille, 0, 1, t)) return false;
    if (get(grille, 0, 1)  != t) return false;
    t = creer_tuile();
    if (!placer_tuile(grille, 1, 0, t)) return false;
    if (get(grille, 1, 0)  != t) return false;
    t = creer_tuile();
    if (!placer_tuile(grille, 2, 1, t)) return false;
    if (get(grille, 2, 1) != t) return false;
    t = creer_tuile();
    if (!placer_tuile(grille, 1, 2, t)) return false;
    if (get(grille, 1, 2) != t) return false;

    // placement incompatible
    t = creer_tuile();
    t->nord = Z_VILLE; t->sud = Z_VILLE; t->est = Z_VILLE; t->ouest = Z_VILLE;
    set(&grille, t, 5, 5);

    t = creer_tuile();
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

bool test_varstring_ajouter_char(void)
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

bool test_varstring_retirer_char(void)
{
    VarString s = creer_varstring();

    ajouter_char(&s, 'h');
    ajouter_char(&s, 'i');

    retirer_char(&s);
    if (strcmp(s.texte, "h")) return false;
    if (s.len != 1) return false;

    retirer_char(&s);
    if (*s.texte != '\0') return false;
    if (s.len != 0) return false;

    retirer_char(&s);
    if (*s.texte != '\0') return false;
    if (s.len != 0) return false;

    detruire_varstring(s);
    return true;
}

bool test_varstring_ajouter_chaine(void)
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
    retirer_char(NULL);
    ajouter_chaine(NULL, NULL);
    vider_varstring(NULL);
    return true;
}

bool test_csv_compter_lignes(void)
{

    FILE *f0, *f1, *f2;
    f0 = fopen("data/test/0_test.csv", "r");
    if (compter_lignes(f0) != 1) {
        fclose(f0);
        return false;
    }
    fclose(f0);
    f1 = fopen("data/test/1_test.csv", "r");
    if (compter_lignes(f1) != 5) {
        fclose(f1);
        return false;
    }
    fclose(f1);
    f2 = fopen("data/test/test_non_existant.csv", "r");
    if (compter_lignes(f2) != 0) {
        return false;
    }
    return true;
}

bool test_csv_lecture_zone(void)
{
    FILE *f = fopen("data/test/0_test.csv", "r");
    Tuile t = creer_tuile();

    lire_zone(&t->est, f);
    lire_zone(&t->nord, f);
    lire_zone(&t->ouest, f);
    lire_zone(&t->sud, f);
    lire_zone(&t->milieu, f);

    if (t->est != Z_ROUTE) return false;
    if (t->nord != Z_VILLE) return false;
    if (t->ouest!= Z_BLASON) return false;
    if (t->sud != Z_PRE) return false;
    if (t->milieu != Z_ABBAYE) return false;

    free(t);
    fclose(f);
    return true;
}

bool test_csv_lecture_fichier(void)
{
    int i;
    Pile p = lire_tuiles_csv("data/test/1_test.csv");

    if (p.nb_element < p.nb_element_max) return false;

    for (i = 0; i < p.nb_element_max; i++)
        if (p.tab[i] == NULL) return false;

    if (p.tab[0]->nord != Z_VILLE) return false;
    if (p.tab[0]->sud != Z_VILLE) return false;
    if (p.tab[0]->est != Z_ROUTE) return false;
    if (p.tab[0]->ouest != Z_ROUTE) return false;
    if (p.tab[0]->milieu != Z_ROUTE) return false;

    if (p.tab[1]->nord != Z_ROUTE) return false;
    if (p.tab[1]->sud != Z_VILLE) return false;
    if (p.tab[1]->est != Z_VILLE) return false;
    if (p.tab[1]->ouest != Z_PRE) return false;
    if (p.tab[1]->milieu != Z_ABBAYE) return false;

    if (p.tab[2]->nord != Z_BLASON) return false;
    if (p.tab[2]->sud != Z_BLASON) return false;
    if (p.tab[2]->est != Z_BLASON) return false;
    if (p.tab[2]->ouest != Z_BLASON) return false;
    if (p.tab[2]->milieu != Z_BLASON) return false;

    if (p.tab[3]->nord != Z_ROUTE) return false;
    if (p.tab[3]->sud != Z_ROUTE) return false;
    if (p.tab[3]->est != Z_PRE) return false;
    if (p.tab[3]->ouest != Z_ROUTE) return false;
    if (p.tab[3]->milieu != Z_VILLAGE) return false;

    if (p.tab[4]->nord != Z_BLASON) return false;
    if (p.tab[4]->sud != Z_BLASON) return false;
    if (p.tab[4]->est != Z_BLASON) return false;
    if (p.tab[4]->ouest != Z_ROUTE) return false;
    if (p.tab[4]->milieu != Z_BLASON) return false;

    detruire_pile(&p);
    return true;
}

bool test_csv_fichier_vide(void)
{
    Pile p;
    p = lire_tuiles_csv("data/test/2_test.csv");

    if (p.nb_element_max != 0) return false;
    detruire_pile(&p);

    return true;
}

bool test_csv_fichier_introuvable(void)
{
    Pile p;
    p = lire_tuiles_csv("data/test/test_non_existant.csv");

    if (p.nb_element_max != 0) return false;
    detruire_pile(&p);

    return true;
}

bool test_csv_fichier_invalide(void)
{
    Pile p;
    p = lire_tuiles_csv("data/test/3_test.csv");

    if (p.nb_element_max != 0) return false;

    detruire_pile(&p);
    return true;
}

// ajout à la liste de tests à executer
Test unit_tests[] = {
    TEST(test_tuile_creer),
    TEST(test_tuile_pivot90),
    TEST(test_tuile_compatibilite),
    TEST(test_tuile_generer),
    TEST(test_pile_creer),
    TEST(test_pile_creer_aleatoire),
    TEST(test_pile_inserer_tuile),
    TEST(test_pile_recuperer_tuile_non_aleatoire),
    TEST(test_pile_recuperer_tuile_aleatoire),
    TEST(test_vec_creer),
    TEST(test_vec_get_null),
    TEST(test_vec_set_get),
    TEST(test_vec2D_creer),
    TEST(test_vec2D_get_null),
    TEST(test_vec2D_set_get),
    TEST(test_grille_placer_tuile),
    TEST(test_varstring_ajouter_char),
    TEST(test_varstring_retirer_char),
    TEST(test_varstring_ajouter_chaine),
    TEST(test_varstring_vider),
    TEST(test_varstring_ajouter_null),
    TEST(test_varstring_null),
    TEST(test_csv_compter_lignes),
    TEST(test_csv_lecture_zone),
    TEST(test_csv_lecture_fichier),
    TEST(test_csv_fichier_vide),
    TEST(test_csv_fichier_introuvable),
    TEST(test_csv_fichier_invalide),
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
