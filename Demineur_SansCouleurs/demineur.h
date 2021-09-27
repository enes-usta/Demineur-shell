#ifndef __D3MIN3UR__
#define __D3MIN3UR__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
	+-------------------------+
	|   PRIMITIVES MEMOIRE    |
	+-------------------------+

// Structure pour une cellule*/
typedef struct {
    int Valeur; // Nombre de bombes autour de la case
    bool Bombe; // Bombe ou non
    bool Demasquee; // Si demasquee
    bool Etat; // Drapeau ou non
} cell_t;

// Structure d'une grille
typedef cell_t * grille_t;

// Genere une grille
grille_t GenererGrille(int Taille, int Bombe, int c);

// Libere une structure grille_t
void LibererGrille(grille_t Grille);

// Genere un pointeur d'entier
int * Int();

// Libere un pointeur entier
void LibererInt(int * n);

// Libere un pointeur de caracteres
void LibererChar(char * c);

/*
	+-------------------------+
	| PRIMITIVES CONSULTATION |
	+-------------------------+

// Affiche la grille */
void AfficherGrille(grille_t Grille, int Taille);

// Affiche la solution d'une grille_t
void AfficherSolution(grille_t Grille, int Taille);

// Verifie la grille
int VerifierGrille(grille_t Grille, int T, int Nbb);

// Determine la case correspondante dans une grille
int CompterCase(int T, int x, int y);

// Comptabilise le nombre de bombes autours d'une case
void CompterBombes(grille_t Grille, int T, int x, int y);


// Demasque la grille a la position c
// void Demasquer(tgrille *, int, int, int);
void Demasquer(grille_t Grille, int T, int x, int y); 

/*
	+-------------------------+
	| PRIMITIVES ULTILISATEUR |
	+-------------------------+

*/

// Demande la saisie de coordonnees et l'action a y effectuer */
void Demander(int * x, int * y, int Taille, int *Action);

// Demande la longueur de la grille au joueur
int Difficultee(int * T, int * B);

#endif
