#include "libca.h"
#include "jeu.h"
#include "tuile.h"
#include "grille.h"
#include "meeple.h"

Jeu creer_jeu(int nb_joueurs, int nb_meeple, int nb_tuiles)
{
    Jeu jeu;

    if (nb_joueurs < 1) ca_error("nombre de joueurs inferieur à 1");
    if (nb_meeple  < 1) ca_error("nombre de meeple inferieur à 1");
    if (nb_tuiles  < 1) ca_error("nombre de tuiles inferieur à 1");

    jeu.joueurs = creer_listejoueurs(nb_joueurs, nb_meeple);
    jeu.pile = creer_pile(nb_tuiles, true);
    jeu.grille = creer_vec2D();

#ifndef RUN_UNIT_TESTS
    // placer la tuile racine
    set(&jeu.grille, recup_tuile(&jeu.pile), 0, 0);
#endif

    return jeu;
}

void detruire_jeu(Jeu jeu)
{
    detruire_listejoueurs(jeu.joueurs);
    detruire_pile(&jeu.pile);
    detruire_vec2D(jeu.grille);
}

int maximal(int *nb_meeples, int nb_joueur)
{
    int maxi = nb_meeples[0];
    for (int i = 1; i < nb_joueur; i++) {
        if (maxi < nb_meeples[i])
            maxi = nb_meeples[i];
    }
    return maxi;
}

void attribution_points(Jeu *jeu, L_meeple loc_meeple, int *nb_meeples, int pts, bool fin)
{
    if (pts == -1)
        return;

    int maxi = maximal(nb_meeples, jeu->joueurs.nb_joueurs);
    if(maxi == 0 ) return;

    for (int i = 0; i < jeu->joueurs.nb_joueurs; i++)
        if (nb_meeples[i] == maxi)
            jeu->joueurs.tableau[i].pts += pts;

    L_meeple tmp = loc_meeple;
    while (tmp != NULL) {
        // si le jeu est encore en cours, il faut retirer proprement le meeple
        // avec retirer_meeple(), sinon, on le retire simplement de la grille pour laisser fin()
        // iterer sur la localisation_meeple du joueur.
        if (!fin) retirer_meeple(jeu->grille, jeu->joueurs, tmp->x,tmp->y);
        else      get(jeu->grille, tmp->x, tmp->y)->id_meeple = -1;

        tmp = tmp->next;
    }
    return;
}

bool tour(Jeu *jeu, Tuile tuile, int x, int y, bool place_meeple, enum Direction position_meeple)
{
    if (!placer_tuile(&jeu->grille, x, y, tuile))
        return false;

    // preparation des arguments pour la recherche
    int *nb_meeple = ca_alloc(jeu->joueurs.nb_joueurs, sizeof(int));
    L_meeple loc_meeple = NULL;

    // il faut traiter le cas du meeple en premier car il peut annuler le placement
    if (place_meeple) {
        // on lance la recherche à partir de la zone où sera placé le meeple
        int pts = amorce_recherche(jeu->grille, nb_meeple, &loc_meeple, x, y, position_meeple, false);
        recherche_est_verifie(jeu->grille, x, y);

        // si d'autres meeple ont été trouvés, annuler le placement et renvoyer faux
        for (int i = 0; i < jeu->joueurs.nb_joueurs; i++) {
            if (nb_meeple[i] != 0) {
                set(&jeu->grille, NULL, x, y);
                free(nb_meeple);
                detruire_liste_meeple(loc_meeple);
                return false;
            }
        }

        // de meme si le placement du meeple est invalide
        if (!placer_meeple(jeu->grille, &jeu->joueurs.tableau[jeu->joueurs.tour], x , y , position_meeple)) {
            set((&jeu->grille), NULL, x, y);
            free(nb_meeple);
            detruire_liste_meeple(loc_meeple);
            return false;
        }

        // sinon, on peut retroactivement ajouter le meeple et attribuer les points
        if (pts != 0) {
            nb_meeple[jeu->joueurs.tour] += 1;
            ajouter_maillon_meeple(&loc_meeple, creer_maillon_meeple(x,y));
            attribution_points(jeu, loc_meeple, nb_meeple, pts, false);
        }
    }

    // lancer la recherche sur tous les autres cotés
    for (enum Direction d = 0; d < D_MILIEU; d++) {
        if (place_meeple && d == position_meeple)
            continue; // on a deja fait cette recherche

        // on réinitialise les listes utilisées par recherche()
        for (int i = 0; i < jeu->joueurs.nb_joueurs; i++)
            nb_meeple[i] = 0;
        detruire_liste_meeple(loc_meeple);
        loc_meeple = NULL;

        int pts = amorce_recherche(jeu->grille, nb_meeple, &loc_meeple, x, y, d, false);
        recherche_est_verifie(jeu->grille, x, y);
        attribution_points(jeu, loc_meeple, nb_meeple, pts, false);
    }

    // verifier si il y a des abbaye sur ou au abords de la tuile
    // et attribuer les points correspondants si elles sont completes
    verification_abbaye(jeu->grille, jeu->joueurs, x, y);

    free(nb_meeple);
    detruire_liste_meeple(loc_meeple);

    jeu->joueurs.tour = (jeu->joueurs.tour + 1) % jeu->joueurs.nb_joueurs;
    return true;
}

int fin(Jeu *jeu)
{
    // preparation des arguments pour la recherche
    int *nb_meeple = ca_alloc(jeu->joueurs.nb_joueurs, sizeof(int));
    L_meeple loc_meeple = NULL;

    // on attribue les points pour chaque meeple encore placé sur la grille
    for (int i = 0; i < jeu->joueurs.nb_joueurs; i++) {
        for (L_meeple meeple = jeu->joueurs.tableau[i].localisation_meeple; meeple != NULL; meeple = meeple->next) {
            Tuile t = get(jeu->grille, meeple->x, meeple->y);

            if (t->position_meeple == D_MILIEU && t->milieu == Z_ABBAYE)
                recherche_abbaye(jeu->grille, jeu->joueurs, meeple->x, meeple->y, true);
            else {
                // on réinitialise les listes utilisées par recherche()
                for (int j = 0; j < jeu->joueurs.nb_joueurs; j++)
                    nb_meeple[j] = 0;
                detruire_liste_meeple(loc_meeple);
                loc_meeple = NULL;

                int pts = recherche(jeu->grille, nb_meeple, &loc_meeple, meeple->x, meeple->y,
                                    zone_tuile(t, t->position_meeple), t->position_meeple, true);
                recherche_est_verifie(jeu->grille, meeple->x, meeple->y);
                attribution_points(jeu, loc_meeple, nb_meeple, pts, true);
            }
        }
    }

    int pts_max = jeu->joueurs.tableau[0].pts;
    int id_max = 0;
    for (int i = 1; i < jeu->joueurs.nb_joueurs; i++) {
        if (pts_max < jeu->joueurs.tableau[i].pts) {
            pts_max = jeu->joueurs.tableau[i].pts;
            id_max = i;
        }
    }

    free(nb_meeple);
    detruire_liste_meeple(loc_meeple);

    return id_max;
}
