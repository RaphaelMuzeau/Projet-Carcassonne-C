#include "libca.h"
#include "raylib.h"
#include "raymath.h"
#include "plateau.h"
#include "render.h"
#include "sidebar.h"
#include "jeu.h"

Plateau creer_plateau(Jeu *jeu, Texture spritesheet)
{
    Plateau plateau = { 0 };

    // initialisation des chunks
    plateau.max_chunks = jeu->pile.nb_element * TEXTURE_SIZE * 2 / CHUNK_SIZE + 2;
    plateau.nb_chunks  = 0;
    plateau.chunks  = ca_alloc(sizeof(Chunk), plateau.max_chunks);

    // etat initial de la vue
    plateau.vue.x = plateau.vue.y = 0;
    plateau.vue.width  = GetScreenWidth() - SIDEBAR_WIDTH;
    plateau.vue.height = GetScreenHeight();

    plateau.camera.target = (Vector2) { 0, 0 };
    plateau.camera.zoom   = 0.5f;
    plateau.camera.offset = (Vector2) { plateau.vue.width/2.0f, plateau.vue.height/2.0f };
    plateau.camera.offset = Vector2Scale(plateau.camera.offset, plateau.camera.zoom);

    // afficher la tuile racine
    RenderTexture2D render = generer_render_tuile(get(jeu->grille, 0, 0), spritesheet);
    placer_render_tuile(&plateau, render, (Vector2) { 0, 0 }, 0.0f);

    return plateau;
}

void detruire_plateau(Plateau plateau)
{
    for (int i = 0; i < plateau.nb_chunks; i++)
        UnloadRenderTexture(plateau.chunks[i].render);
    free(plateau.chunks);
}

void update_plateau(Plateau *plateau)
{
    plateau->vue.width = GetScreenWidth() - SIDEBAR_WIDTH;
    plateau->vue.height = GetScreenHeight();

    Vector2 position_curseur_ecran = GetMousePosition();
    Vector2 position_curseur_grille = GetScreenToWorld2D(position_curseur_ecran, plateau->camera);

    // il n'y a rien à faire si le curseur n'est pas sur le plateau
    if (!CheckCollisionPointRec(position_curseur_ecran, plateau->vue))
        return;

    /* Deplacement */

    // se deplacer avec la croix directionelle
    if (IsKeyDown(KEY_LEFT))  plateau->camera.target.x += -7.5f / plateau->camera.zoom;
    if (IsKeyDown(KEY_RIGHT)) plateau->camera.target.x +=  7.5f / plateau->camera.zoom;
    if (IsKeyDown(KEY_UP))    plateau->camera.target.y += -7.5f / plateau->camera.zoom;
    if (IsKeyDown(KEY_DOWN))  plateau->camera.target.y +=  7.5f / plateau->camera.zoom;

    // se deplacer un tirant avec le clique droit
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f / plateau->camera.zoom);
        plateau->camera.target = Vector2Add(plateau->camera.target, delta);
    }

    /* Zoom */

    // zoomer avec la molette
    float scroll = GetMouseWheelMove();
    if (scroll != 0) {
        // Garder la souris à la meme position sur l'ecran et la grille pendant le zoom
        plateau->camera.offset = position_curseur_ecran;
        plateau->camera.target = position_curseur_grille;

        // Incrementer le zoom avec une échelle logarithmique
        float echelle = 0.2f*scroll;
        plateau->camera.zoom = Clamp(expf(logf(plateau->camera.zoom) + echelle), 0.0125f, 64.0f);
    }
}

void dessiner_plateau(Plateau plateau, RenderTexture2D render_tuile, float rotation)
{
    // la tuile doit etre dessiné sur la case sous le curseur
    Vector2 position = GetScreenToWorld2D(GetMousePosition(), plateau.camera);
    position.x = multiple_inf(position.x, TEXTURE_SIZE);
    position.y = multiple_inf(position.y, TEXTURE_SIZE);

    BeginMode2D(plateau.camera);
        // dessiner la tuile courante avec une transparence de 50%
        dessiner_tuile(render_tuile, position, rotation, 128);

        // dessiner tous les chunks
        for (int i = 0; i < plateau.nb_chunks; i++)
            dessiner_chunk(plateau.chunks[i]);
    EndMode2D();
}

void placer_render_tuile(Plateau *plateau, RenderTexture2D render, Vector2 position, float rotation)
{
    int x_chunk = multiple_inf(position.x, CHUNK_SIZE);
    int y_chunk = multiple_inf(position.y, CHUNK_SIZE);

    position.x = mod(multiple_inf(position.x, TEXTURE_SIZE), CHUNK_SIZE);
    position.y = mod(multiple_inf(position.y, TEXTURE_SIZE), CHUNK_SIZE);

    // On cherche si le chunk requis existe déjà
    for (int i = 0; i < plateau->nb_chunks; i++) {
        if (plateau->chunks[i].x == x_chunk && plateau->chunks[i].y == y_chunk) {
            BeginTextureMode(plateau->chunks[i].render);
                dessiner_tuile(render, position, rotation, 255);
            EndTextureMode();
            return;
        }
    }

    // Sinon, il faut le creer
    plateau->chunks[plateau->nb_chunks].render = LoadRenderTexture(CHUNK_SIZE, CHUNK_SIZE);
    plateau->chunks[plateau->nb_chunks].x = x_chunk;
    plateau->chunks[plateau->nb_chunks].y = y_chunk;

    BeginTextureMode(plateau->chunks[plateau->nb_chunks].render);
        dessiner_tuile(render, position, rotation, 255);
    EndTextureMode();

    ++plateau->nb_chunks;
}
