#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "tuile.h"
#include "meeple.h"
#include "grille.h"

void ajout_meeple_tuile(Joueur *joueur, Grille grille, int x, int y, enum Direction d)
{
    assert(grille.tableau[x][y]->meeple == '\0' && "Impossible de placer un meeple sur un meeple déjà existant");
    if (joueur->nb_meeple_restant != 0) {
        L_meeple new;
        new = ca_alloc(1, sizeof(struct _maillon);
        new->d = d;
        new->x = x;
        new->y = y;
        new->next = NULL;
        //ajout_meeple_chaine(joueur,new);
        grille.tableau[x][y]->meeple = joueur->color;
        joueur->nb_meeple_restant -= 1;
    }
}

// Ajout en fin de liste
void ajout_meeple_chaine(Joueur *joueur, L_meeple new)
{
    L_meeple tmp;
    tmp = joueur->localisation_meeples;
    if (tmp == NULL)
        joueur->localisation_meeples = new;
    else {
        while( tmp->next != NULL)
            tmp=tmp->next;
        tmp->next = new;
    }
}

bool destruction_meeple(Joueur *joueur, int x, int y)
{
    L_meeple tmp;
    L_meeple tmp2;
    tmp = joueur->localisation_meeples;
    if (tmp == NULL)
        return false;
    else {
        tmp2 = tmp->next;
        while(tmp->next != NULL && tmp2->next != NULL) {
            if ((tmp2->x == x) && (tmp2->y == y)) {
                tmp->next = tmp2->next;
                free(tmp2);
                return true;
            }

        }
    return false;
    }
}
