#ifndef jeu_h
#define jeu_h

#include "joueur.h"
#include "pile.h"
#include "vec.h"
#include "tuile.h"

typedef struct _Jeu {
    ListeJoueurs joueurs;
    Pile pile;
    Vec2D grille;
} Jeu;

bool tour(Jeu *jeu, Tuile tuile, int x, int y, int id_meeple, enum Direction position_meeple, bool fin);
Jeu init_jeu(int nb_joueur, int nb_meeple, int taille_pile);

#endif
