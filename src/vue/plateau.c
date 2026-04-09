#include "libca.h"
#include "raylib.h"
#include "raymath.h"
#include "plateau.h"
#include "render.h"
#include "sidebar.h"
#include "jeu.h"
#include "grille.h"

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

    plateau.camera.zoom   = 0.5f;
    plateau.camera.target = (Vector2) { 0.0f, 0.0f };
    plateau.camera.offset.x = plateau.vue.width  / 2.0f - TEXTURE_SIZE / 2.0f;
    plateau.camera.offset.y = plateau.vue.height / 2.0f - TEXTURE_SIZE / 2.0f;
    plateau.camera.offset = Vector2Scale(plateau.camera.offset, plateau.camera.zoom);

    // intialiser les boutons de placement de meeple
    plateau.placement = false;

    plateau.nord = creer_bouton(0, 0, NULL);
    plateau.nord.champ.height = TEXTURE_SIZE * 0.2f;
    plateau.nord.champ.width  = TEXTURE_SIZE * 0.2f;
    plateau.sud = plateau.est = plateau.ouest = plateau.milieu = plateau.aucun = plateau.nord;

    plateau.aucun.texte.contenu = "x";
    adapter_bouton(&plateau.aucun);

    // afficher toutes les tuiles placés
    dessiner_grille(&plateau, spritesheet, jeu->grille, 0, 0);
    recherche_est_verifie(jeu->grille, 0, 0);

    return plateau;
}

void detruire_plateau(Plateau plateau)
{
    for (int i = 0; i < plateau.nb_chunks; i++)
        UnloadRenderTexture(plateau.chunks[i].render);
    free(plateau.chunks);
}

PlacementTuile update_plateau(Plateau *plateau)
{
    PlacementTuile placement = { 0 };

    plateau->vue.width = GetScreenWidth() - SIDEBAR_WIDTH;
    plateau->vue.height = GetScreenHeight();

    Vector2 position_curseur_ecran = GetMousePosition();
    Vector2 position_curseur_grille = GetScreenToWorld2D(position_curseur_ecran, plateau->camera);

    // il n'y a rien à faire si le curseur n'est pas sur le plateau
    if (!CheckCollisionPointRec(position_curseur_ecran, plateau->vue))
        return placement;

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

    /* Placement */

    // ces deux phases doivent prendre place à des frames differentes
    // pour eviter qu'un clic place la tuile et pose un meeple en meme temps

    /* Phase 1: placement de tuile */

    if (!plateau->placement) {
        // sauvegarder la position de la tuile
        plateau->pos_tuile.x = multiple_inf(position_curseur_grille.x, TEXTURE_SIZE);
        plateau->pos_tuile.y = multiple_inf(position_curseur_grille.y, TEXTURE_SIZE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            plateau->placement = true; // la position de la tuile est maintenant verrouillé

            // disposer les boutons de placement de meeple
            plateau->ouest.champ.x  = plateau->pos_tuile.x + TEXTURE_SIZE * 0.1f;
            plateau->est.champ.x    = plateau->pos_tuile.x + TEXTURE_SIZE * 0.7f;
            plateau->ouest.champ.y  = plateau->pos_tuile.y + TEXTURE_SIZE * 0.40f;
            plateau->est.champ.y    = plateau->pos_tuile.y + TEXTURE_SIZE * 0.40f;

            plateau->nord.champ.x   = plateau->pos_tuile.x + TEXTURE_SIZE * 0.40f;
            plateau->sud.champ.x    = plateau->pos_tuile.x + TEXTURE_SIZE * 0.40f;
            plateau->nord.champ.y   = plateau->pos_tuile.y + TEXTURE_SIZE * 0.1f;
            plateau->sud.champ.y    = plateau->pos_tuile.y + TEXTURE_SIZE * 0.7f;

            plateau->milieu.champ.x = plateau->pos_tuile.x + TEXTURE_SIZE * 0.40f;
            plateau->milieu.champ.y = plateau->pos_tuile.y + TEXTURE_SIZE * 0.40f;

            plateau->aucun.champ.x  = plateau->pos_tuile.x + TEXTURE_SIZE * 0.8f;
            plateau->aucun.champ.y  = plateau->pos_tuile.y + TEXTURE_SIZE * 0.1f;
            adapter_bouton(&plateau->aucun);
        }
    }

    /* Phase 2: placement de meeple */

    else {
        placement.placer_meeple = true;
        if (update_bouton_camera(&plateau->nord,   plateau->camera)) { placement.position_meeple = D_NORD;   plateau->placement = false; }
        if (update_bouton_camera(&plateau->sud,    plateau->camera)) { placement.position_meeple = D_SUD;    plateau->placement = false; }
        if (update_bouton_camera(&plateau->est,    plateau->camera)) { placement.position_meeple = D_EST;    plateau->placement = false; }
        if (update_bouton_camera(&plateau->ouest,  plateau->camera)) { placement.position_meeple = D_OUEST;  plateau->placement = false; }
        if (update_bouton_camera(&plateau->milieu, plateau->camera)) { placement.position_meeple = D_MILIEU; plateau->placement = false; }
        if (update_bouton_camera(&plateau->aucun,  plateau->camera)) { placement.placer_meeple = false;      plateau->placement = false; }

        // on a terminé le placement actuel
        if (!plateau->placement) {
            placement.x = plateau->pos_tuile.x / TEXTURE_SIZE;
            placement.y = plateau->pos_tuile.y / TEXTURE_SIZE;
        }
    }

    return placement;
}

