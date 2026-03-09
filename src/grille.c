#include <stdlib.h>
#include <stdbool.h>
#include "tuile.h"
#include "vec.h"

bool est_vide(Vec2D grille, int x, int y)
{
    return get(grille, x, y) == NULL;
}

bool placer_tuile(Vec2D grille, int x, int y, Tuile piece)
{
    // Tuile occupé
    if (!est_vide(grille, x, y))
        return false;

    // Tuile sans connexion
    if (est_vide(grille, x - 1, y) && est_vide(grille, x + 1, y) && est_vide(grille, x, y - 1) && est_vide(grille, x, y + 1))
        return false;

    // compatibilité
    if (compatibilite_tuile(get(grille, x, y), get(grille, x - 1, y), D_OUEST)
    && compatibilite_tuile(get(grille, x, y), get(grille, x + 1, y), D_EST)
    && compatibilite_tuile(get(grille, x, y), get(grille, x, y - 1), D_NORD)
    && compatibilite_tuile(get(grille, x, y), get(grille, x, y + 1), D_SUD)) {
        set(&grille, piece, x, y);
        return true;
    }
    return false;
}
