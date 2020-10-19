#include <windows.h> 
#include <time.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>

HANDLE hStdout, hStdin;
CHAR chBuffer[256];
DWORD cRead, cWritten, fdwMode, fdwOldMode;

// déplace le curseur en X,Y position du prochain affichage
void moveCursor(unsigned short int X, unsigned short int Y) {
	COORD pos = { X, Y };
	SetConsoleCursorPosition(hStdout, pos);
}

// affiche un caractère à la position du curseur
int plotChar(char SomeChar) {
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE) {
		return -1;
	}
	chBuffer[0] = SomeChar;
	chBuffer[1] = 0;
	cRead = 1;
	WriteFile(hStdout, chBuffer, cRead, &cWritten, NULL);
	return 1;
}

// affiche un caractère à la position X, Y
int plotCharToXY(char Somechar, unsigned short int X, unsigned short int Y) {
	moveCursor(X, Y);
	if (plotChar(Somechar) == 1)
		return 1;
	return 0;
}