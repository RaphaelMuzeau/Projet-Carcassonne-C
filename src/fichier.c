#include <stdio.h>
#include <stdbool.h>
#include "fichier.h"
#include "tuile.h"
#include "vec.h"
// TODO: Décider d'une version de format à écrire en haut du fichier.

int ecrire_grille(Vec2D *g, int x, int y, FILE *f)
{
    Tuile cur = get(*g, x, y);
    // printf("première tuile passée !\n");

    if (!cur)
        return 0;

    if (cur->is_verified) {
        return 0;
    }

    struct _Tuile tmp = *cur;
    cur->is_verified = 1;

    if (fwrite(&tmp, sizeof(struct _Tuile), 1, f) != 1)
        return 0;

    // printf("\tTuile écrite\n");
    if (fwrite(&x, sizeof(int), 1, f) != 1)
        return 0;
    // printf("\tCoordonnée x écrite\n");
    if (fwrite(&y, sizeof(int), 1, f) != 1)
        return 0;
    // printf("\tCoordonnée y écrite\n");

    return ecrire_grille(g, x+1, y, f) + ecrire_grille(g, x-1, y, f) + // EST & OUEST
           ecrire_grille(g, x, y+1, f) + ecrire_grille(g, x, y-1, f) + 1; // NORD & SUD
}

bool sauvegarder_grille(Vec2D *g, FILE *f)
{
    fpos_t pos_nb_tuiles;     // Sauvegarde là où le nb_tuiles sera écrit
    fgetpos(f, &pos_nb_tuiles);

    fseek(f, sizeof(int), SEEK_CUR); // Prépare de la place pour le nb_tuiles

    int nb_tuiles = ecrire_grille(g, 0, 0, f);

    fsetpos(f, &pos_nb_tuiles);  // Retour arrière pour écrire le nb_tuiles
    fwrite(&nb_tuiles, sizeof(int), 1, f);
    fseek(f, 1, SEEK_END);      // Retourne à la fin du fichier

    return true;
}

bool charger_grille(Vec2D *g, FILE *f)
{

    int x = 0, y = 0, nb_tuiles;

    if (fread(&nb_tuiles, sizeof(int), 1, f) != 1)
        return false;

    // printf("%d\n", nb_tuiles);
    for (int i = 0; i < nb_tuiles; i++) {
        Tuile tmp = creer_tuile();
        if (fread(tmp, sizeof(struct _Tuile), 1, f) != 1)
            return false;
        // printf("%d %d %d %d %d\n", tmp->sud, tmp->nord, tmp->est, tmp->ouest, tmp->milieu);

        if (fread(&x, sizeof(int), 1, f) != 1)
            return false;

        // printf("%d\n", x);

        if (fread(&y, sizeof(int), 1, f) != 1)
            return false;

        // printf("%d\n", y);

        set(g, tmp, x, y);
    }

    return true;
}
