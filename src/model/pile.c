#include <stdbool.h>
#include "libca.h"
#include "pile.h"
#include "tuile.h"

Pile creer_pile(int max_element)
{
    Pile p;
    p.nb_element_max = max_element;
    p.nb_element = 0;
    p.tab = ca_alloc(max_element, sizeof(Tuile));

    return p;
}

bool pile_vide(Pile p)
{
    if (p.nb_element == 0)
        return true;
    return false;
}

bool pile_pleine(Pile p)
{
    if (p.nb_element == p.nb_element_max)
        return true;
    return false;
}

Tuile recup_tuile(Pile *p)
{
    if (pile_vide(*p))
        return NULL;

    Tuile tmp;
    int id;
    id = rand()%p->nb_element;
    tmp = p->tab[id];

    p->tab[id] = p->tab[p->nb_element-1];
    p->nb_element--;

    return tmp;
}

bool inserer_tuile(Pile *p, Tuile t)
{

    if (pile_pleine(*p))
        return false;

    p->tab[p->nb_element] = t;
    p->nb_element++;
    return true;
}

void detruire_pile(Pile *p)
{
    for (int i = 0; i < p->nb_element; i++) {
        free(p->tab[i]);
    }
    free(p->tab);
}
