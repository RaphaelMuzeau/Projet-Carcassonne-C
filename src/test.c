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
#include "meeple.h"
#include "joueur.h"
#include "jeu.h"
#include "fichier.h"
#include "gentest.h"
#include "libca.h"

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

        if (tmp->milieu == Z_PRE) {
            if (tmp->est == Z_ROUTE)   return false;
            if (tmp->sud == Z_ROUTE)   return false;
            if (tmp->nord == Z_ROUTE)  return false;
            if (tmp->ouest == Z_ROUTE) return false;
        }

        if (tmp->milieu & (Z_ROUTE | Z_VILLE | Z_BLASON) ) {
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

bool test_varstring_dupliquer_chaine(void)
{
    VarString s = creer_varstring();
    char *dup;

    // duplication de chaine vide
    dup = dupliquer_chaine(&s);
    if (dup != NULL) return false;

    // duplication normal
    ajouter_chaine(&s, "hello");
    dup = dupliquer_chaine(&s);

    if (dup == s.texte) return false;
    if (strcmp(dup, s.texte)) return false;

    free(dup);
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

bool test_joueur_creer(void)
{
    Joueur joueur = creer_joueur(2, 5);

    if (joueur.nom != NULL) return false;
    if (joueur.id != 2)  return false;
    if (joueur.pts != 0) return false;
    if (joueur.couleur.a != CHAR_MAX) return false;
    if (joueur.nb_meeple_restant != 5) return false;
    if (joueur.localisation_meeple != NULL) return false;

    detruire_joueur(joueur);
    return true;
}

bool test_listejoueurs_creer(void)
{
    ListeJoueurs joueurs  = creer_listejoueurs(3, 5);

    if (joueurs.nb_joueurs != 3)    return false;
    if (joueurs.nb_meeple_max != 5) return false;
    if (joueurs.tableau == NULL)    return false;
    if (joueurs.tour != 0)          return false;

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
    L_meeple liste = creer_maillon_meeple(1, 5);

    if (liste->x != 1)       return false;
    if (liste->y != 5)       return false;
    if (liste->next != NULL) return false;

    detruire_liste_meeple(liste);
    return true;
}

bool test_liste_meeple_ajouter(void)
{
    L_meeple liste = NULL;

    L_meeple maillon1 = creer_maillon_meeple(1, 1);
    ajouter_maillon_meeple(&liste, maillon1);

    if (liste != maillon1)    return false;
    if (liste->x != 1)        return false;
    if (liste->y != 1)        return false;
    if (liste->next != NULL)  return false;

    L_meeple maillon2 = creer_maillon_meeple(2, 2);
    ajouter_maillon_meeple(&liste, maillon2);

    if (liste != maillon2)       return false;
    if (liste->x != 2)           return false;
    if (liste->y != 2)           return false;
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

    L_meeple maillon1 = creer_maillon_meeple(1, 1);
    L_meeple maillon2 = creer_maillon_meeple(2, 2);
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
    L_meeple maillon3 = creer_maillon_meeple(3, 3);
    ajouter_maillon_meeple(&liste, maillon3);

    retirer_maillon_meeple(&liste, 1, 1);
    if (liste != maillon3) return false;
    if (liste->next != NULL) return false;

    // retrait de tous les meeple
    retirer_maillon_meeple(&liste, 3, 3);
    if (liste != NULL) return false;

    return true;
}

bool test_liste_meeple_verification_presence_meeple(void)
{
    L_meeple loc_meeple;
    loc_meeple = creer_maillon_meeple(0,0);

    if(!verification_presence_meeple(&loc_meeple,0,0)) return false;

    L_meeple maillon2;
    maillon2 = creer_maillon_meeple(0,1);
    ajouter_maillon_meeple(&loc_meeple,maillon2);
    if(!verification_presence_meeple(&loc_meeple,0,1)) return false;

    if(verification_presence_meeple(&loc_meeple,9,0)) return false;
    if(verification_presence_meeple(&loc_meeple,0,9)) return false;

    retirer_maillon_meeple(&loc_meeple,0,1);
    retirer_maillon_meeple(&loc_meeple,0,0);

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
    if (placer_tuile(&grille, 5, 6, t)) return false;
    if (get(grille, 5, 6)  == t) return false;
    if (placer_tuile(&grille, 4, 5, t)) return false;
    if (get(grille, 4, 5)  == t) return false;
    if (placer_tuile(&grille, 5, 4, t)) return false;
    if (get(grille, 5, 4)  == t) return false;
    if (placer_tuile(&grille, 6, 5, t)) return false;
    if (get(grille, 6, 5) == t) return false;

    free(t);

    detruire_vec2D(grille);
    return true;
}

bool test_grille_placer_meeple(void)
{
    Vec2D grille = creer_vec2D();
    Joueur joueur = creer_joueur(0, 3);

    // placement sur tuile vide
    if (placer_meeple(grille, &joueur, 0, 0, D_MILIEU)) return false;
    if (joueur.localisation_meeple != NULL) return false;
    if (joueur.nb_meeple_restant != 3) return false;

    // placement sur zone invalide
    Tuile t = creer_tuile();
    t->milieu = Z_VILLAGE;
    set(&grille, t, 0, -1);

    if (placer_meeple(grille, &joueur, 0, -1, D_MILIEU)) return false;
    if (joueur.localisation_meeple != NULL) return false;
    if (joueur.nb_meeple_restant != 3) return false;
    if (t->id_meeple != -1) return false;

    t = creer_tuile();
    set(&grille, t, -1, -1);

    if (placer_meeple(grille, &joueur, -1, -1, D_MILIEU)) return false;
    if (joueur.localisation_meeple != NULL) return false;
    if (joueur.nb_meeple_restant != 3) return false;
    if (t->id_meeple != -1) return false;

    // placement sur tuile occupé
    t = creer_tuile();
    t->position_meeple = D_MILIEU;
    t->milieu = Z_ABBAYE;
    t->id_meeple = 1;
    set(&grille, t, 0, 0);

    if (placer_meeple(grille, &joueur, 0, 0, D_MILIEU)) return false;
    if (joueur.localisation_meeple != NULL) return false;
    if (joueur.nb_meeple_restant != 3) return false;
    if (t->id_meeple != 1) return false;

    // placements possibles
    t = creer_tuile();
    t->nord = Z_VILLE;
    set(&grille, t, 1, 0);

    if (!placer_meeple(grille, &joueur, 1, 0, D_NORD)) return false;

    if (joueur.localisation_meeple->x != 1)  return false;
    if (joueur.localisation_meeple->y != 0)  return false;
    if (joueur.localisation_meeple->next != NULL)  return false;
    if (joueur.nb_meeple_restant != 2) return false;

    if (t->id_meeple != joueur.id) return false;
    if (t->position_meeple != D_NORD) return false;

    t = creer_tuile();
    t->sud = Z_ROUTE;
    set(&grille, t, 0, 1);

    if (!placer_meeple(grille, &joueur, 0, 1, D_SUD)) return false;

    if (joueur.localisation_meeple->x != 0)              return false;
    if (joueur.localisation_meeple->y != 1)              return false;
    if (joueur.localisation_meeple->next->x != 1)        return false;
    if (joueur.localisation_meeple->next->y != 0)        return false;
    if (joueur.localisation_meeple->next->next != NULL)  return false;
    if (joueur.nb_meeple_restant != 1) return false;

    if (t->id_meeple != joueur.id) return false;
    if (t->position_meeple != D_SUD) return false;

    t = creer_tuile();
    t->milieu = Z_ABBAYE;
    set(&grille, t, 1, 1);

    if (!placer_meeple(grille, &joueur, 1, 1, D_MILIEU)) return false;

    if (joueur.localisation_meeple->x != 1)                    return false;
    if (joueur.localisation_meeple->y != 1)                    return false;
    if (joueur.localisation_meeple->next->x != 0)              return false;
    if (joueur.localisation_meeple->next->y != 1)              return false;
    if (joueur.localisation_meeple->next->next->x != 1)        return false;
    if (joueur.localisation_meeple->next->next->y != 0)        return false;
    if (joueur.localisation_meeple->next->next->next != NULL)  return false;
    if (joueur.nb_meeple_restant != 0) return false;

    if (t->id_meeple != joueur.id) return false;
    if (t->position_meeple != D_MILIEU) return false;

    // placement sans meeple restant
    t = creer_tuile();
    set(&grille, t, 0, 2);

    if (placer_meeple(grille, &joueur, 0, 2, D_MILIEU)) return false;

    if (joueur.localisation_meeple->x != 1)                    return false;
    if (joueur.localisation_meeple->y != 1)                    return false;
    if (joueur.localisation_meeple->next->x != 0)              return false;
    if (joueur.localisation_meeple->next->y != 1)              return false;
    if (joueur.localisation_meeple->next->next->x != 1)        return false;
    if (joueur.localisation_meeple->next->next->y != 0)        return false;
    if (joueur.localisation_meeple->next->next->next != NULL)  return false;
    if (joueur.nb_meeple_restant != 0) return false;
    if (t->id_meeple != -1) return false;

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

    // retrait sur une tuile sans meeple
    Tuile t = creer_tuile();
    set(&grille, t, 1, 0);

    retirer_meeple(grille, joueurs, 1, 0);
    if (joueurs.tableau[1].localisation_meeple != NULL) return false;
    if (joueurs.tableau[1].nb_meeple_restant != 2) return false;
    if (t->id_meeple != -1) return false;

    // retrait normal
    placer_meeple(grille, &joueurs.tableau[1], 1, 0, D_NORD);

    retirer_meeple(grille, joueurs, 1, 0);
    if (joueurs.tableau[1].localisation_meeple != NULL) return false;
    if (joueurs.tableau[1].nb_meeple_restant != 2) return false;
    if (t->id_meeple != -1) return false;

    detruire_listejoueurs(joueurs);
    detruire_vec2D(grille);
    return true;
}

bool test_grille_recherche(void)
{
    int *nb_meeple = ca_alloc(2, sizeof(int));
    L_meeple loc_meeple = NULL;
    Vec2D grille = { 0 };
    int pts = 0;

    // ville incomplete

    grille = generer_recherche_ville_incomplete();
    pts = recherche(grille, nb_meeple, &loc_meeple, 2, 1, Z_VILLE, D_OUEST, false);

    if (pts != -1) return false;

    if(!verification_presence_meeple(&loc_meeple, 2, 1)) return false;

    detruire_vec2D(grille);
    detruire_liste_meeple(loc_meeple);
    loc_meeple = NULL;
    nb_meeple[0] = 0;

    // vile complete

    grille = generer_recherche_ville_complete();
    pts = recherche(grille, nb_meeple, &loc_meeple, 0, 0, Z_VILLE, D_SUD, false);

    if (pts != 8) return false;
    if (nb_meeple[0] != 1) return false;
    if (nb_meeple[1] != 0) return false;
    if (loc_meeple == NULL) return false;
    if (loc_meeple->x != 0) return false;
    if (loc_meeple->y != 0) return false;
    if (loc_meeple->next != NULL) return false;

    if(!verification_presence_meeple(&loc_meeple, 0, 0)) return false;

    detruire_vec2D(grille);
    detruire_liste_meeple(loc_meeple);
    loc_meeple = NULL;
    nb_meeple[0] = 0;

    // ville avec blason

    grille = generer_recherche_ville_blason();
    pts =  recherche(grille, nb_meeple, &loc_meeple, 0, 2, Z_VILLE, D_EST, false);

    if (pts != 22) return false;
    if (nb_meeple[0] != 2) return false;
    if (nb_meeple[1] != 2) return false;
    if (loc_meeple == NULL) return false;

    if(!verification_presence_meeple(&loc_meeple, 0, 1)) return false;
    if(!verification_presence_meeple(&loc_meeple, -1, 1)) return false;
    if(!verification_presence_meeple(&loc_meeple, 1, 1)) return false;
    if(!verification_presence_meeple(&loc_meeple, 0, 2)) return false;

    detruire_vec2D(grille);
    detruire_liste_meeple(loc_meeple);
    loc_meeple = NULL;
    nb_meeple[0] = 0;
    nb_meeple[1] = 0;

    // route complete, arret sur un village

    grille = generer_route_village();
    pts =  recherche(grille, nb_meeple, &loc_meeple, 0, 0, Z_ROUTE, D_SUD, false);

    if (pts != 5) return false;
    if (nb_meeple[0] != 2) return false;
    if (loc_meeple == NULL) return false;

    if(!verification_presence_meeple(&loc_meeple, 0, 0)) return false;
    if(!verification_presence_meeple(&loc_meeple, 1, 1)) return false;

    detruire_vec2D(grille);
    detruire_liste_meeple(loc_meeple);
    loc_meeple = NULL;
    nb_meeple[0] = 0;
    nb_meeple[1] = 0;

    // route complete, arret sur une ville

    grille = generer_route_ville();
    pts =  recherche(grille, nb_meeple, &loc_meeple, 1, 1, Z_ROUTE, D_EST, false);

    if (pts != 4) return false;
    if (nb_meeple[0] != 1) return false;
    if (nb_meeple[1] != 1) return false;
    if (loc_meeple == NULL) return false;

    if(!verification_presence_meeple(&loc_meeple, 0, 0)) return false;
    if(!verification_presence_meeple(&loc_meeple, 1, 1)) return false;

    detruire_vec2D(grille);
    detruire_liste_meeple(loc_meeple);
    free(nb_meeple);

    return true;
}

bool test_jeu_attribution_points(void)
{
    Jeu jeu = creer_jeu(3, 3, 3);
    L_meeple loc_meeple = creer_maillon_meeple(0,0);
    int *nb_meeple = ca_alloc(jeu.joueurs.nb_joueurs, sizeof(int));

    attribution_points(&jeu, loc_meeple, nb_meeple, 10, false);

    if (jeu.joueurs.tableau[0].pts != 0) return false;
    if (jeu.joueurs.tableau[1].pts != 0) return false;
    if (jeu.joueurs.tableau[2].pts != 0) return false;
    nb_meeple[1] = 2;

    attribution_points(&jeu, loc_meeple, nb_meeple, 10, false);

    if (jeu.joueurs.tableau[1].pts != 10) return false;

    nb_meeple[2] = 2;
    jeu.joueurs.tableau[1].pts = 0;

    attribution_points(&jeu, loc_meeple, nb_meeple, 10, false);

    if (jeu.joueurs.tableau[1].pts != 10) return false;
    if (jeu.joueurs.tableau[1].pts != 10) return false;

    free(nb_meeple);
    detruire_liste_meeple(loc_meeple);
    detruire_jeu(jeu);
    return true;
}

bool test_jeu_tour(void)
{
    Jeu jeu = creer_jeu(3, 1, 4);

    Tuile tuile1 = creer_tuile();
    tuile1->milieu = Z_VILLAGE;
    tuile1->sud = Z_ROUTE;

    Tuile tuile2 = creer_tuile();
    tuile2->sud = Z_VILLE;
    tuile2->milieu = Z_VILLE;
    tuile2->est = Z_VILLE;
    tuile2->ouest = Z_VILLE;
    tuile2->nord = Z_ROUTE;

    Tuile tuile3 = creer_tuile();
    tuile3->milieu = Z_ROUTE;
    tuile3->nord = Z_ROUTE;
    tuile3->sud = Z_ROUTE;
    Tuile tuile4 = creer_tuile();
    set(&jeu.grille, tuile1, 0, 0);

    tour(&jeu, tuile3, 1, 0, true, D_NORD);
    tour(&jeu, tuile2, 0, 1, true, D_NORD);
    tour(&jeu,tuile4, 0, -1, false, D_MILIEU);
    if (jeu.joueurs.tableau[0].pts != 0) return false;

    if (fin(&jeu) != 1) return false;
    if (jeu.joueurs.tableau[1].pts != 2) return false;
    if (jeu.joueurs.tableau[0].pts != 1) return false;
    detruire_jeu(jeu);

    return true;
}

bool test_grille_recherche_abbaye_complete(void)
{
    Vec2D grille = generer_recherche_abbaye_complete();
    ListeJoueurs listejoueur = creer_listejoueurs(1,1);

    recherche_abbaye(grille, listejoueur, 0, 1, false);

    if (listejoueur.tableau[0].pts != 9) return false;

    detruire_listejoueurs(listejoueur);
    detruire_vec2D(grille);
    return true;
}

bool test_grille_recherche_abbaye_non_complete(void)
{
    Vec2D grille = generer_recherche_abbaye_non_complete();
    ListeJoueurs listejoueur = creer_listejoueurs(1,1);

    recherche_abbaye(grille, listejoueur, 0, 1, false);

    if (listejoueur.tableau[0].pts != 0) return false;

    recherche_abbaye(grille, listejoueur, 0, 1, true);

    if (listejoueur.tableau[0].pts != 4) return false;

    detruire_listejoueurs(listejoueur);
    detruire_vec2D(grille);
    return true;
}

bool test_grille_verification_abbaye(void)
{
    Vec2D grille = generer_recherche_abbaye_complete();
    ListeJoueurs listejoueur = creer_listejoueurs(1,1);

    verification_abbaye(grille, listejoueur, 0, 0);

    if (listejoueur.tableau[0].pts != 9) return false;

    detruire_listejoueurs(listejoueur);
    detruire_vec2D(grille);
    return true;
}

bool test_jeu_maximal(void)
{
    int nb_joueur = 3;
    int nb_meeple[3];
    nb_meeple[0] = 2;
    nb_meeple[1] = 1;
    nb_meeple[2] = 3;
    if (maximal(nb_meeple, nb_joueur) != 3) return false;
    nb_joueur = 1;
    nb_meeple[0] = 1;
    if (maximal(nb_meeple,nb_joueur) != 1) return false;

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
    if (!lire_tuiles_csv("data/test/1_test.csv", &p)) return false;

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
    if (!lire_tuiles_csv("data/test/2_test.csv", &p)) return false;

    if (p.nb_element_max != 0) return false;
    detruire_pile(&p);

    return true;
}

bool test_csv_fichier_introuvable(void)
{
    Pile p = creer_pile(0, 0);
    if (lire_tuiles_csv("data/test/test_non_existant.csv", &p)) return false;

    if (p.nb_element_max != 0) return false;
    detruire_pile(&p);

    return true;
}

bool test_csv_fichier_invalide(void)
{
    Pile p;
    if (lire_tuiles_csv("data/test/3_test.csv", &p)) return false;

    if (p.nb_element_max != 0) return false;

    detruire_pile(&p);
    return true;
}

bool test_fichier_charger_grille(void)
{
    generer_fichier_grille();
    FILE *f = fopen("data/test/fichier_test_grille.bin", "r");
    Vec2D grille = charger_grille(f);

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            Tuile tmp = get(grille, i, j);
            if (tmp->sud != Z_PRE) return false;
            if (tmp->nord != Z_PRE) return false;
            if (tmp->milieu != Z_PRE) return false;
            if (tmp->est != Z_PRE) return false;
            if (tmp->ouest != Z_PRE) return false;
        }
    }

    fclose(f);
    detruire_vec2D(grille);
    remove("data/test/fichier_test_grille.bin");
    return true;
}

