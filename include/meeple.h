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

L_meeple creer_maillon_meeple(int x, int y, enum Direction d);
void detruire_liste_meeple(L_meeple liste);

void ajouter_maillon_meeple(L_meeple *liste, L_meeple new);
void retirer_maillon_meeple(L_meeple *liste, int x, int y);

#endif
