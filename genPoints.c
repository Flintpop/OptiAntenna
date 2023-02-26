#include <stdio.h>
#include <stdlib.h>
#include "genPoints.h"

/*
 * Ce générateur de points et de matrice de visibilité cherche à générer une suite de points et d'informations de
 * visibilités sous forme de matrice.
 * Ces points seront disposés de manière que la matrice prenne la forme la plus carrée possible. Si aucun multiple n'est
 * trouvé, le programme s'arrête et renvoie une erreur.
 * La matrice de visibilité est générée en fonction d'un pourcentage donné en paramètre, en dur dans le code.
 * Si le pourcentage est de 80, 80% des points seront visibles et 20% non visibles.
 */
#define POURCENTAGE_VISIBLE 90

/*
 * Cette fonction affiche la liste des multiples de n_points du arr->matriceleau matrice.
 * Cette fonction est utilisée pour le débuggage.
 */
__attribute__((unused)) void print_list_multiples(modular_array const *arr) {
    for (int k = 0; k < arr->n_multiples; ++k) {
        printf("[");
        for (int l = 0; l < 2; ++l) {
            if (l == 1) {
                printf("%d", arr->matrice[k][l]);
            } else {
                printf("%d, ", arr->matrice[k][l]);
            }
        }
        printf("]\n");
    }
}

/*
 * Cette fonction vérifie s'il faut agrandir le arr->matriceleau matrice.
 * Si le nombre de multiples de n_points est égal à la taille du arr->matriceleau matrice, on agrandit
 * le arr->matriceleau matrice.
 */
void check_realloc_arr(modular_array *arr) {
    if (arr->n_multiples == arr->taille) {
        arr->taille = arr->taille + arr->taille_bloc;
        arr->matrice = (int **) realloc(arr->matrice, sizeof(int *) * arr->taille);

        for (int m = 0; m < arr->taille; ++m) {
            arr->matrice[m] = (int *) realloc(arr->matrice[m], sizeof(int) * 2);
        }
    }
}

/*
 * Initialise le type modular_array ainsi que la matrice qui va contenir les multiples de n_points.
 * La matrice fonctionne par allocation de blocs. Ici, c'est de 5 en 5 (optimisation de performances).
 */
modular_array *construct_modular_array() {
    modular_array *arr;
    arr = (struct modular_array *) malloc(sizeof(modular_array));
    arr->n_multiples = 0;
    arr->taille_bloc = 5;
    arr->taille = arr->taille_bloc;
    arr->matrice = (int **) malloc(sizeof(int *) * arr->taille);

    for (int k = 0; k < arr->taille; ++k) {
        arr->matrice[k] = (int *) malloc(sizeof(int) * 2);
    }
    return arr;
}

/*
 * Libère la mémoire allouée pour le type modular_array.
 */
void free_modular_array(modular_array *arr) {
    for (int k = 0; k < arr->taille; ++k) {
        free(arr->matrice[k]);
    }
    free(arr->matrice);
}

/*
 * Cette fonction vérifie si k et l sont des multiples de n_points et les ajoute dans la matrice.
 * Si k et l sont des multiples de n_points, on ajoute k et l dans la matrice. On vérifie ensuite s'il faut agrandir
 * la matrice.
 */
void check_multiple(int k, int l, long n_points, modular_array *arr) {
    if ((k * l) == n_points) {
        arr->matrice[arr->n_multiples][0] = k;
        arr->matrice[arr->n_multiples][1] = l;
        (arr->n_multiples)++;
        check_realloc_arr(arr);
    }
}

/*
 * Cette fonction affiche un point de la matrice de points. Si le point est le dernier de la matrice, on affiche
 * un point-virgule à la place d'une virgule.
 */
void print_point(int i, int j, int k, int l) {
    if (k == i && l == j) {
        printf("< %d, %d>\n", k, l);
        return;
    }
    printf("< %d, %d>,\n", k, l);
}

/*
 * Cette fonction affiche la matrice de points en fonction des multiples de n_points, i et j.
 */
void print_points(int i, int j) {
    printf("points = {\n");
    for (int k = 1; k < i + 1; ++k) {
        for (int l = 1; l < j + 1; ++l) {
            print_point(i, j, k, l);
        }
    }
    printf("};\n");
}

