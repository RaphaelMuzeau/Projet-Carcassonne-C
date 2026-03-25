#include <stdlib.h>
#include <string.h>
#include "libca.h"
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

// fonction anonyme,
// Alloue si besoin l'espace necessaire à une chaine de la
// longueur donnée ('\0' non inclus)
void agrandir_varstring(VarString *chaine, size_t taille)
{
    if (chaine == NULL) return;
    if (taille < chaine->capacite) return;

    // arrondit la nouvelle capacite au prochain multiple de VARSTRING_REALLOC_SIZE
    size_t new_cap = taille + (VARSTRING_REALLOC_SIZE - taille % VARSTRING_REALLOC_SIZE);

    chaine->texte = ca_realloc(chaine->texte, new_cap, sizeof(char));
    chaine->capacite = new_cap;
}

/* manipulation de la chaine */
void vider_varstring(VarString *chaine)
{
    if (chaine == NULL) return;

    chaine->texte[0] = '\0';
    chaine->len      = 0;
}

void ajouter_char(VarString *chaine, char c)
{
    if (chaine == NULL) return;
    if (c == '\0') return;

    agrandir_varstring(chaine, chaine->len + 1);
    chaine->texte[chaine->len++] = c;
    chaine->texte[chaine->len]   = '\0';
}

void retirer_char(VarString *chaine)
{
    if (chaine == NULL) return;
    if (chaine->len == 0) return;

    chaine->texte[--chaine->len] = '\0';
}

void ajouter_chaine(VarString *chaine, char *restrict autre)
{
    if (chaine == NULL) return;
    if (autre == NULL) return;

    size_t new_len = chaine->len + strlen(autre);
    agrandir_varstring(chaine, new_len);

    strcpy(&chaine->texte[chaine->len], autre);
    chaine->len = new_len;
}
