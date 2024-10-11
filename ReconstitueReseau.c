// Beeverly Gourdette & Bettina Mubiligi


#include "ArbreQuat.h"
#include "Chaine.h"
#include "Graphe.h"
#include "Hachage.h"
#include "Reseau.h"
#include "Struct_File.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256
/* Reconstitution du Réseau par 3 méthodes/cas : Liste, Hachage et Arbre
Quaternaire. Dans le cas 1 (liste), nous avons également effectués les tests
liés à la manipulation de graphe de l'exercice 7. */

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Usage: %s <fichier.cha> <methode>\n", argv[0]);
    printf("Methodes disponibles: 1: Liste\n 2:Table de Hachage\n 3:Arbre\n");
    return 1;
  }

  char *donnee = argv[1];
  int methode = atoi(argv[2]);

  FILE *f = fopen(donnee, "r");
  if (f == NULL) {
    printf("Erreur d'ouverture du fichier %s\n", donnee);
    return 1;
  }

  /*Retranscription des données en une Chaines C*/
  Chaines *C = lectureChaines(f);
  afficheChainesSVG(C, "burmasvg");
  fclose(f);

  switch (methode) {

  /*============ Liste ==================*/ 
  case 1: 
    printf("Méthode : Liste\n");

    //Informations générales sur la Chaines 
    printf("Longueur totale des chaines : %f  \n", longueurTotale(C));
    printf("Nombre total de points avec doublons : %d \n", comptePointsTotal(C));



    //Ecriture du Réseau dans un fichier
    Reseau *R1 = reconstitueReseauListe(C);
    if (R1 == NULL) {
      printf("Reconstruction du réseau échouée\n");
      return 1;
    }

    FILE *f1 = fopen("burmacpyLISTE.cha", "w");
    if (f1 == NULL) {
      printf("Erreur lors de l'ouverture du fichier\n");
      return 1;
    }
    ecrireReseau(R1, f1);
    fclose(f1);

    // Affichage SVG
    afficheReseauSVG(R1, "affichageReseau.txt");

    // Affichage du réseau reconstruit
    printf("Réseau reconstruit:\n");
    afficheReseau(R1);

    libererReseau(R1);
    break;

  
  /*============ Table de Hachage ==================*/ 
  case 2:
    printf("Méthode : Table de hachage\n");

    // Gestion de la taille de la table de hachage par l'utilisateur 

    char input[MAX_LENGTH];
    int m; 


    printf("Veuillez entrez la taille de la table de hachage comprise entre 10 et 500\n");
    fgets(input, sizeof(input),stdin);

    /*L'intervalle choisit nous parait raisonnable car lors de la comparaison des temps d'exécution, on remarque 
    qu'une taille 500 est rapide et donne des performances assez proches d'un tablau de taille 1500 et un tableau trop petit serait contre-productif . */


    if((sscanf(input, "%d", &m) != 1) || m<10 || m>500){
      printf("La taille doit etre comprise entre 10 et 500\n");
      break;
    }

    //Informations générales sur la Chaines 
    printf("Longueur totale des chaines : %f  \n", longueurTotale(C));
    printf("Nombre total de points avec doublons : %d \n", comptePointsTotal(C));


    //Ecriture du Réseau dans un fichier
    Reseau *R2 = reconstitueReseauHachage(C, m);
    if (R2 == NULL) {
      printf("Erreur: Reconstruction du réseau échouée\n");
      return 1;
    }

    FILE *f2 = fopen("burmacpyHACHAGE.cha", "w");
    if (f2 == NULL) {
      printf("Erreur lors de l'ouverture du fichier\n");
      return 1;
    }
    ecrireReseau(R2, f2);
    fclose(f2);


    // Affichage du réseau reconstruit
    printf("Réseau reconstruit:\n");
    afficheReseau(R2);

    //Libération mémoire
    libererReseau(R2);
    break;

  /*======== Arbre ============== */
  case 3:

    printf("Méthode : Arbre\n");

    //Informations générales sur la Chaines
    printf("Longueur totale des chaines : %f  \n", longueurTotale(C));
    printf("Nombre total de points avec doublons : %d \n", comptePointsTotal(C));

    //Ecriture du Reseau dans un fichier 

    FILE *f3 = fopen("burmacpyARBRE.cha", "w");
    if (f3 == NULL){
      printf("Erreur lors de l'ouverture du fichier\n");
      return 1;
    }

    Reseau *R3 = reconstitueReseauArbre(C);
    if (R3 == NULL) {
      printf("Erreur: Reconstruction du réseau échouée\n");
      return 1;
    }
    ecrireReseau(R3, f3);
    fclose(f3);

    // Affichage du réseau reconstruit
    printf("Réseau reconstruit:\n");
    afficheReseau(R3);

    // Libération de la mémoire
    libererReseau(R3);

    break;
  default:
    printf("Méthode inconnue. Veuillez utiliser 1, 2 ou 3.\n");
    return 1;
  }

  libererChaines(C);

  return 0;
}
