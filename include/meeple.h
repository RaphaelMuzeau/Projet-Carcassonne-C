#ifndef meeple_h
#define meeple_h

#include "tuile.h"

struct _maillon {
    struct _maillon *next;
    int x;
    int y;
    enum Direction d;
};
typedef struct _maillon *L_meeple;

typedef struct _Joueur{
    char color;
    int compteur;
    int nb_meeple_restant;
    L_meeple localisation_meeples;
} Joueur;

#endif
