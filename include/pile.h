#ifndef pile_h
#define pile_h

#include <stdbool.h>
#include "tuile.h"

typedef struct _Pile {
    int nb_element, nb_element_max;
    Tuile *tab;

} Pile;

Pile creer_pile(int max_element);

bool pile_vide(Pile p);

bool pile_pleine(Pile p);

Tuile recup_tuile(Pile *p);

bool inserer_tuile(Pile *p, Tuile t);

void detruire_pile(Pile *p);

#endif
