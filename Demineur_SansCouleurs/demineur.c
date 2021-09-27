#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "Profil.h"
#include "demineur.h"


// Constante pour colorer texte
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define GRY   "\x1B[30m"
#define RESET "\x1B[0m"

#define CONTOUR 'w'
#define BOMBE 'r'
#define DEMASQUE 'g'
#define DRAP 'c'

/*
	+-------------------------+
	|  PRIMITIVES ALLOCATION  |
	+-------------------------+
*/


/* Genere un vecteur de grille_t
* @Taille : Nombre de case de la grille
* @Bombe : Nombre de bombes
* @c : Case a protegée (1e case cliquée)
* Retourne l'adresse de la grille
*/
grille_t  GenererGrille(int Taille, int Bombe, int c){
    int i, rndm;
    int T = Taille * Taille;

    grille_t Grille = (grille_t) calloc(T, sizeof(cell_t));

        //Generation aleatoire
    rndm = rand()%T;
    for(i=0; i < Bombe; i++){
        while(c == rndm || Grille[rndm].Bombe){
            rndm = rand()%T;
        }
        Grille[rndm].Bombe = true;
    }
    return Grille;
}

// Libere l'espace dans la grille 
void LibererGrille(grille_t Grille) {
	free(Grille);
	Grille = NULL;
}

// Genere un pointeur d'entier
int * Int(){
	int * x = (int*)malloc(sizeof(int));
	return x;
}



// Libere un pointeur d'entier
void LibererInt(int * n) {
	free(n);
	n = NULL;
}

// Libere un pointeur de caracteres
void LibererChar(char * c){
	free(c);
	c = NULL;
}


/*
	+-------------------------+
	| PRIMITIVES CONSULTATION |
	+-------------------------+
*/

/* Affiche le contenu d'une grille ligne par ligne
* @Grille : Grille a afficher
* @Taille : Nombre de case de la grille
*/
void AfficherGrille(grille_t Grille, int Taille) {
	int i, j, c;

	// Entete abscisses

	printf("+---");
	
	for (i = 1; i <= Taille; i++)
	{
		printf("+--");
	}
	printf("+\n|   ");

    for(i=1; i<=Taille; i++){
		printf("|%d", i);
		if(i<10) printf(" ");
    } 
    printf("| x\n");


	printf("+---");
	for (i = 1; i <= Taille; i++)
	{
		printf("+--");
	}
	printf("+\n");

	
	// Grille
    for(i=0; i<Taille; i++)
	{

		printf("|");
		if (i < 9) printf("0");
        printf("%d |", i+1);
		

        
		for(j=0; j<Taille; j++)
		{
            c = CompterCase(Taille,j,i);

			if (Grille[c].Demasquee == false)
			{
				if (Grille[c].Etat) printf("! ");
				else printf("? ");
			}
			else
			{
                if(Grille[c].Bombe) printf("* ");
                else printf("%d ",Grille[c].Valeur);
            }
			printf("|");
        }
        printf("\n");
		
		printf("+---+");
		for (j = 1; j <= Taille; j++)
		{
			printf("--+");
		}
		printf("\n");
    }
	printf("y\n");
}

/* Affiche la solution d'une grille_t
* @Grille : Grille dont il faut la solution
* @T : Taille de la grille (longueur)
*/
void AfficherSolution(grille_t Grille, int T) {

	int i, j, c;
	printf("\n  ");
	for (i = 1; i <= T; i++) printf("%d ", i);
	
	printf(" x\n");

	for (i = 0; i < T; i++) {
		printf("%d ", i + 1);
		for (j = 0; j < T; j++) {
			c = CompterCase(T, j, i);


			if (Grille[c].Bombe) {
				printf("* "); fflush(stdout);
			}
			else {
				CompterBombes(Grille, T, j, i);
				printf("%d ", Grille[c].Valeur);
			}
		}
		printf("\n");
	}

}

/* Verifie si la partie est terminee
* @Grille : grille_t a verifier
* @T : Taille de la grille
* @Nbb : Nombre de bombes dans la Grille
* Retourne 1 (Si perdu), 2( Si gagne), 0 (Si rien a faire)
*/
int VerifierGrille(grille_t Grille, int T, int Nbb) {
	int Tt = T * T;

	int cptdrap = 0;
	int cptbombedrap = 0;
	int cpt = 0;

	for (int i = 0; i < Tt; i++)
	{
		if (Grille[i].Bombe && Grille[i].Demasquee) return 1; // 1 <=> Partie perdue

		else if (!(Grille[i].Bombe) && Grille[i].Demasquee) cpt++; // Nombre de cases sans bombes demasquées

		else if (Grille[i].Etat) {
			cptdrap++; // Nombre de cases avec drapeau
			if (Grille[i].Bombe) {
				cptbombedrap++;
			}
		}
	}
	if ((cptdrap == cptbombedrap && cptbombedrap == Nbb) || cpt == Tt - Nbb) return 2; // 2 <=> Partie gagnee

	else return 0; // 0 <=> On continue la partie
}


/* Determine la case correspondante dans une grille
* @x : abscisse de la grille
* @y : ordonnée de la grille
* @T : Taille de la grille (Longueur en 2D)
*/
int CompterCase(int T, int x, int y) {
	return T * y + x;
}


