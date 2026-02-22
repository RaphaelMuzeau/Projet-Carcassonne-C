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
    free(chaine.text);
}

/* manipulation de la chaine */
void vider_varstring(VarString *chaine)
{
    if (chaine == NULL) return;

    chaine->text[0] = '\0';
    chaine->len     = 0;
}

void ajouter_char(VarString *chaine, char c)
{
    if (chaine == NULL) return;
    if (c == '\0') return;

    size_t required_cap = chaine->len + VARSTRING_REALLOC_SIZE;

    // agrandir la chaine si necessaire
    if (required_cap >= chaine->capacity) {
        chaine->text = realloc(chaine->text, required_cap);
        if (chaine->text == NULL)
            perror("carcassonne");
        chaine->capacity = required_cap;
    }

    chaine->text[chaine->len++] = c;
    chaine->text[chaine->len]   = '\0';
}

void ajouter_chaine(VarString *chaine, char *restrict autre)
{
    if (chaine == NULL) return;
    if (autre == NULL) return;

    size_t l = strlen(autre);
    size_t new_len = chaine->len + l;

    // agrandir la chaine si necessaire
    if (new_len >= chaine->capacity) {
        chaine->text = realloc(chaine->text, new_len + 1);
        if (chaine->text == NULL)
            perror("carcassonne");
        chaine->capacity = new_len + 1;
    }

    strcpy(&chaine->text[chaine->len], autre);
    chaine->len = new_len;
}
