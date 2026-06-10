#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "libca.h"
#include "vec.h"

/* invariant à retenir:
 * Un Vec admet que pour tout i entre 0 et capacite exclus,
 * tableau[i] contient une valeur valide (NULL ou une Tuile)
 */

/* Vec */

Vec creer_vec(void)
{
    return (Vec) { 0 };
}

void detruire_vec(Vec v)
{
    for (int i = 0; i < v._capacite; i++)
        free(v._tableau[i]);
    free(v._tableau);
}

// Alloue si besoin l'espace necessaire pour pouvoir acceuillir taille elements
static void agrandir_vec(Vec *v, int taille)
{
    // arrondit la nouvelle capacite au prochain multiple de VEC_REALLOC_NB
    int new_cap = taille + (VEC_REALLOC_NB - taille % VEC_REALLOC_NB);

    v->_tableau = ca_realloc(v->_tableau, new_cap, sizeof(Tuile));
    for (int i = v->_capacite; i < new_cap; i++)
        v->_tableau[i] = NULL;

    v->_capacite = new_cap;
}

Tuile vget(Vec v, int y)
{
    int index = y + v._decy;
    if (index < 0 || index >= v._capacite)
        return NULL;
    return v._tableau[index];
}

void vset(Vec *v, Tuile t, int y)
{
    long index = (long) y + v->_decy; // /!\ sur windows, int et long ont la meme taille :)
    assert(index <= INT_MAX && "Placement au dela des valeurs maximales de la grille");

    if (index >= v->_capacite)
        agrandir_vec(v, index + 1);
    else if (index < 0) { // il faut etendre le tableau et decaler les valeurs vers la droite
        int old_cap = v->_capacite;
        agrandir_vec(v, v->_capacite + (-index));
        int diff_cap = v->_capacite - old_cap;

        v->_decy += diff_cap;

        // decale les anciennes valeurs et les remplace par NULL
        memmove(v->_tableau + diff_cap, v->_tableau, old_cap * sizeof(Tuile));
        for (int i = 0; i < diff_cap; i++)
            v->_tableau[i] = NULL;

        index = y + v->_decy;
    }
    v->_tableau[index] = t;
}

/* Vec2D, presque des copies verbatim des fonctions de Vec. */

Vec2D creer_vec2D(void)
{
    return (Vec2D) { 0 };
}

void detruire_vec2D(Vec2D g)
{
    for (int i = 0; i < g._capacite; i++)
        detruire_vec(g._tableau[i]);
    free(g._tableau);
}

// Alloue si besoin l'espace necessaire pour pouvoir acceuillir taille elements
static void agrandir_vec2D(Vec2D *g, int taille)
{
    // arrondit la nouvelle capacite au prochain multiple de VEC_REALLOC_NB
    int new_cap = taille + (VEC2D_REALLOC_NB - taille % VEC2D_REALLOC_NB);

    g->_tableau = ca_realloc(g->_tableau, new_cap, sizeof(Vec));
    for (int i = g->_capacite; i < new_cap; i++)
        g->_tableau[i] = creer_vec();

    g->_capacite = new_cap;
}

Tuile get(Vec2D g, int x, int y)
{
    int index = x + g._decx;
    if (index < 0 || index >= g._capacite)
        return NULL;
    return vget(g._tableau[index], y);
}

void set(Vec2D *g, Tuile t, int x, int y)
{
    long index = (long) x + g->_decx;
    assert(index <= INT_MAX && "Placement au dela des valeurs maximales de la grille");

    if (index >= g->_capacite)
        agrandir_vec2D(g, index + 1);
    else if (index < 0) { // il faut etendre le tableau et decaler les valeurs vers la droite
        int old_cap = g->_capacite;
        agrandir_vec2D(g, g->_capacite + (-index));
        int diff_cap = g->_capacite - old_cap;

        g->_decx += diff_cap;

        // decale les anciens Vecs et les remplace par des vecteurs vierges
        memmove(g->_tableau + diff_cap, g->_tableau, old_cap * sizeof(Vec));
        for (int i = 0; i < diff_cap; i++)
            g->_tableau[i] = creer_vec();

        index = x + g->_decx;
    }
    vset(&g->_tableau[index], t, y);
}