/*Compte le nombre de bombes autour d'une case
* @x : Coordonnee en abscisse
* @y : Coordonnee en ordonnee
* @Grille : Grille dans laquelle il faut verifier
* Modifie la grille aux coordonnées x y
*/
void CompterBombes(grille_t Grille, int T, int x, int y) {
	int cpt = 0;
	int x1 = 0;
	int y1 = 0;
	int i, j;

	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			// Ici, il est necessaire d'eviter la case actuelle
			// Au pire cas, de comptabiliser une bombe en trop
			if ((i != 0) || (j != 0))
			{
				x1 = x + j;
				y1 = y + i;
				if (((x1 >= 0 && x1 < T) && (y1 >= 0 && y1 < T)))
				{
					if (Grille[CompterCase(T, x1, y1)].Bombe) cpt++;
				}
			}
		}
	}
	Grille[CompterCase(T, x, y)].Valeur = cpt;
}


/*
	+-------------------------+
	| PRIMITIVES MODIFICATION |
	+-------------------------+
*/

/*Demasque la zone correspondant aux coordonnees (x,y)
* dans la tgrille Grille de la longueur T
* Si c'est la premiere fois, evite les bombes.
* @*gri : tgrille a dans laquel il faut demasquer aux coordonnees (x,y)
* @T : Longueur de grille
* @x : Abscisse ou il faut demasquer
* @y : Ordonnee ou il faut demasquer
*/
void Demasquer(grille_t Grille, int T, int x, int y){
    int i, j;

    int c = x + y * T ;
    if((x<T) && (y<T) && (x>=0) && (y>=0)){
            // Si la case n'est pas demasquee
        if(!Grille[c].Demasquee)
		{
			//On demasque
            Grille[c].Demasquee = true;
			Grille[c].Etat = false;

            CompterBombes(Grille, T, x, y);

            // Si la case est vide, on demasque toutes les cases autour
			if(Grille[c].Valeur == 0)
			{
                for(i = -1; i <= 1; i++)
				{
                    for(j = -1; j <= 1; j++)
					{
					// if(i!=0 || j!=0){
						//Il est plus rentable d'entrer une fois en trop
						//dans le demasquer plutot que de verifier 8 fois en plus
                            Demasquer(Grille, T, x + i, y+j);
                    }
                }
			}
        }
    }
}


/*
	+-------------------------+
	| PRIMITIVES Utilisateur  |
	+-------------------------+
*/


/*Demande l'Action a effectuer aux coordonnees (x,y)
* @*x : Valeur de l'abscisse
* @*y : Valeur de l'ordonnee
* @Taille : Longueur de la grille
* @Action : Action a realiser aux coordonnees x,y (ou pas)
* Les valeurs x et y seront réduites de 1 pour faciliter les calculs
* dans les accès à la grille
*/
void Demander(int *x, int *y, int Taille, int *Action) {
	*Action = 0;
	char confirm = 'a';
	do {
		while (*Action < 1 || *Action > 3) {
			printf("\n1 - Demasquer la case\n2 - Poser/Retirer un drapeau\n3 - Quitter la partie\nQue faire ?  ");
			scanf("%d", Action);
			printf("\n");
		}

		if (*Action == 1 || *Action == 2) {
			*x = 0;
			*y = 0;
			while ((Taille < *x) || (1 > *x))
			{
				printf("Saisir l'abscisse : ");
				scanf("%d", x);
			}

			while ((Taille < *y) || (1 > *y))
			{
				printf("Saisir l'ordonnee : ");
				scanf("%d", y);
			}

			(*x)--;
			(*y)--;
		}
		else if (*Action == 3) {
			printf("Etes-vous sur de vouloir quitter ?[o/n]");
			do
			{
				scanf("%c", &confirm);
			} while (confirm != 'o' && confirm != 'n');
			if (confirm == 'o')
			{
				printf("Dommage, c'etait bien parti ^^'\n\n");

			}
		}
	} while (confirm == 'n' && *Action == 3);
}
	

/* Demande la saisie de la difficulte
* @T : Dimension de la grille
* @Nbb : Nombre de bombe
* Retourne 1 s'il faut quitter
*/
int Difficultee(int * T, int * B){

	char d='F';
	printf("Saisir votre pseudo : ");
	//scanf_s("%10s", pseudo);


	while ((d != 'A') && (d != 'B') && (d != 'C') && (d != 'D') && (d != 'E')){
		printf("Niveaux :\n A : Facile [10x10 - 10 bombes]\n B : 	Moyen [10x10 - 15 bombes]\n C : Difficile [15x15 - 30 bombes]\n D : Customise\n E : Quitter la partie maintenant !!\n Saisir la difficulte :");
		scanf("%c",&d); fflush(stdin);
	}


	switch(d)
	{
		case 'A':
	        *T = 10;
			*B = 10;
			return 0;
		case 'B':
			*T = 10;
			*B = 15;
			return 0;
		case 'C':
			*T = 15;
			*B = 30;
			return 0;
		case 'D':
			do
			{
				printf("Saisir la taille de la grille [Entre 1 et 100]:");
				scanf("%d",T); fflush(stdin);
		    }while((*T>100)||(*T<1));
				
			do
			{
				printf("Saisir le nombre de bombes [Inferieur a %d]:", *T);
				scanf("%d",B); fflush(stdin);
			}while((*B>=*T)||(*B<1));

			printf("Difficultee saisie %d !\n", *T); fflush(stdout);
			return 0;
		case 'E':
			return 1;
		default:
			return 1;
}
}