bool test_fichier_charger_pile(void)
{
    generer_fichier_pile();
    FILE *f_read = fopen("data/test/fichier_test_pile.bin", "r");
    Pile p2 = charger_pile(f_read);

    if (p2.nb_element_max != 56) return false;
    if (p2.nb_element != 56) return false;
    if (p2.gen_aleatoire) return false;

    for (int i = 0; i < 56; i++) {
        if (p2.tab[i]->sud != Z_PRE) return false;
        if (p2.tab[i]->milieu != Z_PRE) return false;
        if (p2.tab[i]->nord != Z_PRE) return false;
        if (p2.tab[i]->est != Z_PRE) return false;
        if (p2.tab[i]->ouest != Z_PRE) return false;
    }

    fclose(f_read);
    detruire_pile(&p2);
    remove("data/test/fichier_test_pile.bin");
    return true;
}

bool test_fichier_charger_pile_aleatoire(void)
{
    generer_fichier_pile_aleatoire();
    FILE *f = fopen("data/test/fichier_test_pile_aleatoire.bin", "r");

    Pile p = charger_pile(f);
    if (p.nb_element != 32) return false;
    if (p.nb_element_max != 32) return false;
    if (!p.gen_aleatoire) return false;
    if (p.tab != NULL) return false;

    fclose(f);
    remove("data/test/fichier_test_pile_aleatoire.bin");
    return true;
}

