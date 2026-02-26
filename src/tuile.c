#include <stdlib.h>
#include "tuile.h"

Tuile init_tuile(void)
{
    Tuile s;
    s = calloc(1, sizeof(struct _Tuile));
    return s;
}

void pivot_90(Tuile piece)
{
    char tmp;
    tmp = piece->nord;
    piece->nord = piece->est;
    piece->est = piece->sud;
    piece->sud = piece->ouest;
    piece->ouest = tmp;
}

bool compatibilite_tuile(Tuile depart, Tuile arrivee, enum Direction d)
{
    if (arrivee == NULL || depart == NULL)
        return true;

    switch (d) {
    case D_NORD:
        return (depart->nord == arrivee->sud);
    case D_SUD:
        return (depart->sud == arrivee->nord);
    case D_OUEST:
        return (depart->ouest == arrivee->est);
    case D_EST:
        return (depart->est == arrivee->ouest);
    default:
        return false;
    }
}
