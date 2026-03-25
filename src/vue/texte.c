#include "raylib.h"
#include "texte.h"

Texte creer_texte(float x, float y, char *contenu)
{
    return (Texte) {
        .position = { x, y },
        .contenu = contenu,
        .couleur = TEXTE_DEFAULT_COLOR,
        .taille = TEXTE_DEFAULT_SIZE,
        .espacement = TEXTE_DEFAULT_SPACING,
        .police = GetFontDefault(),
    };
}

Vector2 mesurer_texte(Texte t)
{
    return MeasureTextEx(t.police, t.contenu, t.taille, t.espacement);
}

void dessiner_texte(Texte t)
{
    DrawTextEx(t.police, t.contenu, t.position, t.taille, t.espacement, t.couleur);
}
