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
    for (int i = 0; i < v.capacite; i++)
        free(v.tableau[i]);
    free(v.tableau);
}

// fonction privée,
// Alloue si besoin l'espace necessaire pour pouvoir acceuillir taille elements
void agrandir_vec(Vec *v, int taille)
{
    // arrondit la nouvelle capacite au prochain multiple de VEC_REALLOC_NB
    int new_cap = taille + (VEC_REALLOC_NB - taille % VEC_REALLOC_NB);

    v->tableau = ca_realloc(v->tableau, new_cap, sizeof(Tuile));
    for (int i = v->capacite; i < new_cap; i++)
        v->tableau[i] = NULL;

    v->capacite = new_cap;
}

Tuile vget(Vec v, int y)
{
    int index = y + v.decy;
    if (index < 0 || index >= v.capacite)
        return NULL;
    return v.tableau[index];
}

void vset(Vec *v, Tuile t, int y)
{
    long index = (long) y + v->decy; // /!\ sur windows, int et long ont la meme taille :)
    assert(index <= INT_MAX && "Placement au dela des valeurs maximales de la grille");

    if (index >= v->capacite)
        agrandir_vec(v, index + 1);
    else if (index < 0) { // il faut etendre le tableau et decaler les valeurs vers la droite
        int old_cap = v->capacite;
        agrandir_vec(v, v->capacite + (-index));
        int diff_cap = v->capacite - old_cap;

        v->decy += diff_cap;

        // decale les anciennes valeurs et les remplace par NULL
        memmove(v->tableau + diff_cap, v->tableau, old_cap * sizeof(Tuile));
        for (int i = 0; i < old_cap; i++)
            v->tableau[i] = NULL;

        index = y + v->decy;
    }
    v->tableau[index] = t;
}

/* Vec2D, presque des copies verbatim des fonctions de Vec. */

Vec2D creer_vec2D(void)
{
    return (Vec2D) { 0 };
}

void detruire_vec2D(Vec2D g)
{
    for (int i = 0; i < g.capacite; i++)
        detruire_vec(g.tableau[i]);
    free(g.tableau);
}

// fonction privée,
// Alloue si besoin l'espace necessaire pour pouvoir acceuillir taille elements
void agrandir_vec2D(Vec2D *g, int taille)
{
    // arrondit la nouvelle capacite au prochain multiple de VEC_REALLOC_NB
    int new_cap = taille + (VEC2D_REALLOC_NB - taille % VEC2D_REALLOC_NB);

    g->tableau = ca_realloc(g->tableau, new_cap, sizeof(Vec));
    for (int i = g->capacite; i < new_cap; i++)
        g->tableau[i] = creer_vec();

    g->capacite = new_cap;
}

Tuile get(Vec2D g, int x, int y)
{
    int index = x + g.decx;
    if (index < 0 || index >= g.capacite)
        return NULL;
    return vget(g.tableau[index], y);
}

void set(Vec2D *g, Tuile t, int x, int y)
{
    long index = (long) x + g->decx;
    assert(index <= INT_MAX && "Placement au dela des valeurs maximales de la grille");

    if (index >= g->capacite)
        agrandir_vec2D(g, index + 1);
    else if (index < 0) { // il faut etendre le tableau et decaler les valeurs vers la droite
        int old_cap = g->capacite;
        agrandir_vec2D(g, g->capacite + (-index));
        int diff_cap = g->capacite - old_cap;

        g->decx += diff_cap;

        // decale les anciens Vecs et les remplace par des vecteurs vierges
        memmove(g->tableau + diff_cap, g->tableau, old_cap * sizeof(Vec));
        for (int i = 0; i < old_cap; i++)
            g->tableau[i] = creer_vec();

        index = x + g->decx;
    }
    vset(&g->tableau[index], t, y);
}
