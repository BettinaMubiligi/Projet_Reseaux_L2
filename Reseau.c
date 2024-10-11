// Beeverly Gourdette & Bettina Mubiligi

#include "Reseau.h"
#include "Chaine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

/*===================Fonction de création des Structures=========================*/

/*Allocation mémoire et inialisation de la structure Noeud. Renvoie le Noeud crée*/
Noeud* creeNoeud(int num, double x, double y) {
    Noeud* N = (Noeud*) malloc(sizeof(Noeud));
    if(N == NULL){
        printf("Erreur lors de l'allocation mémoire du Noeud \n");
        return NULL;
    }
    N->num = num;
    N->x = x;
    N->y = y;
    N->voisins = NULL;
    return N;
}

/*--------------------------------------------------------------*/ 

/*Allocation mémoire et inialisation de la structure CellNoeud. Renvoie le CellNoeud crée*/
CellNoeud * creerCellNoeud(Noeud * N){

     if (N == NULL){
        printf("Noeud non alloué\n");
        return NULL;
    }
    CellNoeud * c = (CellNoeud *) malloc(sizeof(CellNoeud));
    if(c == NULL){
        printf("Erreur lors de l'allocation mémoire du CellNoeud\n");
        return NULL;
    }

    c->nd = N;
    c->suiv = NULL;
    return c;
}

/*--------------------------------------------------------------*/ 


/*Allocation mémoire et inialisation de la structure Reseau. Renvoie le Reseau crée*/
Reseau * creerReseau (int gamma){
    Reseau* R = (Reseau*) malloc(sizeof(Reseau));
    if(R == NULL){
        printf("Erreur lors de l'allocation mémoire du Reseau\n");
        return NULL;
    }

    R->nbNoeuds = 0;
    R->gamma = gamma;
    R->noeuds = NULL;
    R->commodites = NULL;

    return R;

}

/*--------------------------------------------------------------*/ 

/*Allocation mémoire et initialisation de la structure CellCommodité. Renvoie la CellCommodite crée*/
CellCommodite * creerCellCommodites(Noeud* A, Noeud *B){

  if (A == NULL){ 
    printf("Noeud A non alloué\n");
    return NULL;
  }
  if (B == NULL){
    printf("Noeud B no alloué\n");
    return NULL;
  }
  
  CellCommodite *co = (CellCommodite *)malloc(sizeof(CellCommodite));
  if (co == NULL){
    printf("Erreur lors de l'allocation mémoire de CellCommodite\n");
    return NULL;
  }
  co->extrA = A;
  co->extrB = B;
  co->suiv = NULL;
  return co;
}

/*===============================================================================*/

/*============== Outils de mise en relations Chaines et Reseau =================*/


/*Recherche un noeud dans le Reseau R avec les coordonnées x et y. Renvoie si trouvé, sinon crée le Noeud.*/
Noeud *rechercheCreeNoeudListe(Reseau *R, double x, double y) {
  CellNoeud *n = R->noeuds;
    /*Recherche d'un Noeud avec les coordonnées x et y dans les CellNoeuds de R*/

    while (n != NULL) {
      if (n->nd->x == x && n->nd->y == y) {
        return n->nd;
      }
      n = n->suiv;
    }

    /*Cas où aucun Noeud existant avec les coordonnées x et y : Création du Noeud*/
    Noeud * N = creeNoeud((R->nbNoeuds+1),x,y);
    if (N == NULL){
      printf("Erreur: rechercheCreeNoeudListe\n");
      return NULL;
    }

    CellNoeud *c = creerCellNoeud(N);
    if (c == NULL){
      printf("Erreur: rechercheCreeNoeudListe\n");
      free(N);
      return NULL;
    }

    c->suiv = R->noeuds;
    R->noeuds = c;

    R->nbNoeuds++;
  return N;
}

/*--------------------------------------------------------------*/ 


