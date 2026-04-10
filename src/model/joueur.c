#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "libca.h"
#include "joueur.h"

Joueur creer_joueur(int id, int nb_meeple)
{
    Joueur joueur = { 0 };
    joueur.nom = NULL;
    joueur.id = id;
    joueur.pts = 0;
    joueur.nb_meeple_restant = nb_meeple;
    joueur.localisation_meeple = NULL;

    joueur.couleur.r = rand()%UCHAR_MAX;
    joueur.couleur.g = rand()%UCHAR_MAX;
    joueur.couleur.b = rand()%UCHAR_MAX;
    joueur.couleur.a = UCHAR_MAX;

    return joueur;
}

void detruire_joueur(Joueur joueur)
{
    detruire_liste_meeple(joueur.localisation_meeple);
}

ListeJoueurs creer_listejoueurs(int nb_joueurs, int nb_meeple)
{
    ListeJoueurs joueurs = { 0 };

    joueurs.tableau = ca_alloc(nb_joueurs, sizeof(Joueur));
    joueurs.nb_joueurs  = nb_joueurs;
    joueurs.nb_meeple_max = nb_meeple;
    joueurs.tour = 0;

    for (int i = 0; i < nb_joueurs; i++)
        joueurs.tableau[i] = creer_joueur(i, nb_meeple);

    return joueurs;
}

void detruire_listejoueurs(ListeJoueurs joueurs)
{
    for (int i = 0; i < joueurs.nb_joueurs; i++) {
        detruire_joueur(joueurs.tableau[i]);
    }
    free(joueurs.tableau);
}
