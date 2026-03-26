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

    return s;
}

void pivot_90(Tuile piece)
{
    enum Zone tmp;
    tmp = piece->nord;
    piece->nord = piece->ouest;
    piece->ouest = piece->sud;
    piece->sud = piece->est;
    piece->est = tmp;
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

/* TODO:
enum Zone generer_milieu()
{
    int aleatoire = rand()%100;


}

Tuile generer_tuile()
{
    Tuile t = creer_tuile();

    t->milieu = generer_milieu();


}
*/
