#ifndef varstring_h
#define varstring_h

#include <stdlib.h>

#define VARSTRING_REALLOC_SIZE 16 // nombre d'octets ajoutés pour agrandir la chaine
_Static_assert(VARSTRING_REALLOC_SIZE > 0, "VARSTRING_REALLOC_SIZE doit etre supérieur à 0");

/* Le propriétaire d'une VarString est responsable de la libération de sa mémoire,
 * la passer par copie délègue cette responsabilité.
 * Il ne faut JAMAIS utiliser une VarString après l'avoir passé par copie. */
typedef struct _VarString {
    char *texte;
    size_t _len;
    size_t _capacite;
} VarString;

VarString creer_varstring(void);
void detruire_varstring(VarString chaine);

// Les fonctions suivantes n'acceptent que des chaines initalisées
// avec creer_varstring() ou { 0 }

/* Ne libère pas la mémoire occupé par la chaine,
 * la chaine est réutilisable après l'appel. */
void vider_varstring(VarString *chaine);

void ajouter_char(VarString *chaine, char c);
void retirer_char(VarString *chaine);
void ajouter_chaine(VarString *chaine, char *restrict autre); // fait une COPIE de autre
char *dupliquer_chaine(VarString *chaine);

#endif