/*
 * Cette fonction cherche les multiples de n_points les plus équilibrés (pour que la matrice soit le plus carré
 * possible) pour faire la matrice de points
 */
void find_multiples(long n_points, int *i, int *j) {
    // Construction d'arr qui est un struct qui contient la matrice des multiples de n_points et diverses variables liés
    // à la matrice
    modular_array *arr = construct_modular_array();

    // On cherche les multiples de n_points les plus équilibrés pour faire la matrice de points
    for (int k = 1; k < n_points; ++k) {
        for (int l = k; l <= n_points; ++l) {
            check_multiple(k, l, n_points, arr);
        }
    }

    if (arr->n_multiples == 0) {
        fprintf(stderr, "Erreur, pas de multiples de %ld trouvés\n", n_points);
        exit(1);
    }

    // On a trouvé les multiples de n_points les plus équilibrés pour faire la matrice de points

    // i est le premier multiple choisi, le plus petit
    *i = arr->matrice[arr->n_multiples - 1][0];
    // j est le second multiple choisi, le plus grand
    *j = arr->matrice[arr->n_multiples - 1][1];

    free_modular_array(arr);
    free(arr);
}

/*
 * Donne un chiffre 0 ou 1, en fonction d'un pourcentage donné en paramètre.
 * Si le chiffre était de 80, la fonction donnerait un 1 80 fois et un 0 20 fois du temps.
 * Donc si le chiffre était de 20, le 1 serait retourné 20 fois et 0 80 fois.
 */
int get_visibility_information(int pourcentage) {
    if (pourcentage < 0 || pourcentage > 99) {
        fprintf(stderr, "Erreur de paramètre. Pourcentage n'est pas dans les bornes (0, 99) requis. "
                        "Le paramètre est : %d", pourcentage);
        return -1;
    }
    // Donne les 1 ou 0 de la table de visibilité en fonction d'une probabilité
    int res = rand() % 100;
    if (res < pourcentage) res = 1;
    else res = 0;
    return res;
}

/*
 * Affiche un point de la matrice de visibilité qui est composé de soit 0, soit 1. 0 signifie que le point n'est pas
 * visible. 1 signifie que le point est visible.
 */
void print_visibility_point(int l, long n_points, int pourcentage) {
    if (l == n_points - 1) {
        printf("%d", get_visibility_information(pourcentage));
        return;
    }

    printf("%d ", get_visibility_information(pourcentage));
}

/*
 * Affiche la matrice de visibilité. La matrice de visibilité est une matrice carrée de 0 et de 1.
 * 0 signifie que le point n'est pas visible. 1 signifie que le point est visible. Les points peuvent ne pas être
 * visibles pour eux-mêmes.
 * La matrice est carrée, donc il y a autant de lignes que de colonnes.
 */
void print_visibility_array(long n_points, int pourcentage) {
    printf("\nvisibilite = [\n");
    for (int k = 0; k < n_points; ++k) {
        printf("[ ");
        for (int l = 0; l < n_points; ++l) {
            print_visibility_point(l, n_points, pourcentage);
        }
        printf(" ]\n");
    }
    printf("];\n");
}

/*
 * Cette fonction permet de récupérer le nombre de points à générer en fonction des arguments de la ligne de commande.
 */
long get_n_points(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Erreur, il faut au moins un argument\n");
        exit(1);
    }

    long n_points;
    if ((n_points = strtol(argv[1], NULL, 10)) == 0) {
        fprintf(stderr, "Erreur, le nombre de points doit être un entier positif\n");
        exit(1);
    }

    return n_points;
}

/*
 * Cette fonction permet de récupérer le seed à utiliser en fonction des arguments de la ligne de commande.
 */
void set_seed(int argc, char** argv) {
    if (argc == 3) {
        long seed;
        if ((seed = strtol(argv[2], NULL, 10)) == 0) {
            fprintf(stderr, "Erreur, le seed doit être un entier positif\n");
            exit(1);
        }
        srand((unsigned int) seed);
    }
}

int main(int argc, char *argv[]) {
    int i, j;

    long n_points = get_n_points(argc, argv);

    set_seed(argc, argv);

    find_multiples(n_points, &i, &j);

    print_points(i, j);

    print_visibility_array(n_points, POURCENTAGE_VISIBLE);

    return 0;

}