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
#include "grille.h"
#include "pile.h"
#include "tuile.h"
#include "varstring.h"
#include "vec.h"
#include "csv.h"
#include "meeple.h"
#include "joueur.h"

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
    if (t->id_meeple != -1) return false;

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

bool test_pile_creer(void)
{
    Pile p = creer_pile(10);

    if (!pile_vide(p)) return false;
    if (p.nb_element_max != 10) return false;
    if (p.nb_element != 0) return false;

    detruire_pile(&p);
    return true;
}

bool test_pile_inserer_tuile(void)
{
    Pile p = creer_pile(10);
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

bool test_pile_recuperer_tuile(void)
{
    Pile p = creer_pile(5);
    Tuile t = creer_tuile();

    inserer_tuile(&p, t);
    if (recup_tuile(&p) != t) return false;
    if (p.nb_element != 0) return false;
    if (recup_tuile(&p) != NULL) return false;

    free(t);
    detruire_pile(&p);
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
    Tuile t0 = creer_tuile();

    // acces simple
    vset(&v, t0, 0);
    if (v.tableau == NULL) return false;
    if (v.capacite == 0) return false;
    if (v.capacite != VEC_REALLOC_NB) return false;
    if (v.decy != 0) return false;

    if (v.tableau[0] != t0) return false;
    if (vget(v, 0) != t0) return false;

    // acces complexes
    Tuile t1 = creer_tuile();
    vset(&v, t1, -50);

    if (v.capacite % VEC_REALLOC_NB != 0) return false;
    if (v.tableau[  0 + v.decy] != t0) return false;
    if (v.tableau[-50 + v.decy] != t1) return false;
    if (vget(v,   0) != t0) return false;
    if (vget(v, -50) != t1) return false;

    Tuile t2 = creer_tuile();
    vset(&v, t2, 50);

    if (v.capacite % VEC_REALLOC_NB != 0) return false;
    if (v.tableau[  0 + v.decy] != t0) return false;
    if (v.tableau[-50 + v.decy] != t1) return false;
    if (v.tableau[ 50 + v.decy] != t2) return false;
    if (vget(v,   0) != t0) return false;
    if (vget(v, -50) != t1) return false;
    if (vget(v,  50) != t2) return false;

    Tuile t3 = creer_tuile();
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
    Tuile t0 = creer_tuile();

    // acces simple
    set(&g, t0, 0, 0);
    if (g.tableau == NULL) return false;
    if (g.capacite == 0) return false;
    if (g.capacite != VEC2D_REALLOC_NB) return false;
    if (g.decx != 0) return false;

    if (g.tableau[0].tableau[0] != t0) return false;
    if (get(g, 0, 0) != t0) return false;

    // acces complexes
    Tuile t1 = creer_tuile();
    set(&g, t1, -50, -50);

    if (g.capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;

    Tuile t2 = creer_tuile();
    set(&g, t2, 50, -50);

    if (g.capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;
    if (get(g,  50, -50) != t2) return false;

    Tuile t3 = creer_tuile();
    set(&g, t3, -50, 50);

    if (g.capacite % VEC2D_REALLOC_NB != 0) return false;
    if (get(g,   0,   0) != t0) return false;
    if (get(g, -50, -50) != t1) return false;
    if (get(g,  50, -50) != t2) return false;
    if (get(g, -50,  50) != t3) return false;

    Tuile t4 = creer_tuile();
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
    Pile p;
    p = lire_tuiles_csv("data/test/1_test.csv");

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

bool test_joueur_creer(void)
{
    Joueur joueur = creer_joueur(2, 5);

    if (joueur.nom != NULL) return false;
    if (joueur.id != 2)  return false;
    if (joueur.pts != 0) return false;
    if (joueur.nb_meeple_restant != 5) return false;
    if (joueur.localisation_meeples != NULL) return false;

    detruire_joueur(joueur);
    return true;
}

bool test_listejoueurs_creer(void)
{
    ListeJoueurs joueurs  = creer_listejoueurs(3, 5);

    if (joueurs.nb_joueurs != 3) return false;
    if (joueurs.tableau == NULL) return false;

    if (joueurs.tableau[0].id != 0) return false;
    if (joueurs.tableau[1].id != 1) return false;
    if (joueurs.tableau[2].id != 2) return false;
    if (joueurs.tableau[0].nb_meeple_restant != 5) return false;
    if (joueurs.tableau[1].nb_meeple_restant != 5) return false;
    if (joueurs.tableau[2].nb_meeple_restant != 5) return false;

    detruire_listejoueurs(joueurs);
    return true;
}

bool test_liste_meeple_creer(void)
{
    L_meeple liste = creer_maillon_meeple(1, 5, D_EST);

    if (liste->x != 1)       return false;
    if (liste->y != 5)       return false;
    if (liste->d != D_EST)   return false;
    if (liste->next != NULL) return false;

    detruire_liste_meeple(liste);
    return true;
}

bool test_liste_meeple_ajouter(void)
{
    L_meeple liste = NULL;

    L_meeple maillon1 = creer_maillon_meeple(1, 1, D_MILIEU);
    ajouter_maillon_meeple(&liste, maillon1);

    if (liste != maillon1)    return false;
    if (liste->x != 1)        return false;
    if (liste->y != 1)        return false;
    if (liste->d != D_MILIEU) return false;
    if (liste->next != NULL)  return false;

    L_meeple maillon2 = creer_maillon_meeple(2, 2, D_SUD);
    ajouter_maillon_meeple(&liste, maillon2);

    if (liste != maillon2)       return false;
    if (liste->x != 2)           return false;
    if (liste->y != 2)           return false;
    if (liste->d != D_SUD)    return false;
    if (liste->next != maillon1) return false;

    detruire_liste_meeple(liste);
    return true;
}

bool test_liste_meeple_retirer(void)
{
    L_meeple liste = NULL;

    // retrait sur liste vide
    retirer_maillon_meeple(&liste, 0, 0);
    if (liste != NULL) return false;

    L_meeple maillon1 = creer_maillon_meeple(1, 1, D_MILIEU);
    L_meeple maillon2 = creer_maillon_meeple(2, 2, D_SUD);
    ajouter_maillon_meeple(&liste, maillon1);
    ajouter_maillon_meeple(&liste, maillon2);

    // retrait de meeple non present
    retirer_maillon_meeple(&liste, 0, 0);
    if (liste != maillon2) return false;
    if (liste->next != maillon1) return false;

    // retrait de meeple en milieu de chaine
    retirer_maillon_meeple(&liste, 2, 2);
    if (liste != maillon1) return false;
    if (liste->next != NULL) return false;

    // retrait de meeple en fin de chaine
    L_meeple maillon3 = creer_maillon_meeple(3, 3, D_NORD);
    ajouter_maillon_meeple(&liste, maillon3);

    retirer_maillon_meeple(&liste, 1, 1);
    if (liste != maillon3) return false;
    if (liste->next != NULL) return false;

    // retrait de tous les meeple
    retirer_maillon_meeple(&liste, 3, 3);
    if (liste != NULL) return false;

    return true;
}

bool test_grille_placer_tuile(void)
{
    Vec2D grille = creer_vec2D();
    Tuile t = creer_tuile();

    // placement sans connexion
    if (placer_tuile(&grille, 1, 1, t)) return false;
    if (placer_tuile(&grille, 1, 0, t)) return false;
    if (placer_tuile(&grille, 0, 1, t)) return false;
    if (placer_tuile(&grille, 1, 9, t)) return false;
    if (placer_tuile(&grille, 9, 1, t)) return false;
    if (get(grille, 1, 1) == t) return false;
    if (get(grille, 1, 0) == t) return false;
    if (get(grille, 0, 1) == t) return false;
    if (get(grille, 1, 9) == t) return false;
    if (get(grille, 9, 1) == t) return false;

    set(&grille, t, 1, 1);

    // tuile occupée
    if (placer_tuile(&grille, 1, 1, t)) return false;

    // placement compatible
    t = creer_tuile();
    if (!placer_tuile(&grille, 0, 1, t)) return false;
    if (get(grille, 0, 1)  != t) return false;
    t = creer_tuile();
    if (!placer_tuile(&grille, 1, 0, t)) return false;
    if (get(grille, 1, 0)  != t) return false;
    t = creer_tuile();
    if (!placer_tuile(&grille, 2, 1, t)) return false;
    if (get(grille, 2, 1) != t) return false;
    t = creer_tuile();
    if (!placer_tuile(&grille, 1, 2, t)) return false;
    if (get(grille, 1, 2) != t) return false;

    // placement incompatible
    t = creer_tuile();
    t->nord = Z_VILLE; t->sud = Z_VILLE; t->est = Z_VILLE; t->ouest = Z_VILLE;
    set(&grille, t, 5, 5);

    t = creer_tuile();
    if (placer_tuile(&grille, 0, 1, t)) return false;
    if (get(grille, 0, 1)  == t) return false;
    if (placer_tuile(&grille, 1, 0, t)) return false;
    if (get(grille, 1, 0)  == t) return false;
    if (placer_tuile(&grille, 2, 1, t)) return false;
    if (get(grille, 2, 1)  == t) return false;
    if (placer_tuile(&grille, 1, 2, t)) return false;
    if (get(grille, 1, 2) == t) return false;

    free(t);

    detruire_vec2D(grille);
    return true;
}

bool test_grille_placer_meeple(void)
{
    Vec2D grille = creer_vec2D();
    Joueur joueur = creer_joueur(0, 2);

    // placement sur tuile vide
    if (placer_meeple(grille, &joueur, 0, 0, D_MILIEU)) return false;
    if (joueur.localisation_meeples != NULL) return false;
    if (joueur.nb_meeple_restant != 2) return false;

    // placement sur tuile occupé
    Tuile t0 = creer_tuile();
    t0->id_meeple = 1;
    set(&grille, t0, 0, 0);

    if (placer_meeple(grille, &joueur, 0, 0, D_MILIEU)) return false;
    if (joueur.localisation_meeples != NULL) return false;
    if (joueur.nb_meeple_restant != 2) return false;

    // placements possibles
    Tuile t1 = creer_tuile();
    set(&grille, t1, 1, 0);

    if (!placer_meeple(grille, &joueur, 1, 0, D_NORD)) return false;

    if (joueur.localisation_meeples->x != 1)      return false;
    if (joueur.localisation_meeples->y != 0)      return false;
    if (joueur.localisation_meeples->d != D_NORD) return false;
    if (joueur.nb_meeple_restant != 1) return false;

    if (t1->id_meeple != joueur.id) return false;
    if (t1->position_meeple != D_NORD) return false;

    Tuile t2 = creer_tuile();
    set(&grille, t2, 0, 1);

    if (!placer_meeple(grille, &joueur, 0, 1, D_SUD)) return false;

    if (joueur.localisation_meeples->x != 0)            return false;
    if (joueur.localisation_meeples->y != 1)            return false;
    if (joueur.localisation_meeples->d != D_SUD)        return false;
    if (joueur.localisation_meeples->next->x != 1)      return false;
    if (joueur.localisation_meeples->next->y != 0)      return false;
    if (joueur.localisation_meeples->next->d != D_NORD) return false;
    if (joueur.nb_meeple_restant != 0) return false;


    if (t2->id_meeple != joueur.id) return false;
    if (t2->position_meeple != D_SUD) return false;

    // placement sans meeple restant
    Tuile t3 = creer_tuile();
    set(&grille, t3, 1, 1);

    if (placer_meeple(grille, &joueur, 1, 1, D_MILIEU)) return false;

    if (joueur.localisation_meeples->x != 0)             return false;
    if (joueur.localisation_meeples->y != 1)             return false;
    if (joueur.localisation_meeples->d != D_SUD)         return false;
    if (joueur.localisation_meeples->next->x != 1)       return false;
    if (joueur.localisation_meeples->next->y != 0)       return false;
    if (joueur.localisation_meeples->next->d != D_NORD)  return false;
    if (joueur.localisation_meeples->next->next != NULL) return false;
    if (joueur.nb_meeple_restant != 0) return false;

    detruire_joueur(joueur);
    detruire_vec2D(grille);
    return true;
}

bool test_grille_retirer_meeple(void)
{
    Vec2D grille = creer_vec2D();
    ListeJoueurs joueurs = creer_listejoueurs(2, 2);

    // retrait sur tuile vide
    retirer_meeple(grille, joueurs, 0, 0);
    if (joueurs.tableau[0].nb_meeple_restant != 2) return false;
    if (joueurs.tableau[1].nb_meeple_restant != 2) return false;

    // retrait normal
    Tuile t1 = creer_tuile();
    set(&grille, t1, 1, 0);
    placer_meeple(grille, &joueurs.tableau[1], 1, 0, D_NORD);

    retirer_meeple(grille, joueurs, 1, 0);
    if (joueurs.tableau[1].localisation_meeples != NULL) return false;
    if (joueurs.tableau[1].nb_meeple_restant != 2) return false;
    if (t1->id_meeple != -1) return false;

    detruire_listejoueurs(joueurs);
    detruire_vec2D(grille);
    return true;
}


// ajout à la liste de tests à executer
Test unit_tests[] = {
    TEST(test_tuile_creer),
    TEST(test_tuile_compatibilite),
    TEST(test_pile_creer),
    TEST(test_pile_inserer_tuile),
    TEST(test_pile_recuperer_tuile),
    TEST(test_vec_creer),
    TEST(test_vec_get_null),
    TEST(test_vec_set_get),
    TEST(test_vec2D_creer),
    TEST(test_vec2D_get_null),
    TEST(test_vec2D_set_get),
    TEST(test_varstring_ajout_char),
    TEST(test_varstring_ajout_chaine),
    TEST(test_varstring_vider),
    TEST(test_varstring_ajouter_null),
    TEST(test_varstring_null),
    TEST(test_csv_compter_lignes),
    TEST(test_csv_lecture_zone),
    TEST(test_csv_lecture_fichier),
    TEST(test_csv_fichier_vide),
    TEST(test_csv_fichier_introuvable),
    TEST(test_csv_fichier_invalide),
    TEST(test_joueur_creer),
    TEST(test_listejoueurs_creer),
    TEST(test_liste_meeple_creer),
    TEST(test_liste_meeple_ajouter),
    TEST(test_liste_meeple_retirer),
    TEST(test_grille_placer_tuile),
    TEST(test_grille_placer_meeple),
    TEST(test_grille_retirer_meeple),
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