void dessiner_plateau(Plateau plateau, RenderTexture2D render_tuile, float rotation)
{
    BeginMode2D(plateau.camera);
        // dessiner tous les chunks
        for (int i = 0; i < plateau.nb_chunks; i++)
            dessiner_chunk(plateau.chunks[i]);

        // dessiner la tuile courante avec une transparence de 50%
        dessiner_tuile(render_tuile, plateau.pos_tuile, rotation, 128);

        // dessiner les boutons de placement de meeple
        if (plateau.placement) {
            dessiner_bouton(plateau.nord);
            dessiner_bouton(plateau.sud);
            dessiner_bouton(plateau.est);
            dessiner_bouton(plateau.ouest);
            dessiner_bouton(plateau.milieu);
            dessiner_bouton(plateau.aucun);
        }
    EndMode2D();
}

void dessiner_grille(Plateau *plateau, Texture spritesheet, Vec2D grille, int x, int y)
{
    Tuile t = get(grille, x, y);

    if (t == NULL) return;
    if (t->est_verifie) return;

    t->est_verifie = true;

    float rotation = 0.0f;
    RenderTexture2D render = generer_render_tuile(t, spritesheet, &rotation);
    placer_render_tuile(plateau, render, x, y, rotation);

    dessiner_grille(plateau, spritesheet, grille, x-1, y);
    dessiner_grille(plateau, spritesheet, grille, x+1, y);
    dessiner_grille(plateau, spritesheet, grille, x, y-1);
    dessiner_grille(plateau, spritesheet, grille, x, y+1);
}

void placer_render_tuile(Plateau *plateau, RenderTexture2D render, int x, int y, float rotation)
{
    // On passe de la position dans la grille à la position graphique
    x *= TEXTURE_SIZE;
    y *= TEXTURE_SIZE;

    int x_chunk = multiple_inf(x, CHUNK_SIZE);
    int y_chunk = multiple_inf(y, CHUNK_SIZE);
    Vector2 position_tuile = { mod(x, CHUNK_SIZE), mod(y, CHUNK_SIZE) };

    // On cherche si le chunk requis existe déjà
    for (int i = 0; i < plateau->nb_chunks; i++) {
        if (plateau->chunks[i].x == x_chunk && plateau->chunks[i].y == y_chunk) {
            BeginTextureMode(plateau->chunks[i].render);
                dessiner_tuile(render, position_tuile, rotation, 255);
            EndTextureMode();
            return;
        }
    }

    // Reallouer si besoin
    if (plateau->nb_chunks == plateau->max_chunks) {
        plateau->chunks = ca_realloc(plateau->chunks, plateau->max_chunks + CHUNKS_REALLOC_SIZE, sizeof(Chunk));
        plateau->max_chunks += CHUNKS_REALLOC_SIZE;
    }

    // Sinon, il faut le creer
    plateau->chunks[plateau->nb_chunks].render = LoadRenderTexture(CHUNK_SIZE, CHUNK_SIZE);
    plateau->chunks[plateau->nb_chunks].x = x_chunk;
    plateau->chunks[plateau->nb_chunks].y = y_chunk;

    BeginTextureMode(plateau->chunks[plateau->nb_chunks].render);
        ClearBackground(GRAY);
        dessiner_tuile(render, position_tuile, rotation, 255);
    EndTextureMode();

    ++plateau->nb_chunks;
}
