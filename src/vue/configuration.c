#include <stdio.h>
#include "libca.h"
#include "raylib.h"
#include "page.h"
#include "bouton.h"
#include "champsaisie.h"
#include "scrollbar.h"

enum Page page_configuration(bool custom)
{
    // Etat initial
    enum Page prochaine_page = P_CUSTOM;
    int largeur_ecran = GetScreenWidth();

    SetExitKey(KEY_NULL); // echape retourne à l'ecran titre au lieu de fermer la fenetre

    // Elements de la page
    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");
    Bouton confirmer = creer_bouton_adapte(0, 10, "confirmer ->");

    Texte titre_nb_joueur  = creer_texte(0, 100, "Nombre de joueurs");
    float largeur_titre_nb_joueur = mesurer_texte(titre_nb_joueur).x;
    ChampSaisie champ_nb_joueur = creer_champsaisie(0, 130, 150, 40, true);

    Texte titre_nb_meeple  = creer_texte(0, 230, "Nombre de meeple");
    float largeur_titre_nb_meeple = mesurer_texte(titre_nb_meeple).x;
    ChampSaisie champ_nb_meeple = creer_champsaisie(0, 260, 150, 40, true);

    Texte titre_nb_tuile  = creer_texte(0, 360, "Nombre de tuiles");
    float largeur_titre_nb_tuile = mesurer_texte(titre_nb_tuile).x;
    ChampSaisie champ_nb_tuile = creer_champsaisie(0, 390, 150, 40, true);

    while (prochaine_page == P_CUSTOM) {
        largeur_ecran = GetScreenWidth();

        if (WindowShouldClose())
                prochaine_page = P_QUITTER;

        if (update_bouton(&retour) || IsKeyPressed(KEY_ESCAPE))
                prochaine_page = P_TITRE;

        if (update_bouton(&confirmer)) {
            // TODO creer une partie avec config

            // on appelle une sous-page pour recuperer les noms de joueurs
            if (champ_nb_joueur.saisie.texte != NULL && champ_nb_joueur.saisie.texte[0] != '\0') {
                int nb_joueurs = 0;
                sscanf(champ_nb_joueur.saisie.texte, "%d", &nb_joueurs);
                prochaine_page = page_joueurs(nb_joueurs);

                // continue pour sauter le dessin de cette page
                if (prochaine_page != P_CUSTOM)
                    continue;
            }
        }

        /* realigner le bouton de confirmation à droite */
        confirmer.champ.x = largeur_ecran - confirmer.champ.width - 10;
        adapter_bouton(&confirmer); // recentre le texte après le decalage

        /* centrer les champs et leurs titres */
        champ_nb_joueur.champ.x = (float) largeur_ecran/2 - champ_nb_joueur.champ.width/2;
        titre_nb_joueur.position.x = (float) largeur_ecran/2 - largeur_titre_nb_joueur/2;
        update_champsaisie(&champ_nb_joueur);

        if (custom) {
            champ_nb_meeple.champ.x = (float) largeur_ecran/2 - champ_nb_meeple.champ.width/2;
            titre_nb_meeple.position.x = (float) largeur_ecran/2 - largeur_titre_nb_meeple/2;

            champ_nb_tuile.champ.x = (float) largeur_ecran/2 - champ_nb_tuile.champ.width/2;
            titre_nb_tuile.position.x = (float) largeur_ecran/2 - largeur_titre_nb_tuile/2;

            update_champsaisie(&champ_nb_meeple);
            update_champsaisie(&champ_nb_tuile);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            dessiner_bouton(retour);
            dessiner_bouton(confirmer);

            dessiner_texte(titre_nb_joueur);
            dessiner_champsaisie(champ_nb_joueur);
            if (custom) {
                dessiner_texte(titre_nb_meeple);
                dessiner_champsaisie(champ_nb_meeple);
                dessiner_texte(titre_nb_tuile);
                dessiner_champsaisie(champ_nb_tuile);
            }
        EndDrawing();
    }
    detruire_champsaisie(champ_nb_joueur);
    detruire_champsaisie(champ_nb_meeple);
    detruire_champsaisie(champ_nb_tuile);
    SetExitKey(KEY_ESCAPE);

    return prochaine_page;
}

enum Page page_joueurs(int nb_joueurs)
{
    // Etat initial
    enum Page prochaine_page = P_JOUEURS;

    Rectangle ecran = { 0 };
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    // Elements de la page
    ScrollBar scrollbar = creer_scrollbar(camera);

    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");
    Bouton confirmer = creer_bouton_adapte(0, 10, "confirmer ->");

    ChampSaisie *champs = ca_alloc(nb_joueurs, sizeof(ChampSaisie));
    Texte *titres = ca_alloc(nb_joueurs, sizeof(Texte));
    int fin_champs = 0;

    for (int i = 0; i < nb_joueurs; i++) {
        char *titre_i = ca_alloc(27, sizeof(char));
        snprintf(titre_i, 27, "Nom de joueur n°%d:", i);
        titres[i] = creer_texte(0, 100 + i*130, titre_i);
        champs[i] = creer_champsaisie(0, 130 + i*130, 400, 50, false);
        fin_champs = champs[i].champ.y + 70.0f;
    }

    while (prochaine_page == P_JOUEURS) {
        ecran.height = GetScreenHeight();
        ecran.width = GetScreenWidth();
        update_scrollbar(&scrollbar, ecran, fin_champs);

        if (WindowShouldClose())
            prochaine_page = P_QUITTER;

        if (update_bouton_camera(&retour, scrollbar.vue) || IsKeyPressed(KEY_ESCAPE))
            prochaine_page = P_CUSTOM;

        if (update_bouton_camera(&confirmer, scrollbar.vue))
            prochaine_page = P_JEU;

        /* realigner le bouton de confirmation à droite */
        confirmer.champ.x = ecran.width - SCROLL_DEFAULT_WIDTH - confirmer.champ.width - 10;
        adapter_bouton(&confirmer); // recentre le texte après le decalage

        /* centrer les champs et leurs titres */
        for (int i = 0; i < nb_joueurs; i++) {
            champs[i].champ.x = (float) (ecran.width - SCROLL_DEFAULT_WIDTH)/2 - champs[i].champ.width/2;
            titres[i].position.x = champs[i].champ.x;
            update_champsaisie_camera(&champs[i], scrollbar.vue);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(scrollbar.vue);
                dessiner_bouton(retour);
                dessiner_bouton(confirmer);

                for (int i = 0; i < nb_joueurs; i++) {
                    dessiner_champsaisie(champs[i]);
                    dessiner_texte(titres[i]);
                }
            EndMode2D();

            dessiner_scrollbar(scrollbar, ecran);
        EndDrawing();
    }
    for (int i = 0; i < nb_joueurs; i++) {
        detruire_champsaisie(champs[i]);
        free(titres[i].contenu);
    }
    free(champs);
    free(titres);

    return prochaine_page;
}
