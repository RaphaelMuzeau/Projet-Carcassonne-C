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

int recherche(Vec2D grille, int *nb_meeples, L_meeple *loc_meeple_all, int x, int y, enum Zone z, enum Direction d, bool fin)
{
    // TODO NOUVELLE IMPLEMENTATION AJOUT LOC_MEEPLE_ALL liste chainé de loc de meeple indexé par id joueur
    Tuile t = get(grille, x, y);
    int pts = 0;
    int tmp = 0;
    if (!fin) {
        if (t == NULL) // Lors de la recherche classique si on tombe sur une tuile vide on renvoie -1 ce qui équivaut a une zone non fini
            return -1;
    }
    else {
        if( t == NULL) // Lors de la recherche de fin si on tombe sur une tuile vide on donne renvoie tout les pts
            return pts;
    }
    if (t->is_verified) // a chaque etape de la recherche chaque case parcouru est marqué is_verified == 1 dans pour ne pas ensuite
        return 0;       // retomber sur la case si is_verified est a 1 on renvoie 0 pour ne pas effectuer la recherche sur cette case.
    t->is_verified = true;

    if (zone_tuile(t,d) == Z_VILLE) // Si ma zone z recherché est une blason il faut vérifier
        pts += 2;                   // a la main si une partie de ma case est une ville
                                    // car blason et ville sont compatible donc compatibilité tuile ne crash psa

    else if (zone_tuile(t,d) == Z_BLASON) // Si ma zone z recherché est une ville il faut vérifier
        pts += 4;                         // a la main si une partie de ma case est un blason
                                          // car blason et ville sont compatible donc compatibilité tuile ne crash pas
    else pts += 1;
    if (t->id_meeple != -1 && zone_tuile(t, d) == z)
        nb_meeples[t->id_meeple] += 1; // ajoute a la liste du nb de meeple indexé par joueur

    if (zone_tuile(t, D_MILIEU) == z) { // si le milieu de notre tuile correspond a a notre zone cherché on continue la recherche
                                        // sur les côtés de la tuile ou la zone est la même que celle cherchée
        if (t->id_meeple != -1 && t->position_meeple == D_MILIEU) // on compte chaque meeple de chaque joueur avec une liste
            nb_meeples[t->id_meeple] += 1;                        // la liste est indexé par l'id du joueur (id_meeple)
        // FIN CHECK MILIEU

        if (zone_tuile(t, D_NORD) == z && d != D_NORD) { // on continue la recherche sur les 4 cotés ici nord

            if (t->id_meeple != -1 && t->position_meeple == D_NORD)
                nb_meeples[t->id_meeple] += 1;

            Tuile t2 = get(grille, x, y - 1);
            if (!fin) {                                 // si notre booléen fin est a 0, cela veut dire que nous effectuons
                if (!compatibilite_tuile(t, t2, D_NORD))// une recherche classique et donc que si notre compatibilité tuile
                    return -1;                          // ne fonctionne pas on ne sera pas apte a accorder les pts donc return -1
            }
            tmp = recherche(grille, nb_meeples, loc_meeple_all, x, y - 1 , z, D_SUD, fin); // on effectue la recherche récursive
            if (!fin) {  // si notre booléen fin est a 0 et que notre recherche nous renvoie -1     // sur la case au nord
                if (tmp == -1)
                    return -1; // cela veut dire que notre ville / route est incomplète
            }
            pts += tmp; // et dans le cas ou c'est correct on ajoute nos pts a pts et on renvoie les pts a la fin du programme
        } // FIN CHECK NORD

        if (zone_tuile(t, D_SUD) == z && d != D_SUD) {

            if (t->id_meeple != -1 && t->position_meeple == D_SUD)
                nb_meeples[t->id_meeple] += 1;

            Tuile t2 = get(grille, x, y + 1);
            if (!fin) {                                                 // idem au sud
                if (!compatibilite_tuile(t, t2, D_NORD))
                    return -1;
            }
            tmp = recherche(grille, nb_meeples, loc_meeple_all, x, y + 1, z, D_NORD, fin);
            if (!fin) {
                if (tmp == -1)
                    return -1;
            }
            pts += tmp;
        } // FIN CHECK SUD

        if (zone_tuile(t, D_EST) == z && d != D_EST) {

            if (t->id_meeple != -1 && t->position_meeple == D_EST)
                nb_meeples[t->id_meeple] += 1;

            Tuile t2 = get(grille, x + 1, y);
            if (!fin) {                                                 // idem a l'est
                if (!compatibilite_tuile(t, t2, D_NORD))
                    return -1;
            }
            tmp = recherche(grille, nb_meeples,loc_meeple_all, x + 1, y, z, D_OUEST, fin);
            if (!fin) {
                if (tmp == -1)
                    return -1;
            }
            pts += tmp;
        } // FIN CHECK EST

        if (zone_tuile(t, D_OUEST) == z && d != D_OUEST) {

            if (t->id_meeple != -1 && t->position_meeple == D_OUEST)
                nb_meeples[t->id_meeple] += 1;

            Tuile t2 = get(grille, x - 1, y);
            if (!fin) {                                                 // idem a l'ouest
                if (!compatibilite_tuile(t, t2, D_NORD))
                    return -1;
            }
            tmp += recherche(grille, nb_meeples, loc_meeple_all, x - 1, y, z, D_EST, fin);
            if (!fin) {
                if (tmp == -1)
                    return -1;
            }
            pts += tmp;
        } // FIN CHECK OUEST
    }
    return pts; // a la fin de notre recherche on renvoie les pts totaux (-1 si notre ville ou route est imcomplète)
                // dans le cas de la recherche classique / dans la recherche de fin on donnera tout les points qu'on a trouvé
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
