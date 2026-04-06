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
    // Etat initial de la page
    enum Page prochaine_page = P_JEU;
    bool afficher_popup = false;
    bool centre_camera = false;
    Vector2 position_curseur_ecran  = { 0 };
    Vector2 position_curseur_grille = { 0 };

    // Etat initial du jeu
    Tuile t = recup_tuile(&jeu->pile);
    set(&jeu->grille, t, 0, 0);
    int tour_joueur = 0;

    // Renderer
    int max_chunks = jeu->pile.nb_element * TEXTURE_SIZE * 2 / CHUNK_SIZE + 2;
    int nb_chunks  = 0;
    Chunk *chunks  = ca_alloc(sizeof(Chunk), max_chunks);
    Vector2 pos_tuile = { 0 };

    Texture spritesheet = LoadTexture("data/sprites/spritesheet.png");

    // dessiner la tuile 0, 0
    RenderTexture2D render_tuile = generer_render_tuile(t, spritesheet);
    dessiner_tuile(chunks, &nb_chunks, render_tuile, (Vector2) { 0.0f, 0.0f });

    // piocher la premiere tuile
    t = recup_tuile(&jeu->pile);
    render_tuile = generer_render_tuile(t, spritesheet);

    // Elements de la page
    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");
    Bouton centrer = creer_bouton_adapte(retour.champ.width + 30, retour.champ.y, "centrer");
    Controles ctrl = creer_controles();
    BarreJoueurs barrejoueurs = creer_barrejoueurs(jeu->joueurs);
    Popup popup = creer_popup();

    // Vue de la grille
    Rectangle vue = { 0 };
    Camera2D camera = { 0 };
    camera.zoom = 0.5f;

    while (prochaine_page == P_JEU) {
        if (update_bouton(&retour) || WindowShouldClose())
            afficher_popup = true;

        if (update_bouton(&centrer))
            centre_camera = true;

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

        pos_tuile.x = multiple_inf(position_curseur_grille.x, TEXTURE_SIZE);
        pos_tuile.y = multiple_inf(position_curseur_grille.y, TEXTURE_SIZE);

        if (!afficher_popup && !centre_camera) {
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
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tour(jeu, t, pos_tuile.x / TEXTURE_SIZE, pos_tuile.y / TEXTURE_SIZE, -1, 0, false)) {
                    dessiner_tuile(chunks, &nb_chunks, render_tuile, position_curseur_grille);
                    rafraichir_barrejoueurs(&barrejoueurs);

                    tour_joueur = (tour_joueur + 1) % jeu->joueurs.nb_joueurs;
                    t = recup_tuile(&jeu->pile);
                    render_tuile = generer_render_tuile(t, spritesheet);
                }
            }
        }

        if (t == NULL)
            prochaine_page = P_TITRE;

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

            /* dessin de la grille */
            BeginMode2D(camera);
                // les textures inversent l'axe y par defaut, on doit le
                // remettre dans le bon sens en inversant le rectangle de source.

                // dessiner la tuile actuelle sur le curseur
                Vector2 origin = { TEXTURE_SIZE / 2.0f, TEXTURE_SIZE / 2.0f };
                Rectangle source_tuile = { .width = TEXTURE_SIZE, .height = -TEXTURE_SIZE };
                Rectangle dest_tuile   = { .width = TEXTURE_SIZE, .height =  TEXTURE_SIZE };
                dest_tuile.x = pos_tuile.x + TEXTURE_SIZE/2.0f;
                dest_tuile.y = pos_tuile.y + TEXTURE_SIZE/2.0f;
                Color base = { 255, 255, 255, 128 }; // la tuile est semi-transparente
                DrawTexturePro(render_tuile.texture, source_tuile, dest_tuile, origin, 0.0f, base);

                // dessiner chaque chunk
                for (int i = 0; i < nb_chunks; i++) {
                    Rectangle source_chunk = { .width = CHUNK_SIZE, .height = -CHUNK_SIZE };
                    Vector2 pos = { chunks[i].x, chunks[i].y };
                    DrawTextureRec(chunks[i].render.texture, source_chunk, pos, WHITE);
                }
            EndMode2D();

            /* dessin de la sidebar */
            dessiner_barrejoueurs(barrejoueurs, 3);
            dessiner_controles(ctrl);

            /* dessin de l'interface */
            dessiner_bouton(retour);
            if (afficher_popup) dessiner_popup(popup);

            dessiner_bouton(centrer);
            if (centre_camera) {
                camera.target.x = 0;
                camera.target.y = 0;
                camera.offset = (Vector2) {vue.width / 2, vue.height / 2};
                centre_camera = false;
            }
        EndDrawing();
    }

    for (int i = 0; i < nb_chunks; i++)
        UnloadRenderTexture(chunks[i].render);
    free(chunks);

    UnloadRenderTexture(render_tuile);
    UnloadTexture(spritesheet);

    detruire_barrejoueurs(barrejoueurs);

    detruire_jeu(*jeu);
    *jeu = (Jeu) { 0 };

    return prochaine_page;
}
