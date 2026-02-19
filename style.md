### Style du projet :

Nous nous basons sur le style du [Kernel Linux](https://www.kernel.org/doc/html/v4.10/process/coding-style.html) à l'exception de quelques règles :

- L'indentation est à **4** espaces.
- Les structures sont en UpperCamelCase et les `typedef` sont encouragés avec les noms anonymes préfixés d'un `_`. <br>
**ex :**
```C
typedef struct _ExType {
    int x;
    int y;
} ExType;
```
- Tout le reste s'écrit en snake_case (variables, fonctions, etc.)
- Les gardes des fichiers header sont de la forme `nomdefichier_h` (snake_case).
