#include <stdio.h>
#include <stdlib.h>
#include "../include/pile.h"

pile createPile(max_element)
{
    pile pTuiles;
    pTuiles.nb_element_max = max_element;
    pTuiles.nb_element = 0;
    pTuiles.tab = malloc(sizeof(struct tuile)*max_element);

    return pTuiles;
}

