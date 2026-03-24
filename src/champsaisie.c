#include "raylib.h"
#include "champsaisie.h"
#include "varstring.h"

ChampSaisie creer_champsaisie(float x, float y, float largeur, float hauteur, bool est_numerique)
{
    ChampSaisie cs = { 0 };

    cs.champ  = (Rectangle) { .x = x, .y = y, .width = largeur, .height = hauteur };
    cs.saisie = creer_varstring();
    cs.texte  = creer_texte(0, 0, cs.saisie.texte);
    cs.vue    = LoadRenderTexture(largeur - CHAMP_BORDER_SIZE*2, hauteur - CHAMP_BORDER_SIZE*2);
    cs.numerique = est_numerique;

    return cs;
}

void detruire_champsaisie(ChampSaisie cs)
{
    detruire_varstring(cs.saisie);
    UnloadRenderTexture(cs.vue);
}

void update_champsaisie(ChampSaisie *cs)
{
    /* manipulation du curseur */
    bool survole = CheckCollisionPointRec(GetMousePosition(), cs->champ);

    if (survole)
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    else if (!cs->focus)
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (survole)
            cs->focus = true;
        else {
            cs->focus = false;
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
    }

    /* entrée du texte */
    if (cs->focus) {
        int c = 0;
        while ((c = GetKeyPressed()) != 0) {
            if (c == KEY_BACKSPACE)
                retirer_char(&cs->saisie);
            if (c == KEY_DELETE)
                vider_varstring(&cs->saisie);
        }
        while ((c = GetCharPressed()) != 0) {
            if (!cs->numerique || ('0' <= c && c <= '9'))
                ajouter_char(&cs->saisie, c);
        }
    }
    cs->texte.contenu = cs->saisie.texte;

    /* mouvement du texte */
    float largeur_texte = mesurer_texte(cs->texte).x;
    cs->texte.position.x = cs->champ.width - largeur_texte - 10;
    if (cs->texte.position.x > 4)
        cs->texte.position.x = 4;

    /* dessin de la vue */
    BeginTextureMode(cs->vue);
        ClearBackground(LIGHTGRAY);
        dessiner_texte(cs->texte);
    EndTextureMode();
}

void dessiner_champsaisie(ChampSaisie cs)
{
    // les textures inversent l'axe y par defaut, on doit le
    // remettre dans le bon sens en inversant le rectangle de source.
    Rectangle source = { 0 };
    source.width  =  cs.vue.texture.width;
    source.height = -cs.vue.texture.height;

    // il faut decaler la vue pour laisser la place à la bordure
    Vector2 position = { 0 };
    position.x = cs.champ.x + CHAMP_BORDER_SIZE;
    position.y = cs.champ.y + CHAMP_BORDER_SIZE;

    DrawTextureRec(cs.vue.texture, source, position, WHITE);
    DrawRectangleLinesEx(cs.champ, 4, cs.focus ? DARKBLUE : BLACK);
}
