#ifndef meeple_h
#define meeple_h

#include "tuile.h"
#include "vec.h"

struct _maillon {
    struct _maillon *next;
    int x;
    int y;
    enum Direction d;
};
typedef struct _maillon *L_meeple;

typedef struct _Joueur{
    char id;
    int pts;
    int nb_meeple_restant;
    L_meeple localisation_meeples;
} Joueur;

void ajout_meeple_chaine(Joueur *joueur, L_meeple new);
void ajout_meeple_tuile(Joueur *joueur, Vec2D grille, int x, int y, enum Direction d);
void detruire_meeple(Joueur *joueur, int x, int y);
// TODO: void detruire_joueur(Joueur joueur);

#endif