bool test_fichier_charger_joueur(void)
{
    generer_fichier_joueur();
    FILE *f_read = fopen("data/test/fichier_test_joueur.bin", "r");
    Joueur test_read = charger_joueur(f_read);

    if (strcmp(test_read.nom, "Damien")) return false;
    if (test_read.pts != 300)            return false;

    L_meeple meeple = test_read.localisation_meeple;
    if (meeple->x != 0)      return false;
    if (meeple->y != 0)      return false;

    meeple = meeple->next;
    if (meeple->x != 0)      return false;
    if (meeple->y != 12)     return false;

    meeple = meeple->next;
    if (meeple->x != -4)     return false;
    if (meeple->y != -8)     return false;

    if (meeple->next != NULL)    return false;

    fclose(f_read);
    detruire_joueur(test_read);
    free(test_read.nom);
    remove("data/test/fichier_test_joueur.bin");
    return true;
}


bool test_fichier_charger_joueur_liste_vide(void)
{
    generer_fichier_joueur_vide();
    FILE *f_read = fopen("data/test/fichier_test_joueur_vide.bin", "r");
    Joueur test_read = charger_joueur(f_read);

    if (strcmp(test_read.nom, "Damien")) return false;
    if (test_read.pts != 300)            return false;
    if (test_read.localisation_meeple)  return false;

    free(test_read.nom);
    detruire_joueur(test_read);
    fclose(f_read);
    remove("data/test/fichier_test_joueur_vide.bin");
    return true;
}

