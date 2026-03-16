#ifndef page_h
#define page_h

/* Denomme les differentes pages constituant l'interface graphique.
 * Chaque page a une fonction associé qui modifie l'etat du jeux et renvoit
 * la prochaine page à afficher ou P_QUITTER pour arreter le jeux.
 */
enum Page {
    P_TITRE,
    P_JEUX,
    P_CUSTOM,
    P_CHARGER,
    P_REGLES,
    P_QUITTER,
};

enum Page page_titre(void);
// enum Page page_jeux(void);
// enum Page page_options(void);
enum Page page_charger(void);

#endif