/*Crée un réseau à partie d'une Chaines C*/
Reseau* reconstitueReseauListe(Chaines *C) {

  if (C == NULL) {
    printf("Liste de Chaines non alloué\n");
    return NULL;
  }

  Reseau *R = creerReseau(C->gamma);
  if (R == NULL){
    printf("Erreur : reconstitueReseauListe\n");
    return NULL;
  }

  /*Création des Noeuds et mise à jour des voisins*/
  CellChaine *c = C->chaines;
  while (c != NULL) {
    CellPoint *p = c->points;
    Noeud *prec = NULL;
    while (p != NULL) {
      Noeud *N = rechercheCreeNoeudListe(R, p->x, p->y);
      if (N == NULL){
        printf("Erreur : ReconstitueReseauListe\n");
        libererReseau(R);
        return NULL;
      }
      if (prec != NULL) {
        fusionVoisins(N, prec);
      }
      prec = N;
      p = p->suiv;
    }
    c = c->suiv;
  }

  /*Ajout des Commodités au Réseau R*/
  addCommodite(R, C);

  return R;
}

/*--------------------------------------------------------------*/ 


/*Fonction pour vérifier si N2 est un voisin de N1*/
int InVoisins(Noeud *N1, Noeud *N2) {
  CellNoeud *N = N1->voisins;
  while (N != NULL) {
    if (N->nd->x == N2->x && N->nd->y == N2->y) {
      return 1;
    }
    N = N->suiv;
  }
  return 0;
}

/*--------------------------------------------------------------*/ 

//Fonction pour éliminer les doublons dans les voisins + si N1 est voisin de N2, alors N2 est voisin de N1
void fusionVoisins(Noeud *N1, Noeud *N2) {

    /* Vérifier si N2 est voisin de N1. Sinon, ajout à la liste des voisins de N1 */
    if (InVoisins(N1, N2) == 0) {
        CellNoeud *N = creerCellNoeud(N2);  
        if(N == NULL){
          printf("Erreur: fusionVoisins\n");
          return;
        }
        N->suiv = N1->voisins;
        N1->voisins = N;
    }
    
    /* Vérifier si N1 est voisin de N2. Sinon, ajout a la liste des voisins de N2*/
    if (InVoisins(N2, N1) == 0) {
        CellNoeud *N = creerCellNoeud(N1);
        if (N == NULL){
          printf("Erreur : fusionVoisins\n");
          return;
        }
        N->suiv = N2->voisins;
        N2->voisins = N;
    }
}

/*--------------------------------------------------------------*/ 

/*Créer les commodités du Reseau R à partir de la Chaines C*/
void addCommodite(Reseau *R, Chaines *C) {
  CellChaine *chaines = C->chaines;
  while (chaines != NULL) {
    CellPoint *first = chaines->points;
    CellPoint *last = chaines->points;
    while (last->suiv != NULL) {
      last = last->suiv;
    }
    Noeud *extrA = rechercheCreeNoeudListe(R, first->x, first->y);
    Noeud *extrB = rechercheCreeNoeudListe(R, last->x, last->y);

    if (extrA == NULL || extrB == NULL){
      printf("Erreur: addCommodite\n");
      free(extrA);
      free(extrB);
      return;
    }

    CellCommodite *co = creerCellCommodites(extrA,extrB);
    if (co == NULL){
      printf("Erreur : addCommodite\n");
      free(extrA);
      free(extrB);
      return;
    }
    co->suiv = R->commodites;
    R->commodites = co;
    chaines= chaines->suiv;
  }
}

/*=====================================================================================*/


/*===================== Outils de Manipulations d'un Reseau ===========================*/

/*Fonction d'affichage des commodités du Reseau R*/
void printCommodite(Reseau *R) {
  if (R == NULL){
    printf("Reseau non alloué\n");
    return;
  }
  CellCommodite *co = R->commodites;
  while (co != NULL) {
    printf("Commodité : %d - %d\n", co->extrA->num, co->extrB->num);
    co = co->suiv;
  }
}

/*--------------------------------------------------------------*/ 


/*Fonction d'affichage du Reseau R*/
void afficheReseau(Reseau *R) {
  if (R == NULL) {
    printf("Le réseau est vide.\n");
    return;
  }
  if (R->noeuds == NULL) {
    printf("La liste de noeuds du réseau est vide.\n");
    return;
  }

  printf("Nombre de noeuds: %d\n", R->nbNoeuds);
  printf("Nombre maximal de fibres par câble: %d\n", R->gamma);

  printf("Noeuds du réseau:\n");
  CellNoeud *c = R->noeuds;
  while (c != NULL) {
    printf("Noeud %d : (%.2f, %.2f)\n", c->nd->num, c->nd->x,
           c->nd->y);
    printf("Voisins: \n");
    CellNoeud *voisin = c->nd->voisins;

    while (voisin != NULL) {
      if (voisin == NULL) {
        printf("La liste des voisins du noeud est vide.\n");
      }
      printf("%d : (%.2f %.2f)\n", voisin->nd->num, voisin->nd->x, voisin->nd->y);
      voisin = voisin->suiv;
      printf("\n");

    }
    printf("\n");
    c = c->suiv;
  }
  printCommodite(R);
}

