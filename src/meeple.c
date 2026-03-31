#include <stdlib.h>
#include "meeple.h"
#include "libca.h"

L_meeple creer_maillon_meeple(int x, int y, enum Direction d)
{
    L_meeple liste = ca_alloc(1, sizeof(struct _maillon));
    liste->next = NULL;
    liste->x = x;
    liste->y = y;
    liste->d = d;

    return liste;
}

void detruire_liste_meeple(L_meeple liste)
{
    L_meeple tmp, tmp2;
    if (liste == NULL)
        return;

    tmp = liste;
    tmp2 = tmp->next;

    while(tmp2 != NULL){
        free(tmp);
        tmp = tmp2;
        tmp2 = tmp2->next;
    }
    free(tmp);

    return;
}

// ajout en tete
void ajouter_maillon_meeple(L_meeple *liste, L_meeple new)
{
    new->next = *liste;
    *liste = new;
}

void retirer_maillon_meeple(L_meeple *liste, int x, int y)
{
    if (*liste == NULL)
        return;

    L_meeple tmp = *liste;
    if (tmp->x == x && tmp->y == y) {
        *liste = tmp->next;
        free(tmp);
        return;
    }

    L_meeple tmp2 = tmp->next;
    while (tmp2 != NULL) {
        if (tmp2->x == x && tmp2->y == y) {
            tmp->next = tmp2->next;
            free(tmp2);
            return;
        }
        tmp = tmp->next;
        tmp2 = tmp2->next;
    }
}
