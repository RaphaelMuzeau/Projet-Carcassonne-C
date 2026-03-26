#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tuile.h"
#include "vec.h"
#include "jeu.h"
#include "libca.h"
#include "meeple.h"

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
        fprintf(stderr, "Carcassonne zone invalide\n");
        exit(EXIT_FAILURE);
    }

}
int recherche(Vec2D grille, int* nb_meeples, L_meeple loc_meeple_all, int x, int y, enum Zone z, enum Direction d)
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

    if (t->id_meeple != -1 && verification_tuile_zone(t, z, d))
            nb_meeples[(int)t->id_meeple] += 1; // ajoute a la liste du nb de meeple indexé par joueur


    if (verification_tuile_zone(t, z, D_MILIEU)) {

        if (t->id_meeple != -1 && t->position_meeple == D_MILIEU)
            nb_meeples[(int)t->id_meeple] += 1;
        // FIN CHECK MILIEU

        if (verification_tuile_zone(t, z, D_NORD) && d != D_NORD) {

            if (t->id_meeple != -1 && t->position_meeple == D_NORD)
                nb_meeples[(int)t->id_meeple] += 1;

            Tuile t2 = get(grille, x, y - 1);
            if (!compatibilite_tuile(t, t2, D_NORD))
                return -1;
            tmp = recherche(grille, nb_meeples, loc_meeple_all, x, y - 1 , z, D_SUD);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK NORD

        if (verification_tuile_zone(t, z, D_SUD) && d != D_SUD) {

            if (t->id_meeple != -1 && t->position_meeple == D_SUD)
                nb_meeples[(int)t->id_meeple] += 1;

            Tuile t2 = get(grille, x, y + 1);
            if (!compatibilite_tuile(t, t2, D_SUD))
                return -1;
            tmp = recherche(grille, nb_meeples, loc_meeple_all, x, y + 1, z, D_NORD);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK SUD

        if (verification_tuile_zone(t, z, D_EST) && d != D_EST) {

            if (t->id_meeple != -1 && t->position_meeple == D_EST)
                nb_meeples[(int)t->id_meeple] += 1;

            Tuile t2 = get(grille, x + 1, y);
            if (!compatibilite_tuile(t, t2, D_EST))
                return -1;
            tmp = recherche(grille, nb_meeples,loc_meeple_all, x + 1, y, z, D_OUEST);
            if (tmp == -1)
                return -1;
            pts += tmp;
        } // FIN CHECK EST

        if (verification_tuile_zone(t, z, D_OUEST) && d != D_OUEST) {

            if (t->id_meeple != -1 && t->position_meeple == D_OUEST)
                nb_meeples[(int)t->id_meeple] += 1;

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

//TODO: TESTS / Attribution des pts / encapsulation de recherche / faire fonctionner les joueurs
int maximal(int* nb_meeples, int nb_joueur)
{
    int maxi = nb_meeples[0];
    for (int i = 0; i < nb_joueur; i++){
        if (maxi < nb_meeples[i])
            maxi = nb_meeples[i];
    }
    return maxi;
}
bool attribution_point(L_meeple loc_meeple_all, int* nb_meeples, Jeu *jeu, int pts)
{
    if (pts != -1)
        return false;

    int maxi = maximal(nb_meeples,jeu->listejoueurs.nb_joueur);

    for (int i = 0; i < jeu->listejoueurs.nb_joueur ; i++){
        if (nb_meeples[i] == maxi)
            jeu->listejoueurs.tableau[i].pts += pts;
    }
    for (int i = 0; i < jeu->listejoueurs.nb_joueur; i++){
        retrait_meeple_liste((&jeu->listejoueurs.tableau[i]),jeu->grille, loc_meeple_all
    );
    }
    return true;
}

void round_encapsuler(Jeu *jeu, int id_joueur, int x, int y, Tuile tuile, enum Direction d)
{
    int pts;
    int* nb_meeples;
    enum Zone zone_recherche;
    zone_recherche = zone_tuile(tuile,d);

    nb_meeples = ca_alloc(jeu->listejoueurs.nb_joueur,sizeof(int));
    L_meeple loc_meeple_all = ca_alloc(jeu->listejoueurs.nb_joueur,sizeof(L_meeple));

    if(id_joueur != -1){
        ajouter_meeple((&jeu->listejoueurs.tableau[id_joueur]),jeu->grille, x, y, d);
    }


    placer_tuile(jeu->grille, x, y, tuile);
    pts = recherche(jeu->grille, nb_meeples, loc_meeple_all, x, y, zone_recherche, d);
    attribution_point(loc_meeple_all, nb_meeples, jeu, pts);

    free(nb_meeples);
    free(loc_meeple_all);
}
