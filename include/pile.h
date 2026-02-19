#ifndef pile_h
#define pile_h

#include "grille.h"
#include <stdbool.h>

typedef struct _Pile {
    int nb_element, nb_element_max;
    Tuile *tab;

} Pile;

Pile creer_pile(int max_element);

bool pile_vide(Pile P);

bool pile_pleine(Pile P);

bool acceder_sommet(Pile P, Tuile *T);

int empiler(Pile P, Tuile T);

bool depiler(Pile P, Tuile T);

void vider(Pile *P);

void detruire(Pile P);

#endif