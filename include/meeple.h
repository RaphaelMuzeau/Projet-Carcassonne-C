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

typedef struct _Joueur {
    char id;
    int pts;
    int nb_meeple_restant;
    L_meeple localisation_meeples;
} Joueur;

typedef struct _ListeJoueur {
    int nb_joueur;
    Joueur *tableau;
} ListeJoueurs;

void ajouter_meeple(Joueur *joueur, Vec2D grille, int x, int y, enum Direction d);
void retirer_meeple(Joueur *joueur, Vec2D grille, int x, int y);
void retrait_meeple_liste(Joueur *joueur, Vec2D grille, L_meeple loc_meeple);
void detruire_joueur(Joueur joueur);

#endif
