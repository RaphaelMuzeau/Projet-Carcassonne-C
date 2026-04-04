#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "tuile.h"
#include "render.h"

RenderTexture2D generer_texture(Tuile t)
{
    RenderTexture2D render = LoadRenderTexture(TEXTURE_SIZE, TEXTURE_SIZE);
    if (t != NULL) {
        BeginTextureMode(render);
            DrawRectangle(0, 0, TEXTURE_SIZE, TEXTURE_SIZE, RED);
        EndTextureMode();
    }
    return render;
}

#include <stdio.h>
// dessine le render d'une tuile dans un chunk et le detruit
void dessiner_tuile(Chunk *chunks, int *nb_chunks, RenderTexture2D render_tuile, Vector2 position)
{
    int x_chunk = multiple_inf(position.x, CHUNK_SIZE);
    int y_chunk = multiple_inf(position.y, CHUNK_SIZE);

    int x_tuile = mod(multiple_inf(position.x, TEXTURE_SIZE), CHUNK_SIZE);
    int y_tuile = mod(multiple_inf(position.y, TEXTURE_SIZE), CHUNK_SIZE);

    fprintf(stderr, "%d %d, %d %d\n", x_chunk, y_chunk, x_tuile, y_tuile);

    // On cherche si le chunk requis existe déjà
    for (int i = 0; i < *nb_chunks; i++) {
        if (chunks[i].x == x_chunk && chunks[i].y == y_chunk) {
            BeginTextureMode(chunks[i].render);
                DrawTexture(render_tuile.texture, x_tuile, y_tuile, GREEN);
            EndTextureMode();
            UnloadRenderTexture(render_tuile);
            return;
        }
    }

    // Sinon, il faut le creer
    chunks[*nb_chunks].render = LoadRenderTexture(CHUNK_SIZE, CHUNK_SIZE);
    chunks[*nb_chunks].x = x_chunk;
    chunks[*nb_chunks].y = y_chunk;

    BeginTextureMode(chunks[*nb_chunks].render);
        bool paire = true;
        for (int i = 0; i < CHUNK_SIZE; i += TEXTURE_SIZE) {
            for (int j = 0; j < CHUNK_SIZE; j += TEXTURE_SIZE) {
                DrawRectangle(j, i, TEXTURE_SIZE, TEXTURE_SIZE, paire ? RED : BLUE);
                paire = !paire;
            }
            paire = !paire;
        }
        DrawTexture(render_tuile.texture, x_tuile, y_tuile, GREEN);
    EndTextureMode();
    UnloadRenderTexture(render_tuile);

    ++*nb_chunks;
}

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
