// Beeverly Gourdette & Bettina Mubiligi

#include "Chaine.h"
#include <string.h>
#include <stdio.h>
#include "SVGwriter.h"
#include <math.h>





int main(){

    FILE *donnee = fopen("00014_burma.cha","r");
    Chaines* A = lectureChaines(donnee);
    fclose(donnee);

    affichage(A);
    FILE *f = fopen("burmacpy.cha","w");
    ecrireChaines(A,f);
    afficheChainesSVG(A,"burmasvg");
    printf("%f \n", longueurTotale(A));
    printf("%d \n",comptePointsTotal(A));
    libererChaines(A);
    fclose(f);

    return 0;
}
