#include <stdlib.h>
#include <stdbool.h>
#include "pile.h"

pile createPile(int max_element)
{
    pile P;
    P.nb_element_max = max_element;
    P.nb_element = 0;
    P.tab = malloc(sizeof(struct tuile)*max_element);

    return P;
}

bool estVide(pile P)
{
    if (P.nb_element == 0)
        return 1;
    return 0;
}

bool estPleine(pile P)
{
    if (P.nb_element == P.nb_element_max)
        return 1;
    return 0;
}

bool accederSommet(pile P, struct tuile *T)
{
    if(estVide(P))
        return 0;
    else
        *T = P.tab[P.nb_element-1];
    return 1;
}

bool empiler(pile P, struct tuile T)
{
    if (estPleine(P))
        return 0;
    else {
        P.tab[P.nb_element] = T;
        P.nb_element++;
    }
    return 1;
}

bool depiler(pile P, struct tuile *T)
{
    if (estVide(P))
        return 0;
    else {
        accederSommet(P,T);
        P.nb_element--;
    }
    return 1;
}

void vider(pile P)
{

    P.nb_element = 0;
}

void detruire(pile P)
{

    if (P.nb_element_max != 0) {
        vider(P);
        free(P.tab);
        P.nb_element_max = 0;
    }
}