#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

int main(int argc, char *argv[])
{

    int i, j;
    if (argc < 2) goto usage;

    int n = atoi(argv[1]);
    if (argc == 3) srand(atoi(argv[2]));

    int n_points = rand() % n;
    if (n_points  <= 8) {
        n_points = 8;
    }
    printf("n_points = %d\n", n_points);

    // I have to write n lines of the following pattern in the printf function.
    // The last line must not be with ','
    // n, n changes from 1, sqrt(N) and 1, sqrt(N)
    // It is 2 for loops
    int lignes = (int) sqrt(n);
    if (lignes * lignes != n) {
        fprintf(stderr, "Erreur, pas égal a lignes * lignes : %d et %d\n", lignes, lignes * lignes);
        exit(1);
    }

    int n_multiples = 0;
    int taille_bloc = 5;
    int taille = taille_bloc;
    int ** tab = (int **) malloc(sizeof(int *) * taille);
    for (int k = 0; k < taille; ++k) {
        tab[k] = (int *) malloc(sizeof(int) * 2);
    }

    for (int k = 1; k < n_points; ++k) {
        for (int l = k; l <=  n_points; ++l) {
            if ((k * l) == n_points) {
                tab[n_multiples][0] = k;
                tab[n_multiples][1] = l;
                n_multiples++;
                if (n_multiples == taille) {
                    taille = taille + taille_bloc;
                    tab = (int **) realloc(tab, sizeof(int *) * taille);
                    for (int m = 0; m < taille; ++m) {
                        tab[m] = (int *) realloc(tab[m], sizeof(int) * 2);
                    }
                }
            }
        }
    }
    if (n_multiples == 0) {
        fprintf(stderr, "Erreur, pas de multiples de n trouvés\n");
        exit(1);
    }

//    for (int k = 0; k < n_multiples; ++k) {
//        printf("[");
//        for (int l = 0; l < 2; ++l) {
//            if(l == 1) {
//                printf("%d", tab[k][l]);
//            } else {
//                printf("%d, ", tab[k][l]);
//            }
//        }
//        printf("]\n");
//    }
    i = tab[n_multiples - 1][0];
    j = tab[n_multiples - 1][1];
    printf("points = {\n");
    for (int k = 1; k < i+1; ++k) {
        for (int l = 1; l < j+1; ++l) {
            if (k == i && l == j) {
                printf("< %d, %d>\n", k, l);
            } else
                printf("< %d, %d>,\n", k, l);
        }
    }
    printf("};\n");

    printf("\nvisibilite = [\n");
    for (int k = 0; k < n_points; ++k) {
        
    }
    // A vous
    // generer une matrice de visibilite
    printf("];\n");


    return 0;
    usage:
    fprintf(stderr, "usage: %s <nbPoints> [<seed>]\n", argv[0]);
    return 1;


}
