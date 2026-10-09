#!/bin/bash
gcc cliente.c funciones_juego.h funciones_juego.c -o cliente
gcc servermain.c serverfunc.c server.h funciones_juego.h funciones_juego.c -o server