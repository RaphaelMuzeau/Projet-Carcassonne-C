#include <math.h>
#include <string.h>
#include "libca.h"
#include "raylib.h"
#include "page.h"
#include "render.h"
#include "plateau.h"
#include "sidebar.h"
#include "popup.h"
#include "fichier.h"

enum Page page_jeu(Jeu *jeu)
{
    // Etat initial de la page
    enum Page prochaine_page = P_JEU;
    bool afficher_popup = false;
    bool centre_camera = false;

    // Etat initial du jeu
    Tuile t = recup_tuile(&jeu->pile);
    set(&jeu->grille, t, 0, 0); // on place la tuile racine

    // Charger la Spritesheet
    Texture spritesheet = LoadTexture("data/sprites/spritesheet.png");
    if (!IsTextureValid(spritesheet))
        ca_error("Echec lors du chargement de la spritesheet");

    // piocher la premiere tuile
    Tuile tuile = recup_tuile(&jeu->pile);
    RenderTexture2D render_tuile = generer_render_tuile(tuile, spritesheet);
    float rotation_tuile = 0.0f;

    // Elements de la page
    Plateau plateau = creer_plateau(jeu, spritesheet);
    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");
    Bouton centrer = creer_bouton_adapte(retour.champ.width + 30, retour.champ.y, "centrer");
    Controles ctrl = creer_controles(jeu->pile.nb_element);
    BarreJoueurs barrejoueurs = creer_barrejoueurs(jeu->joueurs);
    Popup popup = creer_popup();

    // Vue de la grille

    while (prochaine_page == P_JEU) {
        if (update_bouton(&retour) || WindowShouldClose())
            afficher_popup = true;

        centre_camera = false;
        if (update_bouton(&centrer))
            centre_camera = true;

        if (tuile == NULL)
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

        /* gestion du plateau */

        if (!afficher_popup && !centre_camera)
            update_plateau(&plateau);

        /* controles de la tuile */

        update_controles(&ctrl);

        if (update_bouton_adapte(&ctrl.rotation) || IsKeyPressed(KEY_R)) {
            pivot_90(t);
            rotation_tuile = fmod(rotation_tuile + 90.0f, 360.0f);
        }

        if (update_bouton_adapte(&ctrl.detruire) || IsKeyPressed(KEY_D)) {
            free(t);
            UnloadRenderTexture(render_tuile);
            rotation_tuile = 0;
            t = recup_tuile(&jeu->pile);
            render_tuile = generer_render_tuile(t, spritesheet);
        }

        /* barre joueurs */

        update_barrejoueurs(&barrejoueurs);

        /* dessin */

        BeginDrawing();
            ClearBackground(RAYWHITE);

            // dessin de la grille
            dessiner_plateau(plateau, render_tuile, rotation_tuile);

            // dessin de la sidebar
            dessiner_barrejoueurs(barrejoueurs, 3);
            dessiner_controles(ctrl, render_tuile, rotation_tuile);

            // dessin de l'interface
            dessiner_bouton(retour);
            dessiner_bouton(centrer);
            if (afficher_popup) dessiner_popup(popup);
        EndDrawing();
    }

    detruire_plateau(plateau);
    detruire_barrejoueurs(barrejoueurs);

    UnloadTexture(spritesheet);
    UnloadRenderTexture(render_tuile);

    detruire_jeu(*jeu);
    *jeu = (Jeu) { 0 };

    return prochaine_page;
}
