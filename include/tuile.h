#ifndef tuile_h
#define tuile_h

#include <stdbool.h>

enum Zone {
    Z_ROUTE,
    Z_VILLE,
    Z_BLASON,
    Z_PRE,
    Z_VILLAGE,
    Z_ABBAYE,
};

enum Direction {
    D_SUD,
    D_NORD,
    D_EST,
    D_OUEST,
};

struct _Tuile {
    enum Zone milieu, nord, sud, est, ouest;
    char meeple;
};
typedef struct _Tuile *Tuile;

Tuile init_tuile(void);
/* pivot à droite */
void pivot_90(Tuile piece);
bool compatibilite_tuile(Tuile depart, Tuile arrivee, enum Direction d);

#endif