#include <stdlib.h>
#include <stdbool.h>
#include "grille.h"
#include "tuile.h"
#include "meeple.h"
#include "joueur.h"

bool est_vide(Vec2D grille, int x, int y)
{
    return get(grille, x, y) == NULL;
}

bool placer_tuile(Vec2D *grille, int x, int y, Tuile piece)
{
    // Tuile occupé
    if (!est_vide(*grille, x, y))
        return false;

    // Tuile sans connexion
    if (est_vide(*grille, x - 1, y) && est_vide(*grille, x + 1, y) && est_vide(*grille, x, y - 1) && est_vide(*grille, x, y + 1))
        return false;

    // compatibilité
    if (compatibilite_tuile(get(*grille, x, y), get(*grille, x - 1, y), D_OUEST)
     && compatibilite_tuile(get(*grille, x, y), get(*grille, x + 1, y), D_EST)
     && compatibilite_tuile(get(*grille, x, y), get(*grille, x, y - 1), D_NORD)
     && compatibilite_tuile(get(*grille, x, y), get(*grille, x, y + 1), D_SUD)) {
        set(grille, piece, x, y);
        return true;
    }

    return false;
}

/* recherche */

int recherche(Vec2D grille, int *nb_meeples, L_meeple *loc_meeple_all, int x, int y, enum Zone z, enum Direction d)
{
    // TODO NOUVELLE IMPLEMENTATION AJOUT LOC_MEEPLE_ALL liste chainé de loc de meeple indexé par id joueur
    Tuile t = get(grille, x, y);
    int pts = 1;
    int tmp = 0;

    if (t == NULL)
        return -1;

    if (t->is_verified)
        return 0;
    t->is_verified = true;

    if (t->id_meeple != -1 && zone_tuile(t, d) == z)
        nb_meeples[t->id_meeple] += 1; // ajoute a la liste du nb de meeple indexé par joueur

    if (zone_tuile(t, D_MILIEU) == z) {

        if (t->id_meeple != -1 && t->position_meeple == D_MILIEU)
            nb_meeples[t->id_meeple] += 1;
        // FIN CHECK MILIEU

        if (zone_tuile(t, D_NORD) == z && d != D_NORD) {

            if (t->id_meeple != -1 && t->position_meeple == D_NORD)
                nb_meeples[t->id_meeple] += 1;

            Tuile t2 = get(grille, x, y - 1);
            if (!compatibilite_tuile(t, t2, D_NORD))
                return -1;
            tmp = recherche(grille, nb_meeples, loc_meeple_all, x, y - 1 , z, D_SUD);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK NORD

        if (zone_tuile(t, D_SUD) == z && d != D_SUD) {

            if (t->id_meeple != -1 && t->position_meeple == D_SUD)
                nb_meeples[t->id_meeple] += 1;

            Tuile t2 = get(grille, x, y + 1);
            if (!compatibilite_tuile(t, t2, D_SUD))
                return -1;
            tmp = recherche(grille, nb_meeples, loc_meeple_all, x, y + 1, z, D_NORD);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK SUD

        if (zone_tuile(t, D_EST) == z && d != D_EST) {

            if (t->id_meeple != -1 && t->position_meeple == D_EST)
                nb_meeples[t->id_meeple] += 1;

            Tuile t2 = get(grille, x + 1, y);
            if (!compatibilite_tuile(t, t2, D_EST))
                return -1;
            tmp = recherche(grille, nb_meeples,loc_meeple_all, x + 1, y, z, D_OUEST);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK EST

        if (zone_tuile(t, D_OUEST) == z && d != D_OUEST) {

            if (t->id_meeple != -1 && t->position_meeple == D_OUEST)
                nb_meeples[t->id_meeple] += 1;

            Tuile t2 = get(grille, x - 1, y);
            if (!compatibilite_tuile(t, t2, D_OUEST))
                return -1;
            tmp += recherche(grille, nb_meeples, loc_meeple_all, x - 1, y, z, D_EST);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK OUEST
    }
    return pts;
}

void recherche_is_verified(Vec2D grille, int x, int y)
{
    Tuile t = get(grille, x , y);
    if (!t->is_verified)
        return;

    t->is_verified = false;
    recherche_is_verified(grille, x+1, y);
    recherche_is_verified(grille, x-1, y);
    recherche_is_verified(grille, x, y+1);
    recherche_is_verified(grille, x, y-1);
}

/* placement de meeple */

bool placer_meeple(Vec2D grille, Joueur *joueur, int x, int y, enum Direction d)
{
    Tuile t = get(grille, x, y);

    if (t == NULL) return false;
    if (t->id_meeple != -1) return false;
    if (joueur->nb_meeple_restant <= 0) return false;

    L_meeple new = creer_maillon_meeple(x, y, d);
    ajouter_maillon_meeple(&joueur->localisation_meeples, new);
    t->id_meeple = joueur->id;
    t->position_meeple = d;

    joueur->nb_meeple_restant -= 1;

    return true;
}

void retirer_meeple(Vec2D grille, ListeJoueurs listejoueurs, int x, int y)
{
    Tuile t = get(grille, x, y);
    if (t == NULL) return;

    listejoueurs.tableau[t->id_meeple].nb_meeple_restant += 1;
    retirer_maillon_meeple(&listejoueurs.tableau[t->id_meeple].localisation_meeples, x, y);
    t->id_meeple = -1;
}
