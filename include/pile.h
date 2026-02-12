#ifndef pile_h
#define pile_h

typedef struct _pile {
    int nb_element, nb_element_max;
    struct tuile *tab;

} pile;

pile createPile(max_element);


#endif
