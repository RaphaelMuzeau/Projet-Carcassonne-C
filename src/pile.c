#include <stdlib.h>
#include <stdbool.h>
#include "pile.h"
#include "grille.h"

Pile creer_pile(int max_element)
{
    Pile P;
    P.nb_element_max = max_element;
    P.nb_element = 0;
    P.tab = malloc(sizeof(Tuile)*max_element);

    return P;
}

bool pile_vide(Pile P)
{
    if (P.nb_element == 0)
        return true;
    return false;
}

bool pile_pleine(Pile P)
{
    if (P.nb_element == P.nb_element_max)
        return true;
    return false;
}

bool acceder_sommet(Pile P, Tuile *T)
{
    if(pile_vide(P))
        return false;
    else
        *T = P.tab[P.nb_element-1];
    return true;
}

int empiler(Pile P, Tuile T)
{
    if (pile_pleine(P))
        return 0;
    else {
        P.tab[P.nb_element] = T;
        P.nb_element++;
    }
    return 1;
}

bool depiler(Pile P, Tuile T)
{
    if (pile_vide(P))
        return false;
    else {
        acceder_sommet(P,&T);
        P.nb_element--;
    }
    return true;
}

void vider(Pile *P)
{
    int i;
    for (i = 0; i < P->nb_element; i++)
        free(P->tab[i]);

    P->nb_element = 0;
}

void detruire(Pile P)
{

    if (P.nb_element_max != 0) {
        vider(&P);
        free(P.tab);
        P.nb_element_max = 0;
    }
}