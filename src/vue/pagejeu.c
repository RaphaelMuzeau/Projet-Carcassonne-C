#include <string.h>
#include "libca.h"
#include "fichier.h"
#include "raylib.h"
#include "raymath.h"
#include "page.h"
#include "sidebar.h"
#include "popup.h"
#include "render.h"

enum Page page_jeu(Jeu *jeu)
{
    // Etat initial
    enum Page prochaine_page = P_JEU;
    bool afficher_popup = false;
    Vector2 position_curseur_ecran  = { 0 };
    Vector2 position_curseur_grille = { 0 };

    int max_chunks = (jeu->pile.nb_element * TEXTURE_SIZE * 2) / CHUNK_SIZE + 2;
    int nb_chunks = 0;
    Chunk *chunks = ca_alloc(sizeof(Chunk), max_chunks);

    printf("%d\n", max_chunks);

    // Elements de la page
    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");
    Controles ctrl = creer_controles();
    BarreJoueurs barrejoueurs = creer_barrejoueurs(jeu->joueurs);
    Popup popup = creer_popup();

    // Vue de la grille
    Rectangle vue = { 0 };
    Camera2D camera = { 0 };
    camera.zoom = 0.5f;

    Tuile t = creer_tuile();

    while (prochaine_page == P_JEU) {
        if (update_bouton(&retour) || WindowShouldClose())
            afficher_popup = true;

        /* reallouer la memoire necessaire à plus de chunks si besoin */
        if (nb_chunks == max_chunks) {
            max_chunks += 4;
            chunks = ca_realloc(chunks, sizeof(Chunk), max_chunks);
        }

        /* gestion de la vue de la grille */
        vue.width = GetScreenWidth() - SIDEBAR_WIDTH;
        vue.height = GetScreenHeight();
        position_curseur_ecran = GetMousePosition();
        position_curseur_grille = GetScreenToWorld2D(position_curseur_ecran, camera);

        if (!afficher_popup) {
            if (IsKeyDown(KEY_LEFT))  camera.target.x += -7.5f / camera.zoom;
            if (IsKeyDown(KEY_RIGHT)) camera.target.x +=  7.5f / camera.zoom;
            if (IsKeyDown(KEY_UP))    camera.target.y += -7.5f / camera.zoom;
            if (IsKeyDown(KEY_DOWN))  camera.target.y +=  7.5f / camera.zoom;

            if (CheckCollisionPointRec(position_curseur_ecran, vue)) {
                // Se deplacer un tirant avec le clique droit
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    Vector2 delta = GetMouseDelta();
                    delta = Vector2Scale(delta, -1.0f/camera.zoom);
                    camera.target = Vector2Add(camera.target, delta);
                }

                // Zoomer avec la molette
                float scroll = GetMouseWheelMove();
                if (scroll != 0) {
                    // Garder la souris à la meme position sur l'ecran et la grille pendant le zoom
                    camera.offset = position_curseur_ecran;
                    camera.target = position_curseur_grille;

                    // Incremeneter le zoom avec une échelle logarithmique
                    float echelle = 0.2f*scroll;
                    camera.zoom = Clamp(expf(logf(camera.zoom)+echelle), 0.0125f, 64.0f);
                }

                // Placer une tuile avec le clique gauche
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    RenderTexture2D texture_tuile = generer_texture(t);
                    dessiner_tuile(chunks, &nb_chunks, texture_tuile, position_curseur_grille);
                }
            }
        }

        /* gestion du popup de sauvegarde */
        if (afficher_popup) {
            centrer_popup(&popup);
            update_champsaisie(&popup.champ_partie);

            if (update_bouton_centre(&popup.annuler))
                afficher_popup = false;

            if (update_bouton_adapte(&popup.quitter))
                prochaine_page = P_TITRE;

            if (update_bouton_adapte(&popup.sauvegarder) && popup.champ_partie.saisie.len != 0) {
                char *fname = ca_alloc(sizeof(CHEMIN_PARTIES) + popup.champ_partie.saisie.len, sizeof(char));
                memcpy(fname, CHEMIN_PARTIES, sizeof(CHEMIN_PARTIES));
                strcat(fname, popup.champ_partie.saisie.texte);

                if (sauvegarder_partie(*jeu, fname))
                    prochaine_page = P_TITRE;

                free(fname);
            }
        }

        update_controles(&ctrl);
        update_barrejoueurs(&barrejoueurs);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
                // dessiner la tuile actuelle sur le curseur
                DrawRectangle(multiple_inf(position_curseur_grille.x, TEXTURE_SIZE),
                              multiple_inf(position_curseur_grille.y, TEXTURE_SIZE),
                              TEXTURE_SIZE, TEXTURE_SIZE,
                              (Color) { 0, 228, 48, 128});


                DrawRectangle(256, 256, 256, 256, GREEN);

                for (int i = 0; i < nb_chunks; i++) {
                    // les textures inversent l'axe y par defaut, on doit le
                    // remettre dans le bon sens en inversant le rectangle de source.
                    Rectangle source_chunk = { .width = CHUNK_SIZE, .height = -CHUNK_SIZE };
                    Vector2 pos = { chunks[i].x, chunks[i].y };
                    DrawTextureRec(chunks[i].render.texture, source_chunk, pos, WHITE);
                }
            EndMode2D();

            dessiner_barrejoueurs(barrejoueurs, 3);
            dessiner_controles(ctrl);

            dessiner_bouton(retour);
            if (afficher_popup) dessiner_popup(popup);

        EndDrawing();
    }
    free(t);

    for (int i = 0; i < nb_chunks; i++)
        UnloadRenderTexture(chunks[i].render);
    free(chunks);

    detruire_barrejoueurs(barrejoueurs);
    detruire_jeu(*jeu);
    *jeu = (Jeu) { 0 };
    return prochaine_page;
}
