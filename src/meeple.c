#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "tuile.h"
#include "meeple.h"
#include "libca.h"
#include "vec.h"

// Ajout en fin de liste
void ajout_meeple_chaine(Joueur *joueur, L_meeple new)
{
    L_meeple tmp;
    tmp = joueur->localisation_meeples;
    if (tmp == NULL)
        joueur->localisation_meeples = new;
    else {
        while(tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new;
    }
}

void ajout_meeple_tuile(Joueur *joueur, Vec2D grille, int x, int y, enum Direction d)
{ // TODO: si le placement est impossible, renvoyer faux.
    Tuile t = get(grille, x, y);
    assert(t->id_meeple == -1 && "Impossible de placer un meeple sur un meeple déjà existant");
    if (joueur->nb_meeple_restant != 0) {
        L_meeple new = ca_alloc(1, sizeof(struct _maillon));
        new->d = d;
        new->x = x;
        new->y = y;
        new->next = NULL;
        ajout_meeple_chaine(joueur,new);

        t->id_meeple = joueur->id;
        // TODO : t->position_meeple = d;

        joueur->nb_meeple_restant -= 1;
    } else
        free(t);
}

void detruire_meeple(Joueur *joueur, int x, int y)
{
    if (joueur->localisation_meeples == NULL)
        return;

    L_meeple tmp = joueur->localisation_meeples;
    if (tmp->x == x && tmp->y == y) {
        joueur->localisation_meeples = tmp->next;
        free(tmp);
        return;
    }

    L_meeple tmp2 = tmp->next;
    while ( tmp2 != NULL){
        if (tmp2->x == x && tmp2->y == y) {
            tmp->next = tmp2->next;
            free(tmp2);
            return;
        }
        tmp = tmp->next;
        tmp2 = tmp2->next;
    }
}
