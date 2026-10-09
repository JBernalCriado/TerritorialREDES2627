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
#include "macros.h"



struct User
{
    char usuario[50];
    char contrasena[50];
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
int agregarUsuarioPartida(int socketjugador);
int agregarUsuario(char *username, char *password);
int buscarEntreUsuario(char *usuario, char *password);
int buscarUsuario(char parameter[50]);
int comprobarNombre(char nombre[50]);
int getIdPartidaJugador(int socket);
int getIndiceJugador(struct Partida *p, int socket);
void procesarSalida(int socket);
void resetearPartida(struct Partida *p);

//REMAKE



int esCliente(char parametros[50]);
int comprobarCliente(int socket);
int esUsuario(char parametros[50]);
int asignarUsuario(int socket, char parametros[50]);
int comprobarClave(char parametros[50], char usuario_actual[50]);


int introducirContra(int socket, char *password);
int comprobarAccion(int socket, int accion);
int enviarAtaque(int socket, char *coords);

void sumarTurno(int socket);
#endif