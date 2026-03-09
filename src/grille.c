#include <stdio.h>
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

bool verification_tuile_zone(Tuile t, enum Zone z, enum Direction d)
{
    switch (d){
    case D_NORD:
        return t->nord == z;
    case D_SUD:
        return t->sud == z;
    case D_EST:
        return t->est == z;
    case D_OUEST:
        return t->ouest == z;
    case D_MILIEU:
        return t->milieu == z;
    default:
        fprintf(stderr, "Carcassonne vérification zone invalide\n");
        exit(EXIT_FAILURE);
    }
}

int recherche(Vec2D grille, int x, int y, char nb_meeples[], enum Zone z, enum Direction d)
{
    Tuile t = get(grille, x, y);
    int pts = 1;
    int tmp = 0;

    if (t == NULL)
        return -1;

    if (t->is_verified)
        return 0;
    t->is_verified = true;

    if (t->meeple != -1 && verification_tuile_zone(t, z, d))
            nb_meeples[(int)t->meeple] += 1;

    if (verification_tuile_zone(t, z, D_MILIEU)) {

        if (t->meeple != -1 && t->position_meeple == D_MILIEU)
            nb_meeples[(int)t->meeple] += 1;
        // FIN CHECK MILIEU

        if (verification_tuile_zone(t, z, D_NORD) && d != D_NORD) {

            if (t->meeple != -1 && t->position_meeple == D_NORD)
                nb_meeples[(int)t->meeple] += 1;

            Tuile t2 = get(grille, x, y - 1);
            if (!compatibilite_tuile(t, t2, D_NORD))
                return -1;
            tmp = recherche(grille, x, y - 1, nb_meeples, z, D_SUD);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK NORD

        if (verification_tuile_zone(t, z, D_SUD) && d != D_SUD) {

            if (t->meeple != -1 && t->position_meeple == D_SUD)
                nb_meeples[(int)t->meeple] += 1;

            Tuile t2 = get(grille, x, y + 1);
            if (!compatibilite_tuile(t, t2, D_SUD))
                return -1;
            tmp = recherche(grille, x, y + 1, nb_meeples, z, D_NORD);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK SUD

        if (verification_tuile_zone(t, z, D_EST) && d != D_EST) {

            if (t->meeple != -1 && t->position_meeple == D_EST)
                nb_meeples[(int)t->meeple] += 1;

            Tuile t2 = get(grille, x + 1, y);
            if (!compatibilite_tuile(t, t2, D_EST))
                return -1;
            tmp = recherche(grille, x + 1, y, nb_meeples, z, D_OUEST);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK EST

        if (verification_tuile_zone(t, z, D_OUEST) && d != D_OUEST) {

            if (t->meeple != -1 && t->position_meeple == D_OUEST)
                nb_meeples[(int)t->meeple] += 1;

            Tuile t2 = get(grille, x - 1, y);
            if (!compatibilite_tuile(t, t2, D_OUEST))
                return -1;
            tmp += recherche(grille, x - 1, y, nb_meeples, z, D_EST);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK OUEST
    }
    return pts;
}
