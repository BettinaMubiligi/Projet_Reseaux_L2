// Beeverly Gourdette & Bettina Mubiligi

//Exercice 6
#include "ArbreQuat.h"
#include "Chaine.h"
#include "Graphe.h"

#include "Hachage.h"
#include "Reseau.h"

#include "time.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_APPEL 5000
#define PAS 500

int main() {
  srand(time(NULL)) ; // utile pour la génération aléatoire utilisée dans generationAleatoire

    clock_t debutListe, finListe, debutHachage, finHachage, debutArbre, finArbre;
    float tempsListe, tempsHachage, tempsArbre;

    FILE *donneesListe = fopen("tempsListe.txt", "w");
    FILE *donneesHachageArbre = fopen("tempsHachageArbre.txt", "w");

    if (donneesListe == NULL || donneesHachageArbre == NULL) {
        printf("Erreur lors de l'ouverture des fichiers\n");
        return 1;
    }

    printf("Debut Liste Chainée\n");

    Chaines *C;
    Reseau *R1;
    for (int i = PAS; i <= NB_APPEL; i += PAS) {
        C = generationAleatoire(i, 3, NB_APPEL, NB_APPEL);
        debutListe = clock();
        R1 = reconstitueReseauListe(C);
        finListe = clock();
        tempsListe = (float)(finListe - debutListe) / CLOCKS_PER_SEC;
        fprintf(donneesListe, "%d %f\n", i, tempsListe);
        fflush(donneesListe); //forcer l'écriture des données 
        libererReseau(R1);
    }

    printf("Fin Liste Chainée\n");
    printf("Debut TableHachage et ArbreQuat\n");

    
    /*Table de Hachage et ArbreQuat*/

    Reseau *R2, *R3;
    for (int i = PAS; i <= NB_APPEL; i += PAS) {
      Chaines *C = generationAleatoire(i, 3, NB_APPEL, NB_APPEL);

      debutHachage = clock();
      R2 = reconstitueReseauHachage(C, 10); //Taille de la table de Hachage = 100
      finHachage = clock();
      tempsHachage = ((double)(finHachage - debutHachage)) / CLOCKS_PER_SEC;
      fprintf(donneesHachageArbre, "%d %f ", i, tempsHachage);
      libererReseau(R2);

      debutHachage = clock();
      R2 = reconstitueReseauHachage(C, 500); //Taille de la table de Hachage = 500
      finHachage = clock();
      tempsHachage = ((double)(finHachage - debutHachage)) / CLOCKS_PER_SEC;
      fprintf(donneesHachageArbre, " %f ",tempsHachage);

      libererReseau(R2);

      debutHachage = clock();
      R2 = reconstitueReseauHachage(C, 1000); //Taille de la table de Hachage = 1500
      finHachage = clock();
      tempsHachage = (double)((finHachage - debutHachage)) / CLOCKS_PER_SEC;

      debutArbre = clock();
      R3 = reconstitueReseauArbre(C);
      finArbre = clock();
      tempsArbre = ((double)(finArbre - debutArbre)) / CLOCKS_PER_SEC;

      fprintf(donneesHachageArbre, " %f %f\n", tempsHachage, tempsArbre);

      libererReseau(R2);
      libererReseau(R3);
      libererChaines(C);
    }



    printf("Fin TableHachage et ArbreQuat\n");

    fclose(donneesListe);
    fclose(donneesHachageArbre);

    return 0;
}
/*Exercice 6.3

1. Table de Hachage et Arbre: 

set title "Temps d'execution : Table de Hachage vs ArbreQuat"
set xlabel "Nombre de chaines"
set ylabel "Temps (secondes)"
set grid 

set output 'performanceHachageArbre.png'

plot "tempsHachageArbre.txt" using 1:2 with lines lw 2 title 'Table hachage taille 100', \
     "tempsHachageArbre.txt" using 1:3 with lines lw 2 title 'Table hachage taille 500', \
     "tempsHachageArbre.txt" using 1:4 with lines lw 2 title 'Table hachage taille 1500', \
     "tempsHachageArbre.txt" using 1:5 with lines lw 2 title 'ArbreQuat'

2. Liste

set title "Temps d'exécution :Liste Chainee"
set xlabel "Nombre de chaines"
set ylabel "Temps (secondes)"
set grid
set output 'performanceListe.png'


plot "tempsListe.txt" using 1:2 with lines lw 2 title 'Liste chaînée'


*/