#include "libca.h"
#include "jeu.h"
#include "grille.h"
#include "meeple.h"

//TODO: TESTS
int maximal(int* nb_meeples, int nb_joueur)
{
    int maxi = nb_meeples[0];
    for (int i = 1; i < nb_joueur; i++) {
        if (maxi < nb_meeples[i])
            maxi = nb_meeples[i];
    }
    return maxi;
}

bool attribution_point(L_meeple *loc_meeple_all, int* nb_meeples, Jeu *jeu, int pts)
{
    if (pts == -1)
        return false;

    int maxi = maximal(nb_meeples,jeu->listejoueurs.nb_joueur);

    for (int i = 0; i < jeu->listejoueurs.nb_joueur ; i++){
        if (nb_meeples[i] == maxi)
            jeu->listejoueurs.tableau[i].pts += pts;
    }
    for (int i = 0; i < jeu->listejoueurs.nb_joueur; i++){
        retrait_meeple_liste((&jeu->listejoueurs.tableau[i]),jeu->grille, loc_meeple_all[i]);
    }
    return true;
}

void round_encapsuler(Jeu *jeu, int id_meeple, int x, int y, Tuile tuile, enum Direction d)
{
    int pts;
    int *nb_meeples;
    enum Zone zone_recherche = zone_tuile(tuile, d);

    nb_meeples = ca_alloc(jeu->listejoueurs.nb_joueur,sizeof(int));
    L_meeple *loc_meeple_all = ca_alloc(jeu->listejoueurs.nb_joueur, sizeof(L_meeple));

    if(id_meeple != -1){
        ajouter_meeple((&jeu->listejoueurs.tableau[id_meeple]),jeu->grille, x, y, d);
    }


    placer_tuile(&jeu->grille, x, y, tuile);
    pts = recherche(jeu->grille, nb_meeples, loc_meeple_all, x, y, zone_recherche, d);
    attribution_point(loc_meeple_all, nb_meeples, jeu, pts);

    free(nb_meeples);
    free(loc_meeple_all);
}
