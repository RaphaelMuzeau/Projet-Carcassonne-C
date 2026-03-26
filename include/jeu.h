#ifndef jeu_h
#define jeu_h

#include "pile.h"
#include "vec.h"
#include "meeple.h"

typedef struct _Jeu {
    ListeJoueurs listejoueurs;
    Pile pile;
    Vec2D grille;
} Jeu;

#endif
