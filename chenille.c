#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "consoleToolsDan.h"
#include "chenille.h"

#define FEUILLETAILLEX 40 // taille de la feuille
#define FEUILLETAILLEY 30
#define CHENILLETAILLEMAX 20 // taille max de la chenille
#define CHENILLETAILLEINIT 5 // taille initiale de la chenille

struct chenille { char direction; int taille; COORD coordxy[CHENILLETAILLEMAX]; };
typedef struct chenille CHENILLE;
char feuille[FEUILLETAILLEX][FEUILLETAILLEY] = { ' ' };

int rangeRand(int minValue, int maxValue) {
	return rand() % (maxValue + 1 - minValue) + minValue;
}

/* initialise le tableau feuille avec des espaces et des caractères @ 
à des positions aléatoires et place les caractères @ à l’écran */
void initFeuille() {
	for (int p = 0; p < (CHENILLETAILLEMAX - CHENILLETAILLEINIT); p++) {
		int rand_x = 0, rand_y = 0;
		// on verifie que les @ ne se superposent pas a la position init de la chenille
		do {
			rand_x = rangeRand(CHENILLETAILLEINIT, FEUILLETAILLEX);
			rand_y = rangeRand(0, FEUILLETAILLEY);
		} while (rand_y == 0 && rand_x <= CHENILLETAILLEINIT);
		// On place les arobases en ces coord X, Y dans le tableau
		feuille[rand_x][rand_y] = '@';
	}
}

// Affiche (ou remplace si deja existant) la feuille dans la console
void afficheFeuille() {
	for (int i = 0; i < FEUILLETAILLEX; i++) {
		for (int j = 0; j < FEUILLETAILLEY; j++) {
			plotCharToXY(feuille[i][j], i, j);
		}
	}
}

// initialise direction, taille et position
void initChenille(CHENILLE* chenille) {
	(*chenille).direction = 'E';
	(*chenille).taille = CHENILLETAILLEINIT;
	for (int x = 0; x < CHENILLETAILLEINIT; x++) {
		COORD pos = { CHENILLETAILLEINIT - 1 - x, 0 };
		(*chenille).coordxy[x] = pos;
	}
}

// dessin chenille a l'ecran en fonction de ses coord.
int dessineChenille(CHENILLE chenille) {
	for (int i = 0; i < chenille.taille; i++) {
		int x = chenille.coordxy[i].X;
		int y = chenille.coordxy[i].Y;
		if (plotCharToXY('*', x, y) == 0)
			return 0;
	}
	return 1;
}

/* Efface la chenille de l'affichage. Chaque * est remplace par un espace. */
int effaceChenille(CHENILLE chenille) {
	for (int i = 0; i < chenille.taille; i++) {
		int x = chenille.coordxy[i].X;
		int y = chenille.coordxy[i].Y;
		if (plotCharToXY(' ', x, y) == 0)
			return 0;
	}
	return 1;
}

/* renvoie 1 si la chenille ne sort pas de la feuille au prochain 
deplacement. Renvoie 0 sinon. */ 
int prochainDeplacementEstValide(CHENILLE chenille) {
	int Xtete = chenille.coordxy[0].X;
	int Ytete = chenille.coordxy[0].Y;
	char direction = chenille.direction;
	switch (direction)
	{
	case 'N':
		if (Ytete != 0)
			return 1;
		return 0;
	case 'E':
		if (Xtete != FEUILLETAILLEX - 1)
			return 1;
		return 0;
	case 'S':
		if (Ytete != FEUILLETAILLEY - 1)
			return 1;
		return 0;
	case 'O':
		if (Xtete != 0)
			return 1;
		return 0;
	default:
		return 0;
	}
}

/* renvoie une direction aleatoire possible en fonction de la
position de la chenille. */
char selectDirection(CHENILLE chenille) {
	int Xtete = chenille.coordxy[0].X;
	int Ytete = chenille.coordxy[0].Y;
	char directions[4] = { 'S', 'O', 'N', 'E' };
	char direction_alea;
	int directionChoisieEstValide = 0;
	do {
		direction_alea = directions[rangeRand(0, 3)];
		switch (direction_alea)
		{
		case 'N':
			if (Ytete != 0)
				directionChoisieEstValide = 1;
			break;
		case 'E':
			if (Xtete != FEUILLETAILLEX - 1)
				directionChoisieEstValide = 1;
			break;
		case 'S':
			if (Ytete != FEUILLETAILLEY - 1)
				directionChoisieEstValide = 1;
			break;
		case 'O':
			if (Xtete != 0)
				directionChoisieEstValide = 1;
			break;
		}
	} while (directionChoisieEstValide == 0);
	return direction_alea;
}

void copieTableau(int* tableauAcopier, int* tableauDeDestination) {
	for (int i = 0; i < 10; i++)
		*(tableauDeDestination + i) = *(tableauAcopier + i);
}

/* deplace la chenille d’une position à l’ecran, met a jour son
tableau de coordonnees et eventuellement sa taille dans le cas ou
elle a mange un @. La fonction renvoie la valeur 0 si on tente de
faire sortir la chenille de la feuille, 1 sinon. */
int avanceChenille(CHENILLE* chenille) {
	if (prochainDeplacementEstValide(*chenille) == 0)
		return 0;
	else {
		effaceChenille(*chenille);
		// On sauvegarde les coord de fin de la chenille au cas ou la chenille mange un @
		COORD sauvCoordFin = (*chenille).coordxy[(*chenille).taille - 1];
		// Hormi la tete, toutes les coord prennent la valeur de la suivante
		for (int i = (*chenille).taille - 1; i > 0; i--) {
			(*chenille).coordxy[i] = (*chenille).coordxy[i - 1];
		}
		// On deplace la tete selon la direction de la chenille
		COORD coordTete = (*chenille).coordxy[0];
		switch ((*chenille).direction)
		{
		case 'N':
			(*chenille).coordxy[0].Y = coordTete.Y - 1;
			break;
		case 'E':
			(*chenille).coordxy[0].X = coordTete.X + 1;
			break;
		case 'S':
			(*chenille).coordxy[0].Y = coordTete.Y + 1;
			break;
		case 'O':
			(*chenille).coordxy[0].X = coordTete.X - 1;
			break;
		}
		/* Si la prochaine coordonnee parcourue est un @ sur la feuille */
		if (feuille[coordTete.X][coordTete.Y] == '@') {
			feuille[coordTete.X][coordTete.Y] = ' ';
			/* ---- la chenille augmente de taille */
			(*chenille).taille++;
			(*chenille).coordxy[(*chenille).taille - 1] = sauvCoordFin;
			/* ---- on actualise a l'ecran la feuille */
			afficheFeuille();
		}
		dessineChenille(*chenille);
		return 1;
	}
}


void main() {
	srand((unsigned)time(NULL));

	CHENILLE chenille;

	initFeuille();
	afficheFeuille();
	initChenille(&chenille);
	dessineChenille(chenille);

	while (chenille.taille < CHENILLETAILLEMAX) {
		int nbDeplacementsAvantNvlleDirection = rangeRand(1, 10);
		int compteDeplacements = 0;
		while (avanceChenille(&chenille) && (compteDeplacements < nbDeplacementsAvantNvlleDirection)) {
			compteDeplacements++;
			// Sleep(200);
		}
		chenille.direction = selectDirection(chenille);
	}
	moveCursor(0, FEUILLETAILLEY + 10 );
}