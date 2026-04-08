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

void attribution_points(Jeu *jeu, L_meeple loc_meeple, int *nb_meeples, int pts, bool fin);
bool tour(Jeu *jeu, Tuile tuile, int x, int y, bool place_meeple, enum Direction position_meeple);
int fin(Jeu *jeu);

int maximal(int *nb_meeples, int nb_joueur);

#endif
