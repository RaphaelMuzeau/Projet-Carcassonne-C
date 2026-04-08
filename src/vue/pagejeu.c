#include <math.h>
#include <string.h>
#include "libca.h"
#include "raylib.h"
#include "page.h"
#include "jeu.h"
#include "render.h"
#include "plateau.h"
#include "vuemeeple.h"
#include "sidebar.h"
#include "popup.h"
#include "fichier.h"

enum Page page_jeu(Jeu *jeu)
{
    // Etat initial de la page
    enum Page prochaine_page = P_JEU;
    bool afficher_popup = false;
    bool centre_camera = false;

    SetExitKey(KEY_NULL);

    // Charger la Spritesheet
    Texture spritesheet = LoadTexture("data/sprites/spritesheet.png");
    if (!IsTextureValid(spritesheet))
        ca_error("Echec lors du chargement de la spritesheet");

    // Piocher la premiere tuile
    Tuile tuile = recup_tuile(&jeu->pile);
    RenderTexture2D render_tuile = generer_render_tuile(tuile, spritesheet);
    float rotation_tuile = 0.0f;

    // Elements de la page
    Plateau plateau = creer_plateau(jeu, spritesheet);
    ListePlacements placements_meeple = creer_listeplacements();
    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");
    Bouton centrer = creer_bouton_adapte(retour.champ.width + 30, retour.champ.y, "centrer");
    Controles ctrl = creer_controles(jeu->pile.nb_element);
    BarreJoueurs barrejoueurs = creer_barrejoueurs(jeu->joueurs);
    Popup popup = creer_popup();

    // Vue de la grille

    while (prochaine_page == P_JEU) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (plateau.placement) plateau.placement = false;
            else afficher_popup = true;
        }

        if (update_bouton(&retour) || WindowShouldClose())
            afficher_popup = true;

        centre_camera = false;
        if (update_bouton(&centrer)) {
            plateau.camera.target = (Vector2) { 0.0f, 0.0f };
            plateau.camera.offset.x = plateau.vue.width  / 2.0f - TEXTURE_SIZE / 2.0f;
            plateau.camera.offset.y = plateau.vue.height / 2.0f - TEXTURE_SIZE / 2.0f;

            centre_camera = true; // evite de mettre à jour le plateau au moment du clique
        }

        if (tuile == NULL)
            prochaine_page = P_TITRE;

        /* gestion du plateau */

        if (!afficher_popup && !centre_camera) {
            PlacementTuile placement = update_plateau(&plateau);
            if (placement.x != 0 || placement.y != 0) {
                if (tour(jeu, tuile, placement.x, placement.y, placement.placer_meeple, placement.position_meeple)) {
                    // mettre à jour l'hud
                    rafraichir_controles(&ctrl, jeu->pile.nb_element);
                    rafraichir_barrejoueurs(&barrejoueurs);
                    rafraichir_listeplacements(&placements_meeple, jeu);
                    placer_render_tuile(&plateau, render_tuile, placement.x, placement.y, rotation_tuile);

                    // piocher la prochaine tuile
                    UnloadRenderTexture(render_tuile);
                    tuile          = recup_tuile(&jeu->pile);
                    render_tuile   = generer_render_tuile(tuile, spritesheet);
                    rotation_tuile = 0.0f;
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

        /* controles de la tuile */

        update_controles(&ctrl);

        if (!plateau.placement && (update_bouton_adapte(&ctrl.rotation) || IsKeyPressed(KEY_R))) {
            pivot_90(tuile);
            rotation_tuile = fmod(rotation_tuile + 90.0f, 360.0f);
        }

        if (!plateau.placement && (update_bouton_adapte(&ctrl.detruire) || IsKeyPressed(KEY_D))) {
            free(tuile);
            UnloadRenderTexture(render_tuile);
            rotation_tuile = 0;
            tuile = recup_tuile(&jeu->pile);
            render_tuile = generer_render_tuile(tuile, spritesheet);

            rafraichir_controles(&ctrl, jeu->pile.nb_element);
        }

        /* barre joueurs */

        update_barrejoueurs(&barrejoueurs);

        /* dessin */

        BeginDrawing();
            ClearBackground(RAYWHITE);

            // dessin de la grille
            dessiner_plateau(plateau, render_tuile, rotation_tuile);
            dessiner_listeplacements(placements_meeple, plateau);

            // dessin de la sidebar
            dessiner_barrejoueurs(barrejoueurs, jeu->joueurs.tour);
            dessiner_controles(ctrl, render_tuile, rotation_tuile);

            // dessin de l'interface
            dessiner_bouton(retour);
            dessiner_bouton(centrer);
            if (afficher_popup) dessiner_popup(popup);
        EndDrawing();
    }

    free(tuile);

    detruire_plateau(plateau);
    detruire_listeplacements(placements_meeple);
    detruire_barrejoueurs(barrejoueurs);
    detruire_controles(ctrl);

    UnloadTexture(spritesheet);
    UnloadRenderTexture(render_tuile);

    detruire_jeu(*jeu);
    *jeu = (Jeu) { 0 };

    SetExitKey(KEY_ESCAPE);

    return prochaine_page;
}
