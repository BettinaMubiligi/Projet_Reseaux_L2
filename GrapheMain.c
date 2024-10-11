#include "ArbreQuat.h"
#include "Chaine.h"
#include "Graphe.h"
#include "Reseau.h"

#include "Struct_File.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  FILE *data = fopen("00014_burma.cha", "r");
  if (data == NULL) {
    printf("Erreur lors de l'ouverture du fichier\n");
    return 1;
  }

  Chaines *C = lectureChaines(data);

  /* On reconstitue le Réseau avec la méthode ArbreQuat à partir duquel le
  graphe G va reconstituer le graphe correspondant*/
  Reseau *R = reconstitueReseauListe(C);
  Graphe *G = creerGraphe(R);
  afficherGraphe(G);

  /*On appelle la fonction reorganiseReseau pour vérifier si ceratines
  arêtes dépassent le nombre maximal gamma de fibres optiques par câbles

  printf("Valeur de retour de reorganiseReseau : %d\n",
  reorganiseReseau(R));
  La fonction affiche : 
  10  5  4 
  5 
  7  11  10 

  2  9  8  7 
  5  10  2  6 
  5  10  2  3 
  2  1 
  gamma : 3, arête(i,j): 10, 5  */
  

  // Libération de la mémoire
  libererChaines(C);
  libererReseau(R);
  libererGraphe(G);
  fclose(data);
}