bool test_fichier_sauvegarder_liste_joueurs(void)
{
    generer_fichier_liste_joueurs();
    FILE* f_read = fopen("data/test/fichier_test_liste_joueurs.bin", "r");
    ListeJoueurs tab = charger_liste_joueurs(f_read);
    if (tab.nb_joueurs != 3)    return false;
    if (tab.nb_meeple_max != 4) return false;

    if (strcmp(tab.tableau[0].nom, "Damien")) return false;
    if (tab.tableau[0].id != 0) return false;
    if (tab.tableau[0].nb_meeple_restant != 1) return false;
    if (tab.tableau[0].pts != 300) return false;

    L_meeple meeple = tab.tableau[0].localisation_meeple;
    if (meeple->x != 0)       return false;
    if (meeple->y != 0)       return false;

    meeple = meeple->next;
    if (meeple->x != 0)       return false;
    if (meeple->y != 12)      return false;

    meeple = meeple->next;
    if (meeple->x != -4)      return false;
    if (meeple->y != -8)      return false;

    if (meeple->next != NULL) return false;

    if (strcmp(tab.tableau[1].nom, "Léo")) return false;
    if (tab.tableau[1].id != 1) return false;
    if (tab.tableau[1].nb_meeple_restant != 4) return false;
    if (tab.tableau[1].pts != 22) return false;

    if (tab.tableau[1].localisation_meeple != NULL) return false;

    if (strcmp(tab.tableau[2].nom, "Seth")) return false;
    if (tab.tableau[2].id != 2) return false;
    if (tab.tableau[2].nb_meeple_restant != 3) return false;
    if (tab.tableau[2].pts != 777) return false;

    meeple = tab.tableau[2].localisation_meeple;

    if (meeple->x != 7) return false;
    if (meeple->y != 6) return false;

    fclose(f_read);
    free(tab.tableau[0].nom);
    free(tab.tableau[1].nom);
    free(tab.tableau[2].nom);
    detruire_listejoueurs(tab);
    remove("data/test/fichier_test_liste_joueurs.bin");

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
    TEST(test_varstring_ajouter_char),
    TEST(test_varstring_retirer_char),
    TEST(test_varstring_ajouter_chaine),
    TEST(test_varstring_dupliquer_chaine),
    TEST(test_varstring_vider),
    TEST(test_varstring_ajouter_null),
    TEST(test_varstring_null),
    TEST(test_joueur_creer),
    TEST(test_listejoueurs_creer),
    TEST(test_liste_meeple_creer),
    TEST(test_liste_meeple_ajouter),
    TEST(test_liste_meeple_retirer),
    TEST(test_liste_meeple_verification_presence_meeple),
    TEST(test_grille_placer_tuile),
    TEST(test_grille_placer_meeple),
    TEST(test_grille_retirer_meeple),
    TEST(test_grille_recherche),
    TEST(test_grille_recherche_abbaye_complete),
    TEST(test_grille_recherche_abbaye_non_complete),
    TEST(test_grille_verification_abbaye),
    TEST(test_jeu_maximal),
    TEST(test_jeu_attribution_points),
    TEST(test_jeu_tour),
    TEST(test_csv_compter_lignes),
    TEST(test_csv_lecture_zone),
    TEST(test_csv_lecture_fichier),
    TEST(test_csv_fichier_vide),
    TEST(test_csv_fichier_introuvable),
    TEST(test_csv_fichier_invalide),
    TEST(test_fichier_charger_grille),
    TEST(test_fichier_charger_pile),
    TEST(test_fichier_charger_pile_aleatoire),
    TEST(test_fichier_charger_joueur),
    TEST(test_fichier_charger_joueur_liste_vide),
    TEST(test_fichier_sauvegarder_liste_joueurs),
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
