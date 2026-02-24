#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "varstring.h"

/* gestion de mémoire */
VarString creer_varstring(void)
{
    return (VarString) { 0 };
}

void detruire_varstring(VarString chaine)
{
    free(chaine.texte);
}

/* manipulation de la chaine */
void vider_varstring(VarString *chaine)
{
    if (chaine == NULL) return;

    chaine->texte[0] = '\0';
    chaine->len     = 0;
}

void ajouter_char(VarString *chaine, char c)
{
    if (chaine == NULL) return;
    if (c == '\0') return;

    size_t required_cap = chaine->len + VARSTRING_REALLOC_SIZE;

    // agrandir la chaine si necessaire
    if (required_cap >= chaine->capacite) {
        chaine->texte = realloc(chaine->texte, required_cap);
        if (chaine->texte == NULL)
            perror("carcassonne");
        chaine->capacite = required_cap;
    }

    chaine->texte[chaine->len++] = c;
    chaine->texte[chaine->len]   = '\0';
}

void ajouter_chaine(VarString *chaine, char *restrict autre)
{
    if (chaine == NULL) return;
    if (autre == NULL) return;

    size_t l = strlen(autre);
    size_t new_len = chaine->len + l;

    // agrandir la chaine si necessaire
    if (new_len >= chaine->capacite) {
        chaine->texte = realloc(chaine->texte, new_len + 1);
        if (chaine->texte == NULL)
            perror("carcassonne");
        chaine->capacite = new_len + 1;
    }

    strcpy(&chaine->texte[chaine->len], autre);
    chaine->len = new_len;
}
