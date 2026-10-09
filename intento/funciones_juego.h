#ifndef FUNCIONES_JUEGO_H
#define FUNCIONES_JUEGO_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdbool.h>
#include "macros.h"



struct Casilla{
    int jugador, soldados;
    //Para indicar si una coordenada ha cambiado y si se ha pasado

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

void CrearTablero(struct Casilla matriz_casillas[5][5]);
void mostrarTableroString(struct Casilla tablero[5][5]);

void reconstruirTablero(struct Casilla tablero[5][5], char mensaje[200]);
void codificarTablero(struct Casilla tablero[5][5], char mensaje[200]);

#endif