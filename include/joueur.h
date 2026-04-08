#ifndef joueur_h
#define joueur_h

#include "raylib.h"
#include "meeple.h"

typedef struct _Joueur {
    char *nom;
    int id;
    int pts;
    int nb_meeple_restant;
    L_meeple localisation_meeple;
    Color couleur;
} Joueur;

typedef struct _ListeJoueurs {
    int nb_joueurs;
    Joueur *tableau;
    int tour;
} ListeJoueurs;

Joueur creer_joueur(int id, int nb_meeple);
void detruire_joueur(Joueur joueur);

ListeJoueurs creer_listejoueurs(int nb_joueurs, int nb_meeple);
void detruire_listejoueurs(ListeJoueurs joueurs);

#endif
