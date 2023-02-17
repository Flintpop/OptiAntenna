#include <stdio.h>
#include <stdlib.h>
#include "getPoints.h"

// genPoints <nbPoints> [<seed>]
// les points sont à la fois les points à couvrir et les points possibles pour les antennes
/* produit:
points = {
< 1, 1>,
< 1, 2>,
< 1, 3>,
< 1, 4>,
< 2, 1>,
< 2, 2>,
< 2, 3>,
< 2, 4>
};

visibilite = [
[ 1 1 1 0 0 0 0 0 ]
[ 1 1 1 0 0 0 0 0 ]
[ 1 1 0 0 0 0 1 0 ]
[ 0 1 1 0 0 0 1 0 ]
[ 1 1 1 0 0 1 0 1 ]
[ 1 1 1 0 1 0 0 0 ]
[ 1 0 1 1 0 0 0 1 ]
[ 1 1 1 0 0 0 1 0 ]
];
*/

// A utiliser pour ajouter a couv.dat les points: ./genPoints 100 3 >> couv.dat
// les antennes doivent etre saisies � part
// cf doTestsCouverture

/*
 * Cette fonction affiche la liste des multiples de n_points du arr->matriceleau matrice
 */
__attribute__((unused)) void print_list_multiples(modular_array *arr) {
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
 * Cette fonction vérifie s'il faut agrandir le arr->matriceleau matrice
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
 * Initialise le type modular_array ainsi que la matrice qui va contenir les multiples de n_points
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
 * Libère la mémoire allouée pour le type modular_array
 */
void free_modular_array(modular_array *arr) {
    for (int k = 0; k < arr->taille; ++k) {
        free(arr->matrice[k]);
    }
    free(arr->matrice);
}

/*
 * Cette fonction vérifie si k et l sont des multiples de n_points et les ajoute dans la matrice
 */
void check_multiple(int k, int l, int n_points, modular_array *arr) {
    if ((k * l) == n_points) {
        arr->matrice[arr->n_multiples][0] = k;
        arr->matrice[arr->n_multiples][1] = l;
        (arr->n_multiples)++;
        check_realloc_arr(arr);
    }
}

void print_point(int i, int j, int k, int l) {
    if (k == i && l == j) {
        printf("< %d, %d>\n", k, l);
    } else
        printf("< %d, %d>,\n", k, l);
}

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
 * Cette fonction cherche les multiples de n_points les plus équilibrés pour faire la matrice de points
 */
void find_multiples(int n_points, int *i, int *j) {
    // Construction d'arr qui est un struct qui contient la matrice des multiples de n_points et divers lié
    // à la matrice
    modular_array *arr = construct_modular_array();

    // On cherche les multiples de n_points les plus équilibrés pour faire la matrice de points
    for (int k = 1; k < n_points; ++k) {
        for (int l = k; l <= n_points; ++l) {
            check_multiple(k, l, n_points, arr);
        }
    }

    if (arr->n_multiples == 0) {

        fprintf(stderr, "Erreur, pas de multiples de n trouvés\n");
        exit(1);
    }

    // On a trouvé les multiples de n_points les plus équilibrés pour faire la matrice de points

    // i est le premier multiple choisi, le plus petit
    *i = arr->matrice[arr->n_multiples - 1][0];
    // j est le second multiple choisi, le plus grand
    *j = arr->matrice[arr->n_multiples - 1][1];

    free_modular_array(arr);
}

/*
 * Donne un chiffre 0 ou 1, en fonction d'un pourcentage donné en paramètre.
 * Si le chiffre est de 80, la fonction donnerait un 1 80 fois et un 0 20 fois du temps.
 * Donc si le chiffre est de 20, le 1 serait retourné 20 fois et 0 80 fois.
 */
int get_visibility_information(int pourcentage) {
    if (pourcentage < 0 || pourcentage > 99) {
        fprintf(stderr, "Erreur de paramètre. Pourcentage n'est pas dans les bornes (0, 99) requis. "
                        "Le paramètre est : %d", pourcentage);
        return -1;
    }
    // Donne les 1 ou 0 de la table de visibilité en fonction d'une probabilité
    // (ici 66% de chance que la fonction donne un 1)
    int res = rand() % 100;
    if (res > pourcentage) res = 1;
    else res = 0;
    return res;
}

void print_visibility_array(int n_points, int pourcentage) {
    printf("\nvisibilite = [\n");
    for (int k = 0; k < n_points; ++k) {
        printf("[");
        for (int l = 0; l < n_points; ++l) {
            printf(" %d ", get_visibility_information(pourcentage));
        }
        printf("]\n");
    }
    printf("];\n");
}

int main(int argc, char *argv[]) {
    int i, j;
    if (argc < 2) goto usage;

    int n = atoi(argv[1]);
    if (argc == 3) srand(atoi(argv[2]));

    int n_points = rand() % n;
    if (n_points <= 8) {
        n_points = 8;
    }

    find_multiples(n_points, &i, &j);

    print_points(i, j);

    print_visibility_array(n_points, 66);

    return 0;

    usage:
    fprintf(stderr, "usage: %s <nbPoints> [<seed>]\n", argv[0]);
    return 1;

}