/*--------------------------------------------------------------*/ 


/*Renvoie le nombre de Chaines dans R*/
int nbCommodites(Reseau *R)
{

    if (R == NULL){
        printf("Reseau non alloué\n");
        return -1;
    }
    int total = 0;
    CellCommodite *co = R->commodites;
    while (co != NULL)
    {
        total++;
        co = co->suiv;
    }
    return total;
}

/*--------------------------------------------------------------*/ 


/*Renvoie le nombre de liaisons dans le Reseau R sans doublons*/
int nbLiaisons(Reseau *R){

  if (R == NULL){
    printf("Reseau non alloué\n");
    return -1;
  }

  int total = 0;
  CellNoeud *c = R->noeuds;

  while (c != NULL){
    total += compteVoisins(c->nd->voisins);
    c = c->suiv;
  }
  return total / 2 ;
}

/*--------------------------------------------------------------*/ 

/*Compte le nombre de voisins */
int compteVoisins(CellNoeud *voisins)
{

    int total = 0;
    CellNoeud *c = voisins;
    while (c != NULL)
    {
        total++;
        c = c->suiv;
    }
    return total;
}


/*=====================================================================================*/
/*=============== Outils de manipulation des fichiers avec Reseau =====================*/



void ecrireReseau(Reseau *R, FILE *f)
{

    /*Ecriture de l'entête du fichier avec les informations générales sur le reséau R*/
    fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R));
    fprintf(f, "NbCommodites: %d\n", nbCommodites(R));
    fprintf(f, "Gamma: %d\n", R->gamma);

    /*Ecriture des Noeuds*/

    CellNoeud *c = R->noeuds;
    Noeud *N;
    while (c != NULL)
    {
        N = c->nd;
        fprintf(f,"v %d %f %f\n", N->num, N->x, N->y);
        c=c->suiv;
    }
    fprintf(f,"\n");

    //Ecriture des Liaisons/Voisins
    c = R->noeuds;
    while (c != NULL)
    {
        CellNoeud *voisin = c->nd->voisins;
        while (voisin != NULL)
        {
            if (voisin->nd->num < c->nd->num)
                fprintf(f, "l %d %d\n", c->nd->num, voisin->nd->num);
            voisin = voisin->suiv;
        }
        c = c->suiv;
    }
    fprintf(f,"\n");

    /*Ecriture des commodites*/
    CellCommodite *co = R->commodites;
    while (co != NULL)
    {
        fprintf(f, "k %d %d\n", co->extrA->num, co->extrB->num);
        co = co->suiv;
    }
}

/*--------------------------------------------------------------*/ 


void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}


/*=====================================================================================*/
/*=================== Fonctions de libération de la mémoire ===========================*/

/*Libération mémoire d'un CellNoeud*/
void libererCellNoeud(CellNoeud *c){
    if( c== NULL) return;

    c->nd = NULL;
    free(c);
}

/*Libération mémoire d'un Noeud*/
void libererNoeud(Noeud* N){
    if(N == NULL) return;

    CellNoeud *del;
    CellNoeud *c = N->voisins;
    while(c != NULL){
        del = c;
        c = c->suiv;
        libererCellNoeud(del);
    }
    free(N);
}

void libererCellCommodite(CellCommodite *co){
    if(co ==NULL) return;

    co->extrA = NULL;
    co->extrB = NULL;

    free(co);
}

/*--------------------------------------------------------------*/

/*Libération mémoire d'un Réseau*/
void libererReseau(Reseau *R){
    if(R == NULL)return;


    CellNoeud *delC; 
    CellNoeud *c = R->noeuds;
    while(c != NULL){
        delC = c;
        c = c->suiv;
        libererNoeud(delC->nd);
        libererCellNoeud(delC);
    }

    
    CellCommodite *delCo;
    CellCommodite *co = R->commodites;
    while(co != NULL){
        delCo = co;
        co = co->suiv;
        libererCellCommodite(delCo);
    }

    free(R);
}
