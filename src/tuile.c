#include <stdlib.h>
#include "tuile.h"

Tuile init_tuile(void)
{
    Tuile s;
    s = malloc(sizeof(struct _Tuile));
    return s;
}

void pivot_90(Tuile piece)
{
    char tmp;
    tmp = piece->haut;
    piece->haut = piece->droite;
    piece->droite = piece->bas;
    piece->bas = piece->gauche;
    piece->gauche = tmp;
}