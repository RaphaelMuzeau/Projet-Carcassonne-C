#ifndef pile_h
#define pile_h

typedef struct _pile {
    int nb_element, nb_element_max;
    struct tuile *tab;

} pile;

pile createPile(int max_element);

bool estVide(pile P);

bool estPleine(pile P);

bool accederSommet(pile P, struct tuile *T);

int empiler(pile P, struct tuile T);

bool depiler(pile P, struct tuile *T);

void vider(pile P);

void detruire(pile P);

#endif