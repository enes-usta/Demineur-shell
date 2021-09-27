#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAILLE_CH 25 // Nombre de car maximum pour nom fichier/pseudo

typedef struct {
	char * pseudo; // Pseudo d'un joueur
	int BestScore; // Meilleur score du joueur
} TProfil;


// Retourne un profil initialisé
TProfil CreerProfil(char *, int);

// Enregistre un profil dans un fichier
void EnregistrerProfil(char fichier[TAILLE_CH], TProfil Profil);

// Vérifie l'existence d'un joueur
bool ExistProfil(char fichier[TAILLE_CH], char * pseudo);

// Charge TProfil depuis un fichier
TProfil ChargerProfil(char fichier[TAILLE_CH], char * pseudo);

// Supprime un TProfil dans un fichier dont le pseudo correspond (et aps le score)
void SupprimerProfil(char fichier[TAILLE_CH], TProfil Profil);

// Modifie le pseudo
void ModifPseudo(char fichier[TAILLE_CH], char * Actuel, char * Nouveau);

// Modifie le meilleur score
void ModifScore(char fichier[TAILLE_CH], char * pseudo, int score);

// Met à joueur le profil dans le fichier
void MAJScoreProfil(char fichier[TAILLE_CH], TProfil Profil);