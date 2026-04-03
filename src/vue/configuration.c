#include <stdio.h>
#include "libca.h"
#include "raylib.h"
#include "jeu.h"
#include "page.h"
#include "bouton.h"
#include "champsaisie.h"
#include "scrollbar.h"

enum Page page_configuration(Jeu *jeu, bool custom)
{
    // Etat initial
    enum Page prochaine_page = P_CUSTOM;
    int largeur_ecran = GetScreenWidth();

    SetExitKey(KEY_NULL); // echape retourne à l'ecran titre au lieu de fermer la fenetre

    int nb_joueur = 4;
    int nb_meeple = 7;
    int nb_tuiles = 72;

    // Elements de la page
    Bouton retour = creer_bouton_adapte(10, 10, "<- retour");
    Bouton confirmer = creer_bouton_adapte(0, 10, "confirmer ->");

    char buffer[10];

    Texte titre_nb_joueur  = creer_texte(0, 100, "Nombre de joueurs");
    float largeur_titre_nb_joueur = mesurer_texte(titre_nb_joueur).x;
    ChampSaisie champ_nb_joueur = creer_champsaisie(0, 130, 150, 40, true);
    snprintf(buffer, 10, "%d", nb_joueur);
    ajouter_chaine(&champ_nb_joueur.saisie, buffer);

    Texte titre_nb_meeple  = creer_texte(0, 230, "Nombre de meeple");
    float largeur_titre_nb_meeple = mesurer_texte(titre_nb_meeple).x;
    ChampSaisie champ_nb_meeple = creer_champsaisie(0, 260, 150, 40, true);
    snprintf(buffer, 10, "%d", nb_meeple);
    ajouter_chaine(&champ_nb_meeple.saisie, buffer);

    Texte titre_nb_tuiles  = creer_texte(0, 360, "Nombre de tuiles");
    float largeur_titre_nb_tuiles = mesurer_texte(titre_nb_tuiles).x;
    ChampSaisie champ_nb_tuiles = creer_champsaisie(0, 390, 150, 40, true);
    snprintf(buffer, 10, "%d", nb_tuiles);
    ajouter_chaine(&champ_nb_tuiles.saisie, buffer);

    while (prochaine_page == P_CUSTOM) {
        largeur_ecran = GetScreenWidth();

        if (WindowShouldClose())
                prochaine_page = P_QUITTER;

        if (update_bouton(&retour) || IsKeyPressed(KEY_ESCAPE)) {
                detruire_jeu(*jeu);
                *jeu = (Jeu) { 0 };
                prochaine_page = P_TITRE;
        }

        if (update_bouton(&confirmer)
            && champ_nb_joueur.saisie.len != 0
            && champ_nb_meeple.saisie.len != 0
            && champ_nb_tuiles.saisie.len != 0) {

            sscanf(champ_nb_joueur.saisie.texte, "%d", &nb_joueur);
            sscanf(champ_nb_meeple.saisie.texte, "%d", &nb_meeple);
            sscanf(champ_nb_tuiles.saisie.texte, "%d", &nb_tuiles);

            if (nb_joueur != 0 && nb_meeple != 0 && nb_tuiles != 0) {
                detruire_jeu(*jeu);
                *jeu = creer_jeu(nb_joueur, nb_meeple, nb_tuiles);
                // on appelle une sous-page pour recuperer les noms de joueurs
                prochaine_page = page_joueurs(jeu->joueurs);

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

            champ_nb_tuiles.champ.x = (float) largeur_ecran/2 - champ_nb_tuiles.champ.width/2;
            titre_nb_tuiles.position.x = (float) largeur_ecran/2 - largeur_titre_nb_tuiles/2;

            update_champsaisie(&champ_nb_meeple);
            update_champsaisie(&champ_nb_tuiles);
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
                dessiner_texte(titre_nb_tuiles);
                dessiner_champsaisie(champ_nb_tuiles);
            }
        EndDrawing();
    }
    detruire_champsaisie(champ_nb_joueur);
    detruire_champsaisie(champ_nb_meeple);
    detruire_champsaisie(champ_nb_tuiles);
    SetExitKey(KEY_ESCAPE);

    return prochaine_page;
}

enum Page page_joueurs(ListeJoueurs joueurs)
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

    ChampSaisie *champs = ca_alloc(joueurs.nb_joueurs, sizeof(ChampSaisie));
    Texte *titres = ca_alloc(joueurs.nb_joueurs, sizeof(Texte));
    int fin_champs = 0;

    for (int i = 0; i < joueurs.nb_joueurs; i++) {
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

        if (update_bouton_camera(&retour, scrollbar.vue) || IsKeyPressed(KEY_ESCAPE)) {
            // on abandone la saisie, donc les noms actuellement chargés doivent etre liberés
            for (int i = 0; i < joueurs.nb_joueurs && champs[i].saisie.len != 0; i++) {
                free(joueurs.tableau[i].nom);
                joueurs.tableau[i].nom = NULL;
            }
            prochaine_page = P_CUSTOM;
        }

        /* À la confirmation, copier les noms de chaque joueur hors des champs de saisie
         * Si un champ est vide, ne rien faire. */
        if (update_bouton_camera(&confirmer, scrollbar.vue)) {
            int i;
            for (i = 0; i < joueurs.nb_joueurs && champs[i].saisie.len != 0; i++) {
                free(joueurs.tableau[i].nom);
                joueurs.tableau[i].nom = dupliquer_chaine(&champs[i].saisie);
            }

            // Si on s'est arreté prematurement, un des champs est vide
            if (i < joueurs.nb_joueurs)
                prochaine_page = P_JOUEURS;
            else
                prochaine_page = P_JEU;
        }

        /* realigner le bouton de confirmation à droite */
        confirmer.champ.x = ecran.width - SCROLL_DEFAULT_WIDTH - confirmer.champ.width - 10;
        adapter_bouton(&confirmer); // recentre le texte après le decalage

        /* centrer les champs et leurs titres */
        for (int i = 0; i < joueurs.nb_joueurs; i++) {
            champs[i].champ.x = (float) (ecran.width - SCROLL_DEFAULT_WIDTH)/2 - champs[i].champ.width/2;
            titres[i].position.x = champs[i].champ.x;
            update_champsaisie_camera(&champs[i], scrollbar.vue);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(scrollbar.vue);
                dessiner_bouton(retour);
                dessiner_bouton(confirmer);

                for (int i = 0; i < joueurs.nb_joueurs; i++) {
                    dessiner_champsaisie(champs[i]);
                    dessiner_texte(titres[i]);
                }
            EndMode2D();

            dessiner_scrollbar(scrollbar, ecran);
        EndDrawing();
    }
    for (int i = 0; i < joueurs.nb_joueurs; i++) {
        detruire_champsaisie(champs[i]);
        free(titres[i].contenu);
    }
    free(champs);
    free(titres);

    return prochaine_page;
}
