#include <stdlib.h>
#include <stdio.h>
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
    if (!compatibilite_tuile(get(*grille, x, y), get(*grille, x - 1, y), D_OUEST)
     || !compatibilite_tuile(get(*grille, x, y), get(*grille, x + 1, y), D_EST)
     || !compatibilite_tuile(get(*grille, x, y), get(*grille, x, y - 1), D_NORD)
     || !compatibilite_tuile(get(*grille, x, y), get(*grille, x, y + 1), D_SUD))
        return false;

    set(grille, piece, x, y);
    return true;
}

/* recherche */

int zone_pts(enum Zone zone)
{
    if (zone == Z_ROUTE)  return 1;
    if (zone == Z_VILLE)  return 2;
    if (zone == Z_BLASON) return 4;

    fprintf(stderr, "carcassonne: recherche sur une zone invalide\n");
    exit(EXIT_FAILURE);
}

int recherche_suite(Vec2D grille, L_meeple *loc_meeple_all, enum Zone z, enum Direction d_arrive, enum Direction d_depart , Tuile t, int *nb_meeples, int x, int y, bool fin)
{
    int pts = 0;

    // traiter ce coté si c'est celui où a commencé la recherche
    // ou si on peut y acceder par le milieu
    if (d_depart == d_arrive || ((zone_tuile(t, d_depart) & z) && (t->milieu & z))) {

        // ajouter un meeple aux listes s'il est present sur ce coté
        if (t->id_meeple != -1 && t->position_meeple == d_depart) {
            nb_meeples[t->id_meeple] += 1;
            L_meeple maillon = creer_maillon_meeple(x, y, d_depart);
            ajouter_maillon_meeple(loc_meeple_all, maillon);
        }

        // effectuer la recherche récursive
        if      (d_depart == D_NORD)  pts = recherche(grille, nb_meeples, loc_meeple_all, x,     y - 1, z, D_SUD, fin);
        else if (d_depart == D_SUD)   pts = recherche(grille, nb_meeples, loc_meeple_all, x,     y + 1, z, D_NORD, fin);
        else if (d_depart == D_EST)   pts = recherche(grille, nb_meeples, loc_meeple_all, x + 1,     y, z, D_OUEST, fin);
        else if (d_depart == D_OUEST) pts = recherche(grille, nb_meeples, loc_meeple_all, x - 1,     y, z, D_EST, fin);
    }

    return pts;
}

int recherche(Vec2D grille, int *nb_meeples, L_meeple *loc_meeple_all, int x, int y, enum Zone z, enum Direction d_arrive, bool fin)
{
    Tuile t = get(grille, x, y);

    if (t == NULL) return fin ? 0 : -1;
    if (t->is_verified) return 0;
    t->is_verified = true;

    // traiter le cas du meeple au milieu en premier
    if (t->id_meeple != -1 && t->position_meeple == D_MILIEU) {
        nb_meeples[t->id_meeple] += 1;
        L_meeple maillon = creer_maillon_meeple(x, y, D_MILIEU);
        ajouter_maillon_meeple(loc_meeple_all, maillon);
    }

    int pts = zone_pts(zone_tuile(t, d_arrive));

    // effectuer la recherche sur tous les cote de la tuile
    for (enum Direction d_depart = 0; d_depart < D_MILIEU; ++d_depart) {
        int tmp = recherche_suite(grille, loc_meeple_all, z, d_arrive, d_depart, t, nb_meeples, x, y, fin);
        if(tmp == -1) return -1;
        pts += tmp;
    }

    return pts;
}


void recherche_is_verified(Vec2D grille, int x, int y)
{
    Tuile t = get(grille, x , y);

    if (t == NULL || !t->is_verified)
        return;

    t->is_verified = false;
    recherche_is_verified(grille, x+1, y);
    recherche_is_verified(grille, x-1, y);
    recherche_is_verified(grille, x, y+1);
    recherche_is_verified(grille, x, y-1);
}

/* recherche abbaye */

void recherche_abbaye(Vec2D grille, ListeJoueurs joueurs, int x, int y, bool fin)
{
    Tuile t = get(grille, x, y);
    int id_joueur = t->id_meeple;
    int pts = 1;

    if (id_joueur == -1)
        return;

    for(int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            t = get(grille, x + i, y + j);

            if (t != NULL)
                pts += 1;
            else if (!fin)
                return;
        }
    }

    joueurs.tableau[id_joueur].pts += pts;
}

void verification_abbaye(Vec2D grille, ListeJoueurs joueurs, int x, int y)
{
    for(int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (get(grille, x + i, y + j)->milieu == Z_ABBAYE)
                recherche_abbaye(grille, joueurs, x + i, y + j, false);
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
