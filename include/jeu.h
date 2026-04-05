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

Jeu creer_jeu(int nb_joueurs, int nb_meeple, int nb_tuiles);
void detruire_jeu(Jeu jeu);

bool tour(Jeu *jeu, Tuile tuile, int x, int y, int id_meeple, enum Direction position_meeple, bool fin);
int maximal(int *nb_meeples, int nb_joueur);

#endif
