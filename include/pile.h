#ifndef pile_h
#define pile_h

#include <stdbool.h>
#include "tuile.h"

/* Pile de tuiles principale du jeu.
 * Peut être créée en mode gen_aleatoire pour
 * générer aléatoirement une tuile à chaque pioche.
 * Ne doit pas être manipulée directement.
 */
typedef struct _Pile {
    Tuile *_tab;
    bool _gen_aleatoire;
    int nb_element, _nb_element_max;
} Pile;

Pile creer_pile(int max_element, bool gen);

bool pile_vide(Pile p);

bool pile_pleine(Pile p);

Tuile recup_tuile(Pile *p);

bool inserer_tuile(Pile *p, Tuile t);

void detruire_pile(Pile *p);

#endif
