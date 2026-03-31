#ifndef jeu_h
#define jeu_h

#include "joueur.h"
#include "pile.h"
#include "vec.h"
#include "tuile.h"

typedef struct _Jeu {
    ListeJoueurs listejoueurs;
    Pile pile;
    Vec2D grille;
} Jeu;

bool tour(Jeu *jeu, Tuile tuile, int x, int y, int id_meeple, enum Direction position_meeple);

#endif
