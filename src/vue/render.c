#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "libca.h"
#include "tuile.h"
#include "render.h"

/* operations necessaires au placement des render */

int multiple_inf(float x, int p)
{
    return (int) nearbyint(x) & ~(p - 1);
}

int mod(int x, int z)
{
    int res = x % z;
    if (res < 0)
        res += z;
    return res;
}

/* generation des render */

void dessiner_sprite(Texture spritesheet, enum Zone zone, enum Sprite sprite, float rotation)
{
    if (zone == Z_PRE) return;

    Rectangle dest   = { .x = 0, .y = 0, .width = TEXTURE_SIZE, .height = TEXTURE_SIZE };
    Rectangle source = dest;

    // se referer à la spritesheet pour ces decalages

    if (zone & (Z_ABBAYE | Z_VILLAGE))         source.y = TEXTURE_SIZE * 2;
    if (zone == Z_ROUTE)                       source.y = TEXTURE_SIZE;

    if (zone & (Z_ROUTE | Z_BLASON | Z_VILLE)) source.x = TEXTURE_SIZE * sprite;
    if (zone == Z_VILLAGE)                     source.x = TEXTURE_SIZE;

    Vector2 origin = { TEXTURE_SIZE / 2.0f, TEXTURE_SIZE / 2.0f };
    dest.x += origin.x;
    dest.y += origin.y;

    DrawTexturePro(spritesheet, source, dest, origin, rotation, WHITE);
}

RenderTexture2D generer_render_tuile(Tuile t, Texture spritesheet, float *rotation)
{
    RenderTexture2D render = LoadRenderTexture(TEXTURE_SIZE, TEXTURE_SIZE);
    if (t == NULL) return render;

    BeginTextureMode(render);
    ClearBackground(PRE_COLOR);

    *rotation = 0.0f;

    if (t->milieu & (Z_PRE | Z_VILLAGE | Z_ABBAYE)) {
        // dessiner le sprite correspondant à chaque coté
        for (enum Direction d = 0; d < D_MILIEU; d++)
            dessiner_sprite(spritesheet, zone_tuile(t, d), S_COTE, 90.0f * d);
        // dessiner l'element central
        dessiner_sprite(spritesheet, t->milieu, 0, 0.0f);
    } else {
        // pour simplifier la detection de sprite, on tourne la tuile
        // jusqu'à atteindre une configuration connue.
        int nb_pivots;
        for (nb_pivots = 0; nb_pivots < 4; ++nb_pivots) {
            if        (t->milieu == t->ouest && t->milieu == t->nord && t->milieu == t->est && t->milieu == t->sud) {
                dessiner_sprite(spritesheet, t->milieu, S_PLEIN, 0.0f);
                break;
            } else if (t->milieu == t->ouest && t->milieu == t->nord && t->milieu == t->est && t->milieu != t->sud) {
                dessiner_sprite(spritesheet, t->sud,    S_COTE,           180.0f);
                dessiner_sprite(spritesheet, t->milieu, S_OUEST_NORD_EST,  0.0f);
                break;
            } else if (t->milieu == t->ouest && t->milieu == t->nord && t->milieu != t->est && t->milieu != t->sud) {
                dessiner_sprite(spritesheet, t->sud,          S_COTE, 180.0f);
                dessiner_sprite(spritesheet, t->est,          S_COTE,  90.0f);
                dessiner_sprite(spritesheet, t->milieu, S_OUEST_NORD,   0.0f);
                break;
            } else if (t->milieu != t->ouest && t->milieu == t->nord && t->milieu != t->est && t->milieu == t->sud) {
                dessiner_sprite(spritesheet, t->ouest,      S_COTE, 270.0f);
                dessiner_sprite(spritesheet, t->est,        S_COTE,  90.0f);
                dessiner_sprite(spritesheet, t->milieu, S_SUD_NORD,   0.0f);
                break;
            } else
                pivot_90(t);
        }
        if (nb_pivots == 4) ca_warn("Le sprite de la tuile courante n'a pas pu être generé");

        // puis on indique la rotation et on la remet dans le bon sens
        *rotation = fmod(90.0f * (4 - nb_pivots), 360.f);
        for (int pivots = 0; pivots < 4 - nb_pivots; pivots++)
            pivot_90(t);
    }

    EndTextureMode();

    return render;
}

/* dessin des chunks et tuiles */

void dessiner_chunk(Chunk chunk)
{
    Rectangle source = { .width = CHUNK_SIZE, .height = -CHUNK_SIZE };
    Vector2 pos = { chunk.x, chunk.y  };
    DrawTextureRec(chunk.render.texture, source, pos, WHITE);
}

void dessiner_tuile(RenderTexture2D render_tuile, Vector2 position, float rotation, int alpha)
{
    // aligner la tuile à une 'case' sur la grille
    position.x = multiple_inf(position.x, TEXTURE_SIZE);
    position.y = multiple_inf(position.y, TEXTURE_SIZE);

    // dessiner sa texture
    Vector2 origin = { TEXTURE_SIZE / 2.0f, TEXTURE_SIZE / 2.0f };
    Rectangle source_tuile = { .width = TEXTURE_SIZE, .height = -TEXTURE_SIZE };
    Rectangle dest_tuile   = { .width = TEXTURE_SIZE, .height =  TEXTURE_SIZE };
    dest_tuile.x = position.x + TEXTURE_SIZE/2.0f;
    dest_tuile.y = position.y + TEXTURE_SIZE/2.0f;
    Color base = { 255, 255, 255, alpha };

    DrawTexturePro(render_tuile.texture, source_tuile, dest_tuile, origin, rotation, base);
}
