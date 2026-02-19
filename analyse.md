## Rapport d'analyse

### Analyse des besoins

Jeux Carcassonne interactif pour GNU/Linux.

### Spécifications

- Règles principales (__sans extensions__) du jeux de plateau Carcassonne.
- Un nombre fini de tuiles placés au cours d'une partie.
- Import d'un "jeux" de tuiles au format csv fournis.
- Generation aléatoire de tuiles possible.
- Sauvegarde et recharge d'une partie en cours.
- Une interface graphique.

### Conception Architecturale

#### Modèle MVC

- Modele genere et contient la grille, tuiles, points, meeples etc.
- Vue affiche l'etat du modele et recoit les entrées utilisateur.
- Controller recoit les entrées de la vue et modifie le modele en consequence.

#### Dependences

Utilisation de la librairie Raylib pour l'interface utilisateur.
