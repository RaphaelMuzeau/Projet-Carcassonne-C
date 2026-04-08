#include <stdlib.h>
#include <stdio.h>
#include "libca.h"
#include "joueur.h"

Joueur creer_joueur(int id, int nb_meeple)
{
    Joueur joueur = { 0 };
    joueur.nom = NULL;
    joueur.id = id;
    joueur.nb_meeple_restant = nb_meeple;
    joueur.localisation_meeple = NULL;

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
