#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "Profil.h"

// #define FICHIER "Joueurs.txt" : A définir dans le Profil.h

/* Creer un TProfil avec un pseudo et un son meilleur score
* @pseudo : Pseudonyme du joueur
* @score : Meilleur score du joueur
* Retourne le profil correspondant aux informations ci-dessus
*/
TProfil CreerProfil(char * pseudo, int score)
{
	TProfil Profil;
	Profil.BestScore = score;
	Profil.pseudo = pseudo;
	
	return Profil;
}


void EnregistrerProfil(char fichier[TAILLE_CH], TProfil Profil)
{
	FILE * file = fopen(fichier, "a+");

	if (file != NULL) fprintf(file, "%s %d\n", Profil.pseudo, Profil.BestScore);
	else printf("Fichier inexistant !!!");
	
	fclose(file);
}

/* Retourne le profil correspond à un pseudo dans un fichier
* @fichier : fichier depuis lequel il faut charger le profil
* @pseudo : Pseudo du profil à charger
*
*/
TProfil ChargerProfil(char fichier[TAILLE_CH], char * pseudo) {
	int score;
	char * temp = (char *)malloc(TAILLE_CH * sizeof(char));
	FILE * file = fopen(fichier, "a+");


	while (fscanf(file, "%s %d\n", temp, &score) != EOF && *temp != *pseudo){}
	
	if (*temp == *pseudo) return CreerProfil(pseudo, score);
	else return CreerProfil("", 0);
}


/* Supprime un profil d'un fichier
* @fichier : Fichier dans lequel il faut supprimer
* @Profil : Profil a supprimer
*/
void SupprimerProfil(char fichier[TAILLE_CH], TProfil Profil) {
	int score;
	char * temp = (char *)malloc(TAILLE_CH * sizeof(char));
	

	FILE * file = fopen(fichier,"r"); // Ouverture en lecture seule
	FILE *filetemp = fopen("temp.txt", "w+"); // Ouverture en écriture(+fichier nettoyé + creation si necessaire)


	// Copie sans la ligne du profil a supprimer
	while (fscanf(file, "%s %d\n", temp, &score) != EOF) {
		if (*temp != *Profil.pseudo) fprintf(filetemp, "%s %d\n", temp, score);
		printf("%s %d\n", temp, score);
	}

	fclose(filetemp); fclose(file); // Fermeture
	
	filetemp = fopen("temp.txt", "r+"); // Ouverture en lecture / ecriture
	file = fopen(fichier, "w+"); // Ouverture en lecture / ecriture(+ fichier nettoyé)

	//Recopie de la copie dans le fichier de base
	while (fscanf(filetemp, "%s %d\n", temp, &score) != EOF) fprintf(file, "%s %d\n", temp, score);

	fclose(file); fclose(filetemp); // Fermeture
}



/* Vérifie l'existence d'un pseudo
* @fichier : Nom du fichier contenant les profils (TAILLE_CH car max)
* @pseudo : Pseudo du joueur recherché
* Retourne vrai si le pseudo existe
* Ressemble à charger profil
*/
bool ExistProfil(char fichier[TAILLE_CH], char * pseudo) {
	int score;
	char * pseudtemp = (char *)malloc(TAILLE_CH * sizeof(char));

	
	FILE *file = fopen(fichier, "r");

	while (fscanf(file, "%s %d", pseudtemp, &score) != EOF && *pseudtemp != *pseudo){}

	fclose(file);

	if (strcmp(pseudo,pseudtemp) == 0) return true;
	else return false;
}


/* Modifie le pseudo d'un joueur dans un fichier
* @fichier : Fichier dans lequel il faut modifier le pseudo du joueur
* @Actuel : Pseudonyme actuel du joueur
* @Nouveau : Nouveau pseudonyme du joueur
*/
void ModifPseudo(char fichier[TAILLE_CH], char * Actuel, char * Nouveau) {
	if(ExistProfil(fichier, Actuel)) {
		TProfil Profil = ChargerProfil(fichier, Actuel);
		SupprimerProfil(fichier, Profil);
		Profil.pseudo = Nouveau;
		
		EnregistrerProfil(fichier, Profil);
	}
}

/* Modifie le BestScore d'un joueur dans un fichier
* @ficher : Fichier dans lequel il faut remplacer le score
* @pseudo: Pseudonyme du joueur
* @
*/
void ModifScore(char fichier[TAILLE_CH], char * pseudo, int score) {
	if (ExistProfil(fichier, pseudo)) {
		TProfil Profil = ChargerProfil(fichier, pseudo);
		SupprimerProfil(fichier, Profil);
		Profil.BestScore = score;

		EnregistrerProfil(fichier, Profil);
	}
	else EnregistrerProfil(fichier, CreerProfil(pseudo, score));
}

void MAJScoreProfil(char fichier[TAILLE_CH], TProfil nProfil)
{
	char * pseudo = nProfil.pseudo;
	TProfil aProfil = ChargerProfil(fichier, nProfil.pseudo);
	if (aProfil.BestScore > nProfil.BestScore)
	{
		ModifScore(fichier, pseudo, nProfil.BestScore);
		printf("\nBravo ! Vous avez resolu la grille en %d secondes de moins par rapport à votre score precedent !\n", (int)fabs((nProfil.BestScore - aProfil.BestScore)));
	}
	else printf("\nDommage ! Vous avez résolu la grille en %d secondes de plus par rapport à votre meileur score !\n", (int)fabs((nProfil.BestScore - aProfil.BestScore)));
}
