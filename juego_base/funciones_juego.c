#include "funciones_juego.h"

int numeroAleatorio(){
   return rand() % 5;
}

void mostrarTablero(struct Casilla tablero[5][5]){
    printf("\n");
    for(int i=-1 ; i<5 ; i++){
        for(int j=-1 ; j<5 ; j++){
            if(i == -1 && j == -1){
                printf("        ");
            }
            else if(i == -1){
                printf(" |  %i  | ", i + j + 1);
            }
            else if(j == -1){
                printf("%c         ", 'A' + i);
            }
            else if(tablero[i][j].jugador == 1){
                printf("\033[1;31m(%i, %i)\033[1;0m   ", tablero[i][j].jugador, tablero[i][j].soldados);
            }
            else if(tablero[i][j].jugador == 2){
                printf("\033[1;34m(%i, %i)\033[1;0m  ", tablero[i][j].jugador, tablero[i][j].soldados);
            }
            else{
                printf("(%i, %i)   ", tablero[i][j].jugador, tablero[i][j].soldados);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void ataque(struct Casilla tablero[5][5], int jugador){
    int enemigo = jugador == 1 ? 2 : 1;
    const char *color = jugador == 1 ? "31" : "34";
    int origen[2], destino[2], soldados;
    char comando[20];

    printf("\n\033[1;%smAqui tienes tus casillas marcadas\033[1;0m\n", color);
    mostrarTablero(tablero);
    printf("\033[1;%smCoordenadas: \033[1;0m", color);
    mostrarCasillasDominadas(tablero, jugador - 1);

    limpiarBuffer();
    while (1) {
        printf("Escriba el comando de ataque, ej B4 A3 10 siendo origen destino soldados\n");
        if (fgets(comando, sizeof(comando), stdin) == NULL) {
            return;
        }
        strcpy(comando, quitarn(comando));

        origen[0] = origen[1] = destino[0] = destino[1] = -1;
        soldados = -1;
        traductorCoordenadasC(comando, origen, destino, &soldados);

        if (origen[0] < 0 || origen[0] >= 5 || origen[1] < 0 || origen[1] >= 5 ||
            destino[0] < 0 || destino[0] >= 5 || destino[1] < 0 || destino[1] >= 5 ||
            soldados <= 0 ||
            tablero[origen[0]][origen[1]].jugador != jugador ||
            soldados > tablero[origen[0]][origen[1]].soldados ||
            tablero[destino[0]][destino[1]].jugador != enemigo ||
            (abs(origen[0] - destino[0]) + abs(origen[1] - destino[1]) != 1)) {
            printf("Ataque no permitido: debes atacar una casilla del enemigo adyacente con suficientes soldados.\n");
            continue;
        }
        break;
    }

    printf("Ataque permitido\n");
    tablero[destino[0]][destino[1]].soldados -= soldados;
    tablero[origen[0]][origen[1]].soldados -= soldados;
    if (tablero[origen[0]][origen[1]].soldados == 0) {
        tablero[origen[0]][origen[1]].jugador = 0;
    }
    if(tablero[destino[0]][destino[1]].soldados < 0) {
        tablero[destino[0]][destino[1]].soldados = soldados;
        printf("Conquistaste la casilla del jugador %i, mantuviste %i soldados\n", enemigo, tablero[destino[0]][destino[1]].soldados);
        tablero[destino[0]][destino[1]].jugador = jugador;
    } 
    else if(tablero[destino[0]][destino[1]].soldados == 0) {
        printf("Ambos os quedasteis sin soldados, casilla liberada\n");
        tablero[destino[0]][destino[1]].jugador = 0;
    } 
    else{
        printf("Perdiste la batalla por la casilla, le dejaste %i soldados\n", tablero[destino[0]][destino[1]].soldados);
        tablero[destino[0]][destino[1]].soldados += soldados;
    }
}

void reforzar(struct Casilla tablero[5][5], int jugador){
    int origen[2], destino[2], soldados;
    char comando[20];

    char* color = jugador == 1 ? "\033[1;31m" : "\033[1;34m";
    mostrarTablero(tablero);
    printf("%sCoordenadas: \033[0m", color);
    mostrarCasillasDominadas(tablero, jugador - 1);

    limpiarBuffer();
    while (1) {
        printf("Escriba el comando de refuerzo, ej B4 A3 10 siendo origen destino soldados\n");
        if (fgets(comando, sizeof(comando), stdin) == NULL) {
            return;
        }
        strcpy(comando, quitarn(comando));

        origen[0] = origen[1] = destino[0] = destino[1] = -1;
        soldados = -1;
        traductorCoordenadasC(comando, origen, destino, &soldados);

        if (origen[0] < 0 || origen[0] >= 5 || origen[1] < 0 || origen[1] >= 5 ||
            destino[0] < 0 || destino[0] >= 5 || destino[1] < 0 || destino[1] >= 5 ||
            soldados <= 0 ||
            tablero[origen[0]][origen[1]].jugador != jugador ||
            tablero[destino[0]][destino[1]].jugador != jugador ||
            soldados > tablero[origen[0]][origen[1]].soldados ||
            abs(origen[0] - destino[0]) + abs(origen[1] - destino[1]) != 1) {
            printf("Refuerzo no permitido: ambas casillas deben ser tuyas y adyacentes. Debes tener suficientes soldados en la casilla de origen.\n");
            continue;
        }
        break;
    }

    tablero[origen[0]][origen[1]].soldados -= soldados;
    tablero[destino[0]][destino[1]].soldados += soldados;
    if (tablero[origen[0]][origen[1]].soldados == 0) {
        tablero[origen[0]][origen[1]].jugador = 0;
    }
    printf("Refuerzo realizado: mandaste %i soldados\n", soldados);
}

void conquista(struct Casilla tablero[5][5], int jugador){
    int origen[2], destino[2], soldados;
    char comando[20];

    char* color = jugador == 1 ? "\033[1;31m" : "\033[1;34m";
    mostrarTablero(tablero);
    printf("%sCoordenadas: \033[0m", color);
    mostrarCasillasDominadas(tablero, jugador - 1);

    limpiarBuffer();
    while (1) {
        printf("Escriba el comando de conquista, ej B4 A3 10 siendo origen destino soldados. Debes tener suficientes soldados en la casilla de origen.\n");
        if (fgets(comando, sizeof(comando), stdin) == NULL) {
            return;
        }
        strcpy(comando, quitarn(comando));

        origen[0] = origen[1] = destino[0] = destino[1] = -1;
        soldados = -1;
        traductorCoordenadasC(comando, origen, destino, &soldados);

        if (origen[0] < 0 || origen[0] >= 5 || origen[1] < 0 || origen[1] >= 5 ||
            destino[0] < 0 || destino[0] >= 5 || destino[1] < 0 || destino[1] >= 5 ||
            soldados <= 0 ||
            tablero[origen[0]][origen[1]].jugador != jugador ||
            tablero[destino[0]][destino[1]].jugador != 0 ||
            tablero[origen[0]][origen[1]].soldados < soldados ||
            abs(origen[0] - destino[0]) + abs(origen[1] - destino[1]) != 1) {
            printf("Conquista no permitida: debes mandar soldados de una casilla tuya a una vacia adyacente.\n");
            continue;
        }
        break;
    }

    tablero[destino[0]][destino[1]].jugador = jugador;
    tablero[destino[0]][destino[1]].soldados = soldados;
    tablero[origen[0]][origen[1]].soldados -= soldados;
    if (tablero[origen[0]][origen[1]].soldados == 0) {
        tablero[origen[0]][origen[1]].jugador = 0;
    }
    printf("Conquistaste la casilla y mandaste %i soldados\n", soldados);
}

void mostrarCasillasDominadas(struct Casilla tablero[5][5], int turno){
    for(int i=0 ; i<5 ; i++){
        for(int j=0 ; j<5 ; j++){
            if(tablero[i][j].jugador == turno%2 + 1){
                traductorCoordenadasI(i, j);
                printf(" ");
            }
        }
    }
    printf("\n");
}

void traductorCoordenadasI(int x, int y){
    if(x >= 0 && x < 26 && y >= 0){
        printf("%c%d", 'A' + x, y);
    }
}

void traductorCoordenadasC(char coordenadas[20], int origen[2], int destino[2], int *soldados){
    char columnaOrigen, columnaDestino;
    int filaOrigen, filaDestino;
    int n_soldados;

        if(sscanf(coordenadas, " %c%d %c%d %d", &columnaOrigen, &filaOrigen,
            &columnaDestino, &filaDestino, &n_soldados) == 5 &&
            isalpha((unsigned char)columnaOrigen) &&
            isalpha((unsigned char)columnaDestino))
    {
        columnaOrigen = (char)toupper((unsigned char)columnaOrigen);
        columnaDestino = (char)toupper((unsigned char)columnaDestino);
        printf("(%d, %d) (%d, %d)\n", columnaOrigen - 'A', filaOrigen, columnaDestino - 'A', filaDestino);
        origen[0] = columnaOrigen - 'A';
        origen[1] = filaOrigen;
        destino[0] = columnaDestino - 'A';
        destino[1] = filaDestino;
        *soldados = n_soldados;
    }
    else {
        printf("Formato invalido. Use: B4 A3\n");
        return;
    }
}

int accionPermitida(struct Casilla tablero[5][5], int jugador, int accion){
    int contrario = jugador == 1 ? 2 : 1;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(tablero[i][j].jugador != jugador)
                continue;

            if((i > 0 && ((accion == 1 && tablero[i - 1][j].jugador == contrario) ||
                          (accion == 2 && tablero[i - 1][j].jugador == jugador) ||
                          (accion == 3 && tablero[i - 1][j].jugador == 0))) ||
               (i < 4 && ((accion == 1 && tablero[i + 1][j].jugador == contrario) ||
                          (accion == 2 && tablero[i + 1][j].jugador == jugador) ||
                          (accion == 3 && tablero[i + 1][j].jugador == 0))) ||
               (j > 0 && ((accion == 1 && tablero[i][j - 1].jugador == contrario) ||
                          (accion == 2 && tablero[i][j - 1].jugador == jugador) ||
                          (accion == 3 && tablero[i][j - 1].jugador == 0))) ||
               (j < 4 && ((accion == 1 && tablero[i][j + 1].jugador == contrario) ||
                          (accion == 2 && tablero[i][j + 1].jugador == jugador) ||
                          (accion == 3 && tablero[i][j + 1].jugador == 0))))
                return 1;
        }
    }
    return 0;
}

int comprobarVictoria(struct Casilla tablero[5][5], int turno){
    int territoriosJugador1 = 0;
    int territoriosJugador2 = 0;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(tablero[i][j].jugador == 1)
                territoriosJugador1++;
            else if(tablero[i][j].jugador == 2)
                territoriosJugador2++;
        }
    }

    printf("\n\033[1;32mCasillas conquistadas: \033[1;31mjugador 1: %i \033[0m| \033[1;34mjugador 2: %i\033[0m\n",territoriosJugador1, territoriosJugador2);

    if(territoriosJugador1 == 0 && territoriosJugador2 == 0){
        printf("Empate: ambos jugadores se han quedado sin casillas.\n");
        return 1;
    }
    if(territoriosJugador1 == 0){
        printf("Gana el jugador 2: el jugador 1 se ha quedado sin casillas.\n");
        return 1;
    }
    if(territoriosJugador2 == 0){
        printf("Gana el jugador 1: el jugador 2 se ha quedado sin casillas.\n");
        return 1;
    }
    if(territoriosJugador1 == 25){
        printf("Gana el jugador 1: ha conquistado todas las casillas.\n");
        return 1;
    }
    if(territoriosJugador2 == 25){
        printf("Gana el jugador 2: ha conquistado todas las casillas.\n");
        return 1;
    }
    if(turno >= MAX_TURNOS){
        if(territoriosJugador1 > territoriosJugador2)
            printf("Gana el jugador 1 por tener mas territorios.\n");
        else if(territoriosJugador2 > territoriosJugador1)
            printf("Gana el jugador 2 por tener mas territorios.\n");
        else
            printf("Empate: ambos jugadores tienen el mismo numero de territorios.\n");
        return 1;
    }

    return 0;
}


char* quitarn(char* cadena){

    size_t longitud = strlen(cadena);
    if (longitud > 0 && cadena[longitud - 1] == '\n') {
        cadena[longitud - 1] = '\0';
    }
    return cadena;
}

void limpiarBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}