// Beeverly Gourdette & Bettina Mubiligi

#include "Chaine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SVGwriter.h"
#include "SVGwriter.c"
#include <math.h>
#include "time.h"

#define MAX_LENGTH 256

/*=======Fonction de manipulations des fichiers avec Chaines================*/

/*Crée une Chaines à partir d'un fichier f*/
Chaines* lectureChaines(FILE *f){

    if (f == NULL){
        printf("%s\n", "Erreur lors de l'ouverture du fichier \n");
        return NULL;
    }

    char buffer[MAX_LENGTH], line[MAX_LENGTH];
    double x,y; 
    int numero, nbPoints, gamma, nbChaines;
    CellChaine * chaines = NULL; 

    /*Récupération dans le fichier de nbChaines et Gamma*/
    fgets(buffer, MAX_LENGTH, f);
    sscanf(buffer, "NbChain: %d", &nbChaines);
    fgets(buffer, MAX_LENGTH, f);
    sscanf(buffer, "Gamma: %d", &gamma);

    Chaines *C = creerChaines(gamma,nbChaines);
    if (C == NULL){
        printf("Erreur lors de la création de la Chaines \n");
        return NULL;
    }

    while(fgets(buffer,MAX_LENGTH,f)){

        sscanf(buffer,"%d %d %[^\n]", &numero, &nbPoints, line);

        CellChaine *c = creerCellChaine(numero);
        if(c == NULL){
            printf("Erreur lectureChaine \n");
            libererChaines(C);
            return NULL;
        }

        for (int i = 0 ;  i < nbPoints ; i++){

            sscanf(line,"%lf %lf %[^\n]", &x, &y, line);
            CellPoint * point = insertCellPoint(c,x,y);

            if(point == NULL){
                printf("Erreur lectureChaine\n");
                libererChaines(C);
                return NULL;
            }
        }
        
        c->suiv = chaines;
        chaines = c;
        
    }

    C->chaines = chaines;
    return C;
}


/*--------------------------------------------------------------*/

/*Ecrit une Chaine dans un fichier f*/
void ecrireChaines ( Chaines *C , FILE * f ){
    if (f == NULL){
        printf("%s\n", "Erreur lors de l'ouverture du fichier");
        return;
    }
    
    char buffer[256] = "";
    char res[256] = "";
    int nbPoints = 0;

    fprintf(f,"NbChain : %d\n", C->nbChaines);
    fprintf(f,"Gamma : %d\n",C->gamma);

    CellChaine *c = C->chaines;

    while (c != NULL){
        fprintf(f, "%d", c->numero);
        CellPoint * p = c->points;
        nbPoints = 0;
        res[0]='\0';
        while(p != NULL){
            sprintf(buffer,"%.2f %.2f ", p->x, p->y);
            strcat(res,buffer);
            nbPoints++;
            p = p->suiv;
        }
        fprintf(f," %d %s \n", nbPoints, res);
        c = c->suiv;
    }
}



/*--------------------------------------------------------------*/


