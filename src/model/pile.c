#include <stdbool.h>
#include "libca.h"
#include "pile.h"
#include "tuile.h"

Pile creer_pile(int max_element, bool gen)
{
    Pile p;
    p._nb_element_max = max_element;
    p._gen_aleatoire = gen;

    if (p._gen_aleatoire) {
        p.nb_element = max_element;
        p._tab = NULL;
    } else {
        p.nb_element = 0;
        p._tab = ca_alloc(max_element, sizeof(Tuile));
    }

    return p;
}

bool pile_vide(Pile p)
{
    return p.nb_element == 0;
}

bool pile_pleine(Pile p)
{
    return p.nb_element == p._nb_element_max;
}

Tuile recup_tuile(Pile *p)
{
    if (pile_vide(*p))
        return NULL;

    if (p->_gen_aleatoire) {
        p->nb_element--;
        return generer_tuile();
    }

    Tuile tmp;
    int id;
    id = rand()%p->nb_element;
    tmp = p->_tab[id];

    p->_tab[id] = p->_tab[--p->nb_element];

    return tmp;
}

bool inserer_tuile(Pile *p, Tuile t)
{
    if (p->_gen_aleatoire)
        ca_error("insertion dans une pile en mode aléatoire");

    if (pile_pleine(*p))
        return false;

    p->_tab[p->nb_element++] = t;
    return true;
}

void detruire_pile(Pile *p)
{
    if (p->_gen_aleatoire)
        return;

    for (int i = 0; i < p->nb_element; i++) {
        free(p->_tab[i]);
    }
    free(p->_tab);
}
