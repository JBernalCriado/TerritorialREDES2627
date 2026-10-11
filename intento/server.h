#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "macros.h"
#include <stdbool.h>
#include <ctype.h>

struct Casilla{
    int jugador, soldados;
    //Para indicar si una coordenada ha cambiado y si se ha pasado

};

struct User
{
    char usuario[50];
    char contrasena[50];
};


struct Partida
{
    int id;// Identificador de partida
    int estado;
    //Indica si la partida está en curso 0 no lo está y si es 1 lo está
    int jugadores[2];// Guarda si es jugador 1 o jugador 2 a través de alamacenar su socket
    struct Casilla tablero [5][5];// Tablero de la partida
    int turno; 
};

struct Cliente
{
    int socket;
    int registered;
    char usuario[50];
    char contrasena[50];
};
void listarClientes();
/*
Funcion encargada de ingresar los jugadores en las la estructura cliente que almacena a los que estén conectados
*/
int registrarCliente(int socket);
/*
Funcion encargada de comprobar que los clientes estén en una partida
*/

//REMAKE
int agregarUsuario(char *username, char *password);
int asignarUsuario(int socket, char parametros[50]);
int esCliente(char parametros[50]);
int comprobarCliente(int socket);
int comprobarClave(char parametros[50], int socket);
int esUsuario(char parametros[50]);

//Funciones Juego
void iniciarPartidas();
void crearTablero(struct Casilla tablero[5][5]);
void enviarTableroAJugadores(struct Casilla tablero[5][5], int indicePartida);
void codificarTablero(struct Casilla tablero[5][5], char mensaje[200]);
void reconstruirTablero(struct Casilla tablero[5][5], char mensaje[200]);
int numeroAleatorio();
void mostrarTablero(struct Casilla tablero[5][5]);
int comprobarAccion(int socket, int accion);
int comprobarVictoria(int socket);
char* quitarn(char* cadena);


//REMAKE 2
int agregarUsuarioPartida(int socketjugador);
int comprobarAccion(int socket, int accion);
void mostrarTableroClientes(struct Casilla tablero[5][5], int jugador);
int Conquista(int socket, char coordenadas[50]);
int Ataque(int socket, char coordenadas[50]);
int Reforzar(int socket, char coordenadas[50]);
void traductorCoordenadasC(const char coordenadas[], int origen[2], int destino[2], int *soldados);
int getIndicePartida(int socket);
int getIdJugador(int socket, int indicePartida);
void actualizarPartida(int socket);
int salir(int socket);  
void chaparPartida(int socket);
#endif