void afficheChainesSVG(Chaines *C, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

/*=======================================================================*/

/*================ Fonction de création des Structures ==================*/

/*Fonction allouant l'espace requis pour une Chaines*/
Chaines * creerChaines(int gamma, int nbChaines){
    Chaines * C = (Chaines *) malloc(sizeof(Chaines));
    if (C == NULL){
        printf("Erreur lors de l'allocation mémoire de la Chaines\n");
        return NULL;
    }
    C->nbChaines = nbChaines;
    C->gamma = gamma;
    C->chaines = NULL;
    return C;
}

/*--------------------------------------------------------------*/

/*Fonction allouant l'espace requis pour une CellChaine*/
CellChaine * creerCellChaine(int numero){
    CellChaine * c = (CellChaine *) malloc(sizeof(CellChaine));
    if (c == NULL){
        printf("Erreur lors de l'allocation mémoire du CellChaine\n");
        return NULL;
    }
    c->numero = numero;
    c->points = NULL;
    c->suiv = NULL;
    return c;
}

/*--------------------------------------------------------------*/

CellPoint * insertCellPoint(CellChaine * c, double x, double y){
    if (c == NULL){
        printf("Erreur : CellChaine non alloué. Pas de création de points possibles\n");
        return NULL;
    }

    CellPoint * p = (CellPoint * ) (malloc(sizeof(CellPoint)));
    p->x = x;
    p->y = y;
    p->suiv = c->points;
    c->points = p;
    return p;
}

/*=======================================================================*/


/*============Fonction de libération des Structures======================*/

/*Libération de la mémoire d'une Chaines et des CellChaine contenues*/
void libererChaines(Chaines *C){
    CellChaine *c = C->chaines;
    CellChaine *del;
    while(c != NULL){
        del = c;
        c = c->suiv;
        libererCellChaine(del);
    }
    free(C);
}

/*--------------------------------------------------------------*/

/*Libération de la mémoire d'une CellChaine et des CellPoints contenus*/
void libererCellChaine(CellChaine *c){
    CellPoint *p = c->points;
    CellPoint *del;

    while(p != NULL){
        del = p;
        p = p->suiv;
        free(del);
    }
    free(c);
}

/*=======================================================================*/

/*============ Outils de Manipulation pour les Structures================*/

/*Affiche les données de la Chaines C*/
void affichage(Chaines *C){
    CellChaine * c = C->chaines;
    while(c != NULL){
        printf("Chaine %d : ",c->numero);
        CellPoint * p = c->points;
        while(p != NULL){
            printf("Chaine %d : ",c->numero);
            printf("%f %f ", p->x,p->y);
            p = p->suiv;
        }
        c = c->suiv;
        printf("\n");
    }
}

/*--------------------------------------------------------------*/

/*Renvoie la longueur de la CellChaine c*/
double longueurChaine(CellChaine *c){
    double somme = 0;
    CellPoint * p = c->points;
    while(p->suiv!= NULL){
        somme+= sqrt( pow((p->suiv->x - p->x),2) + pow((p->suiv->y - p->y),2));
        p = p->suiv;
    }
    return somme;
}

/*--------------------------------------------------------------*/

/*Renvoie la longueur totale de toutes les chaines de Chaines C*/
double longueurTotale(Chaines *C){
    double somme = 0;
    CellChaine * c = C->chaines;
    while(c!= NULL){
        somme+= longueurChaine(c);
        c = c->suiv;
    }
    return somme;
} 

/*--------------------------------------------------------------*/

/*Compte le nombre de points total dans la Chaines C */
int comptePointsTotal(Chaines *C){
    int total=0;
    CellChaine *c = C->chaines;
    while(c!= NULL){
        CellPoint * p = c->points;
        while(p != NULL){
            total++;
            p = p->suiv;
        }
        c= c->suiv;
    }
    return total;
}
/*--------------------------------------------------------------*/
// Exercice 6.2 : Fonction de génération aléatoire de Chaines
Chaines *generationAleatoire(int nbChaines, int nbPointsChaine, int xmax,
                             int ymax) {

  srand(time(NULL));

  /* Allocation de la structure Chaines et création  */
  Chaines *chaines = (Chaines *)malloc(sizeof(Chaines));
  chaines->nbChaines = nbChaines;
  chaines->gamma = 3;
  chaines->chaines = NULL;

  for (int i = 0; i < nbChaines; i++) {
    CellChaine *c = (CellChaine *)malloc(sizeof(CellChaine));
    c->numero = i + 1;
    c->points = NULL;
    c->suiv = chaines->chaines;
    chaines->chaines = c;

    /*Création des points pour la chaîne*/
    for (int j = 0; j < nbPointsChaine; j++) {
      CellPoint *p = (CellPoint *)malloc(sizeof(CellPoint));
      p->x = (double)rand() / RAND_MAX * xmax;
      p->y = (double)rand() / RAND_MAX * ymax;
      p->suiv = c->points;
      c->points = p;
    }
  }

  return chaines;
}
