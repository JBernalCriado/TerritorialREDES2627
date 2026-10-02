#ifndef FUNCIONES_JUEGO_H
#define FUNCIONES_JUEGO_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_CASILLAS 5
#define MAX_TURNOS 20

struct Casilla{
    int jugador, soldados;
};

int numeroAleatorio();
void mostrarTablero(struct Casilla tablero[5][5]);

int ataque(struct Casilla tablero[5][5], int jugador, const char comando[]);
int reforzar(struct Casilla tablero[5][5], int jugador, const char comando[]);
int conquista(struct Casilla tablero[5][5], int jugador, const char comando[]);
void mostrarCasillasDominadas(struct Casilla tablero[5][5], int turno);

void traductorCoordenadasI(int x, int y);
void traductorCoordenadasC(const char coordenadas[], int origen[2], int destino[2], int *soldados);
int accionPermitida(struct Casilla tablero[5][5], int jugador, int accion);
int comprobarVictoria(struct Casilla tablero[5][5], int turno);

char* quitarn(char* cadena);
void limpiarBuffer();

#endif