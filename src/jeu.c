#include "libca.h"
#include "jeu.h"
#include "grille.h"
#include "meeple.h"

//TODO: TESTS
int maximal(int *nb_meeples, int nb_joueur)
{
    int maxi = nb_meeples[0];
    for (int i = 1; i < nb_joueur; i++) {
        if (maxi < nb_meeples[i])
            maxi = nb_meeples[i];
    }
    return maxi;
}

bool attribution_point(Jeu *jeu, L_meeple loc_meeple_all, int *nb_meeples, int pts)
{
    if (pts == -1)
        return false;

    int maxi = maximal(nb_meeples, jeu->listejoueurs.nb_joueur);

    for (int i = 0; i < jeu->listejoueurs.nb_joueur; i++)
        if (nb_meeples[i] == maxi)
            jeu->listejoueurs.tableau[i].pts += pts;

    L_meeple tmp = loc_meeple_all;
    while (tmp != NULL) {
        retirer_meeple(jeu->grille, jeu->listejoueurs, tmp->x,tmp->y);
        tmp = tmp->next;
    }

    return true;
}

bool tour(Jeu *jeu, Tuile tuile, int x, int y, int id_meeple, enum Direction position_meeple)
{
    if (!placer_tuile(&jeu->grille, x, y, tuile))
        return false;

    // preparation des arguments pour la recherche
    int *nb_meeples = ca_alloc(jeu->listejoueurs.nb_joueur, sizeof(int));
    L_meeple loc_meeple_all = NULL;

    enum Zone zone = Z_PRE;
    int pts = 0;

    // il faut traiter le cas du meeple en premier car il peut annuler le placement
    if (id_meeple != -1) {
        // on lance la recherche à partir de la zone où sera placé le meeple
        zone = zone_tuile(tuile, position_meeple);
        pts = recherche(jeu->grille, nb_meeples, &loc_meeple_all, x, y, zone, position_meeple);
        recherche_is_verified(jeu->grille, x, y);

        // si d'autres meeple ont été trouvés, annuler le placement et renvoyer faux
        for (int i = 0; i < jeu->listejoueurs.nb_joueur; i++) {
            if (nb_meeples[i] != 0) {
                set((&jeu->grille), NULL, x, y);
                free(nb_meeples);
                detruire_liste_meeple(loc_meeple_all);
                return false;
            }
        }

        // sinon, on peut retroactivement ajouter le meeple et attribuer les points
        placer_meeple(jeu->grille, &jeu->listejoueurs.tableau[id_meeple], x , y , position_meeple);
        nb_meeples[id_meeple] += 1;

        attribution_point(jeu, loc_meeple_all, nb_meeples, pts);
    }

    // lancer la recherche sur tous les autres cotés
    for (enum Direction d = 0; d < D_MILIEU; d++) {
        if (id_meeple != -1 && d == position_meeple)
            continue; // on a deja fait cette recherche

        // on réinitialise les listes utilisées par recherche()
        for (int i = 0; i < jeu->listejoueurs.nb_joueur; i++) {
            nb_meeples[i] = 0;
            detruire_liste_meeple(loc_meeple_all);
            loc_meeple_all = NULL;
        }

        pts = recherche(jeu->grille, nb_meeples, &loc_meeple_all, x, y, zone_tuile(tuile, d), d);
        recherche_is_verified(jeu->grille, x, y);
        attribution_point(jeu, loc_meeple_all, nb_meeples, pts);
    }

    free(nb_meeples);
    detruire_liste_meeple(loc_meeple_all);
    return true;
}
