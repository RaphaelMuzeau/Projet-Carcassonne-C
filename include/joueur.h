#ifndef joueur_h
#define joueur_h

#include "meeple.h"

typedef struct _Joueur {
    int id;
    int pts;
    int nb_meeple_restant;
    L_meeple localisation_meeples;
} Joueur;

typedef struct _ListeJoueur {
    int nb_joueur;
    Joueur *tableau;
} ListeJoueurs;

Joueur creer_joueur(int id, int nb_meeple);
void detruire_joueur(Joueur joueur);

#endif
