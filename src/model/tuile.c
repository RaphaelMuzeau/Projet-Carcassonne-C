#include <stdlib.h>
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

enum Zone zone_tuile(Tuile t, enum Direction d)
{
    switch (d){
    case D_NORD:
        return t->nord;
    case D_SUD:
        return t->sud;
    case D_EST:
        return t->est;
    case D_OUEST:
        return t->ouest;
    case D_MILIEU:
        return t->milieu;
    default:
        ca_error("direction invalide");
    }
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

enum Zone generer_milieu(void)
{
    int aleatoire = rand()%100;

    // Les probabilités ont été extraites du csv
    if      (aleatoire <= 43) return Z_ROUTE;
    else if (aleatoire <= 66) return Z_VILLE;
    else if (aleatoire <= 79) return Z_PRE;
    else if (aleatoire <= 90) return Z_VILLAGE;
    else                      return Z_ABBAYE;
}

enum Zone generer_cote(int aleatoire)
{
    if      (aleatoire <= 40) return Z_PRE;
    else if (aleatoire <= 67) return Z_VILLE;
    else                      return Z_ROUTE;
}

Tuile generer_tuile(void)
{
    Tuile t = creer_tuile();

    t->milieu = generer_milieu();

    if (t->milieu == Z_ABBAYE) {
        t->est = generer_cote(rand()%100);
        t->sud = generer_cote(rand()%100);
        t->nord = generer_cote(rand()%100);
        t->ouest = generer_cote(rand()%100);
    }

    if (t->milieu == Z_VILLAGE) {
        t->est = generer_cote(rand()%100);
        t->sud = generer_cote(rand())%100;
        t->nord = generer_cote(rand()%100);
        t->ouest = generer_cote(rand()%100);
    }

    if (t->milieu == Z_PRE) {
        t->est = generer_cote(rand()%67);
        t->sud = generer_cote(rand()%67);
        t->nord = generer_cote(rand()%67);
        t->ouest = generer_cote(rand()%67);
    }
    /*
     * Si une route ou une ville apparaît au milieu, c'est qu'elle
     * fait le lien entre deux zones, donc au moins deux autres
     * villes ou routes doivent être présentes sur la tuile
     */
    if (t->milieu == Z_ROUTE || t->milieu == Z_VILLE) {
        int cmpt = 0;
        do {
            cmpt = 0;
            t->est = generer_cote(rand()%100);
            if (t->est == t->milieu) cmpt++;

            t->sud = generer_cote(rand()%100);
            if (t->sud== t->milieu) cmpt++;

            t->nord = generer_cote(rand()%100);
            if (t->nord == t->milieu) cmpt++;

            t->ouest = generer_cote(rand()%100);
            if (t->ouest == t->milieu) cmpt++;
        } while (cmpt < 2);
    }

    return t;
}

