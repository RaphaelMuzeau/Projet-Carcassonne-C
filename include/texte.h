#ifndef texte_h
#define texte_h

#include "raylib.h"

#define TEXTE_DEFAULT_COLOR BLACK
#define TEXTE_DEFAULT_SIZE 25.0f
#define TEXTE_DEFAULT_SPACING 5.0f

typedef struct _Texte {
    Vector2 position;
    char *contenu; // pas responsable de la memoire de son contenu
    Color couleur;
    float taille;
    float espacement; // espace horizontal entre chaque caractere
    Font police;
} Texte;

// requiere le contexte OpenGL
Texte creer_texte(char *contenu, float x, float y);

Vector2 mesurer_texte(Texte t);
void dessiner_texte(Texte t);

#endif
