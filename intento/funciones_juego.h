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

#define PORT 2026
#define MAX_CLIENTS 10
#define MAX_PARTIDAS 4
#define BUFFER_SIZE 1000
#define MAX_CASILLAS 5
#define MAX_TURNOS 100

#define DEFAULT    "\x1B[0m"
#define BG_RED     "\x1B[41m"
#define BG_GRAY    "\x1B[48;2;176;174;174m"
#define BG_GREEN   "\x1B[42m"
#define BLACK   "\x1B[30m"
#define WHITE   "\x1B[37m"



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