#include "libca.h"
#include "tuile.h"

Tuile creer_tuile(void)
{
    Tuile s;
    s = ca_alloc(1, sizeof(struct _Tuile));
    s->milieu = Z_PRE;
    s->nord   = Z_PRE;
    s->sud    = Z_PRE;
    s->est    = Z_PRE;
    s->ouest  = Z_PRE;
    s->id_meeple = -1;

    return s;
}

void pivot_90(Tuile piece)
{
    enum Zone tmp;
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
        return (depart->nord & arrivee->sud);
    case D_SUD:
        return (depart->sud & arrivee->nord);
    case D_OUEST:
        return (depart->ouest & arrivee->est);
    case D_EST:
        return (depart->est & arrivee->ouest);
    default:
        return false;
    }
}
