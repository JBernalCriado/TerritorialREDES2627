#!/bin/bash
clear
gcc cliente.c funciones_juego.h funciones_juego.c macros.h -o cliente
gcc servermain.c serverfunc.c server.h funciones_juego.h funciones_juego.c macros.h -o server
