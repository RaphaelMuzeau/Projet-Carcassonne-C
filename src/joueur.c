#include <stdlib.h>
#include "joueur.h"

Joueur creer_joueur(int id, int nb_meeple)
{
    Joueur joueur = { 0 };
    joueur.id = id;
    joueur.nb_meeple_restant = nb_meeple;
    joueur.localisation_meeples = NULL;

    return joueur;
}

void detruire_joueur(Joueur joueur)
{
    detruire_liste_meeple(joueur.localisation_meeples);
}
