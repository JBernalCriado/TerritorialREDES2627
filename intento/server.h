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
#include "funciones_juego.h"
#define PORT 2026
#define MAX_CLIENTS 10
#define MAX_PARTIDAS 4

#define BG_YELLOW  "\x1B[43m"
#define BLACK   "\x1B[30m"
#define DEFAULT    "\x1B[0m"
#define BG_RED     "\x1B[41m"
#define BG_GRAY    "\x1B[48;2;176;174;174m"
#define BG_GREEN   "\x1B[42m"
#define WHITE   "\x1B[37m"
#define BG_WHITE   "\x1B[47m"
#define BG_RED     "\x1B[41m"
#define BG_BLUE    "\x1B[44m"
#define GRAY    "\x1B[38;2;176;174;174m"



struct User
{
    char usuario[50];
    char contraseña[50];
};


struct Partida
{
    int id;
    int estado;             
    int jugadores[2];
    struct Casilla tablero [5][5];
    int turno;  
};

struct Cliente
{
    int socket;
    int registered;
    char usuario[50];
    char contraseña[50];
};
void listarClientes();
/*
Funcion encargada de ingresar los jugadores en las la estructura cliente que almacena a los que estén conectados
*/
int registrarCliente(int socket);
/*
Funcion encargada de comprobar que los clientes estén en una partida
*/
int agregarUsuarioPartida(int socketjugador);
int agregarUsuario(char *username, char *password);
int asignarUsuario(int socket, char *user);
int buscarEntreUsuario(char *usuario, char *password);
int buscarUsuario(char *parameter, int descriptor);
int comprobarCliente(int socket);
int comprobarNombre(char *nombre);
int getIdPartidaJugador(int socket);
int getIndiceJugador(struct Partida *p, int socket);
void procesarSalida(int socket);
void resetearPartida(struct Partida *p);

int introducirContra(int socket, char *password);
int comprobarAccion(int socket, int accion);
int enviarAtaque(int socket, char *coords);

#endif