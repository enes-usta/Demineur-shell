/*
Projet DEMINEUR - Programmation nécessaire

Nom : USTA
Prenom : Enes
Groupe : TP2
Année : 2018 - 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "Profil.h"
#include "demineur.h"

#define FICHIER "Joueurs.txt"

int main(void){

	srand(time(NULL));
	TProfil Profil;
	int T, B, c;
    int fin = 0;

	int *Taille = Int();
	int *Bombe = Int();

	int *x = Int();
	int *y = Int();
	int *Action = Int();

	char * pseudo = (char*)malloc(10 * sizeof(char));


/*
		+---------------------------+
		|	DEMARRAGE DE LA PARTIE	|
		+---------------------------+
*/
		// Demande a saisir la difficultee
	if (Difficultee(Taille, Bombe)) return EXIT_SUCCESS;

		// On deplace les pointeurs dans des variables normales
    T = *Taille; LibererInt(Taille);
	B = *Bombe; LibererInt(Bombe);


		// Afficher une grille de départ
	grille_t Grille = GenererGrille(T, 0, 0);
	AfficherGrille(Grille, T);
	LibererGrille(Grille);

		// Temps de départ
	time_t debut; time(&debut);

	 
	Demander(x, y, T, Action);
	if (*Action == 3) return EXIT_SUCCESS;
	c = CompterCase(T, *x, *y); // Numero de la case dans le vecteur

	Grille = GenererGrille(T, B, c); // Génération de la grille
	Demasquer(Grille, T, *x, *y);

/*
		+---------------------------+
		|	   PARTIE EN COURS      |
		+---------------------------+
*/

    while(fin == 0)
	{
		AfficherSolution(Grille, T);
		AfficherGrille(Grille, T);

		Demander(x, y, T, Action);

		c = CompterCase(T, *x, *y); // Numero de la case dans le vecteur

		switch (*Action)
		{
			case 1: // Action == Demasquer
				Demasquer(Grille, T, *x, *y);
				break;

			case 2: // Sinon c'est qu'on met/retire un drapeau aux coordonnees
				if (Grille[c].Etat == true) {
					Grille[c].Etat = false;
				}
				else {
					Grille[c].Etat = true;
				}
				break;

			case 3: // Quitter
				return EXIT_SUCCESS;
				break;

			default:
				printf("Il est prie de saisir un nombre valide !\n");
				break;
		}
		fin = VerifierGrille(Grille, T, B);
    }


/*
		+---------------------------+
		|	   FIN DE LA PARTIE     |
		+---------------------------+
*/
	for(c=0; c<=(T*T);c++) Grille[c].Demasquee = true;
	AfficherGrille(Grille, T);

	if (fin == 1)
	{
		printf("PERDUUUUU !!!!\n\n");
	}
	else
	{
		printf("GAGNEEEEE !!!!\n\n");

			// Temps de fin
		time_t fin; time(&fin);
		
		c = (int)difftime(fin, debut); // score stocké dans c (on recycle la var)

		printf("Partie gagnée : %d points\n Saisissez votre pseudonyme : ", c);
		scanf("%s", pseudo);

		if (ExistProfil(FICHIER, pseudo))
		{
			printf("\nProfil existant : Mise a jour de votre score !\n");
			Profil = CreerProfil(pseudo, c);
			MAJScoreProfil(FICHIER, Profil);
		}
		else
		{
			EnregistrerProfil(FICHIER, CreerProfil(pseudo, c));
			printf("\nProfil inexistant : Votre profil a été créé !\n");
		}	
	}



	// On libere les pointeurs
	LibererInt(Action);
	LibererInt(x);
	LibererInt(y);
	LibererChar(pseudo);
	LibererGrille(Grille);

    return EXIT_SUCCESS;
}
