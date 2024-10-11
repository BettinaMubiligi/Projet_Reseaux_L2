ChaineMain: ChaineMain.o Chaine.o
	gcc -o ChaineMain ChaineMain.o Chaine.o -lm

Chaine.o: Chaine.c Chaine.h
	gcc -c -o Chaine.o -Wall Chaine.c

ChaineMain.o: ChaineMain.c Chaine.h
	gcc -c -o ChaineMain.o -Wall ChaineMain.c

Hachage.o: Hachage.c Hachage.h
	gcc -c -o Hachage.o -Wall Hachage.c 

ArbreQuat.o: ArbreQuat.c ArbreQuat.h
	gcc -c -o ArbreQuat.o -Wall ArbreQuat.c
	
Reseau.o: Reseau.c Reseau.h Chaine.h 
	gcc -c -o Reseau.o -Wall Reseau.c

Graphe.o: Graphe.c Graphe.h
	gcc -c -o Graphe.o -Wall Graphe.c

Struct_File.o: Struct_File.c Struct_File.h
	gcc -c -o Struct_File.o -Wall Struct_File.c
	
ReconstitueReseau: ReconstitueReseau.o Reseau.o Chaine.o Hachage.o ArbreQuat.o Graphe.o Struct_File.o
	gcc -o ReconstitueReseau ReconstitueReseau.o Reseau.o Chaine.o Hachage.o ArbreQuat.o Graphe.o Struct_File.o -lm

ReconstitueReseau.o: ReconstitueReseau.c Reseau.h Chaine.h Hachage.h ArbreQuat.h Graphe.h Struct_File.h
	gcc -c -o ReconstitueReseau.o -Wall ReconstitueReseau.c

ReconstitueReseauComp: ReconstitueReseauComp.o Reseau.o Chaine.o Hachage.o ArbreQuat.o
	gcc -o ReconstitueReseauComp ReconstitueReseauComp.o Reseau.o Chaine.o Hachage.o ArbreQuat.o  -lm

ReconstitueReseauComp.o: ReconstitueReseauComp.c Reseau.h Chaine.h Hachage.h ArbreQuat.h
	gcc -c -o ReconstitueReseauComp.o -Wall ReconstitueReseauComp.c

GrapheMain: GrapheMain.o Reseau.o Chaine.o ArbreQuat.o Graphe.o Struct_File.o
	gcc -o GrapheMain GrapheMain.o Reseau.o Chaine.o ArbreQuat.o Graphe.o Struct_File.o -lm

GrapheMain.o:  GrapheMain.c Reseau.h Chaine.h ArbreQuat.h Struct_File.h
	gcc -c -o GrapheMain.o -Wall GrapheMain.c

clean:
	rm -rf *.o 
