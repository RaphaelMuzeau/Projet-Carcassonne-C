#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "libca.h"
#include "listeparties.h"
#include "bouton.h"

ListeParties creer_listeparties(float y)
{
    ListeParties parties = { 0 };
    parties.fichiers = LoadDirectoryFiles("data/parties");
    parties.boutons  = ca_alloc(parties.fichiers.count, sizeof(Bouton));

    for (unsigned int i = 0; i < parties.fichiers.count; i++) {
        // GetFileNameWithoutExt utilise une meme chaine statique ecrasé à chaque appel,
        // on doit donc faire une copie avant de la mettre dans un bouton.
        const char *nom = GetFileNameWithoutExt(parties.fichiers.paths[i]);
        char *copy = ca_alloc(strlen(nom) + 1, sizeof(char));
        strcpy(copy, nom);

        // creer un bouton par partie
        parties.boutons[i] = creer_bouton_adapte(0, y, copy);
        y += parties.boutons[i].champ.height + LISTEPARTIES_SPACING;
    }
    parties.fin_liste = y;

    return parties;
}

void detruire_listeparties(ListeParties parties)
{
    for (unsigned int i = 0; i < parties.fichiers.count; i++) {
        free(parties.boutons[i].texte.contenu);
    }
    free(parties.boutons);
    UnloadDirectoryFiles(parties.fichiers);
}

char *update_listeparties(ListeParties parties, Camera2D camera)
{
    int largeur_ecran = GetScreenWidth();
    for (unsigned int i = 0; i < parties.fichiers.count; i++) {
        parties.boutons[i].champ.width = largeur_ecran * 0.8f;
        parties.boutons[i].champ.x     = largeur_ecran * 0.1f;
        centrer_texte(&parties.boutons[i]);

        // on peut se permettre de sortir immediatement car on va quitter la page
        if (update_bouton_camera(&parties.boutons[i], camera))
            return parties.fichiers.paths[i];
    }
    return NULL;
}

void dessiner_listeparties(ListeParties parties)
{
    for (unsigned int i = 0; i < parties.fichiers.count; i++)
        dessiner_bouton(parties.boutons[i]);
}
