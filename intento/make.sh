#!/bin/bash
clear
gcc cliente.c serverfunc.c cliente.h macros.h server.h -o cliente
gcc servermain.c serverfunc.c server.h macros.h -o server
