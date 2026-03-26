#ifndef jeu_h
#define jeu_h

#include "tuile.h"
#include "pile.h"
#include "vec.h"
#include "meeple.h"

typedef struct _Jeu {
    ListeJoueurs listejoueurs;
    Pile pile;
    Vec2D grille;
} Jeu;

void round_encapsuler(Jeu *jeu, int id_joueur, int x, int y, Tuile tuile, enum Direction d);

#endif
