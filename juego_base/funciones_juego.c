#include "funciones_juego.h"
#include <string.h>
#define DEFAULT    "\x1B[0m"
#define BG_GRAY    "\x1B[48;2;176;174;174m"
#define BG_WHITE   "\x1B[47m"
#define BG_RED     "\x1B[41m"
#define BG_BLUE    "\x1B[44m"
#define WHITE   "\x1B[37m"
#define GRAY    "\x1B[38;2;176;174;174m"
/*Genera el numero aleatorio para las casillas iniciales
  @return Numero aleatorio entre 0 y 4
*/
int numeroAleatorio(){
   return rand() % 5;
}

/*Muestra el tablero de juego con los jugadores y sus soldados
  @param tablero Matriz del mapa de juego
*/
void mostrarTablero(struct Casilla tablero[5][5]){
    printf("\n");
    for(int i=-1 ; i<5 ; i++){
        for(int j=-1 ; j<5 ; j++){
            if(i == -1 && j == -1){
                printf("   ");
            }
            else if(i == -1){
                printf("  %i ", i + j + 2);
            }
            else if(j == -1){
                printf(" %c ", 'A' + i);
            }
            else if(tablero[i][j].jugador == 1){
                if(tablero[i][j].soldados<10){printf("%s  %i %s",BG_RED, tablero[i][j].soldados,DEFAULT);}
                if(tablero[i][j].soldados>=10){printf("%s %i %s",BG_RED, tablero[i][j].soldados,DEFAULT);}

            }
            else if(tablero[i][j].jugador == 2){
                if(tablero[i][j].soldados<10){printf("%s  %i %s",BG_BLUE, tablero[i][j].soldados,DEFAULT);}
                if(tablero[i][j].soldados>=10){printf("%s %i %s",BG_BLUE, tablero[i][j].soldados,DEFAULT);}

            }
            else{
                if(i%2==0){
                if(j%2!=0){
                printf("%s%s(30)%s",WHITE, BG_WHITE,DEFAULT);
                }
                if(j%2==0){
                printf("%s%s(30)%s",GRAY, BG_GRAY,DEFAULT);
                }
            }
                if(i%2!=0){
                if(j%2!=0){
                printf("%s%s(30)%s",GRAY, BG_GRAY,DEFAULT);
                }
                if(j%2==0){
                printf("%s%s(30)%s",WHITE, BG_WHITE,DEFAULT);
                }
            }
            }
        }
        printf("\n");
    }
    printf("\n");
}

/*
  Funcion para realizar el ataque al otro jugador.
  @param tablero Matriz del mapa de juego
  @param jugador id del Jugador que realiza el ataque
  @param comando Parametros de coordenadas y soldados a enviar
  @return 1 si el ataque es válido, 0 en caso contrario
*/
int ataque(struct Casilla tablero[5][5], int jugador, const char comando[]){
    int enemigo = jugador == 1 ? 2 : 1;
    const char *color = jugador == 1 ? "31" : "34";
    int origen[2], destino[2], soldados;

    printf("\n\033[1;%smAqui tienes tus casillas marcadas\033[1;0m\n", color);

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
        return 0;
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
        printf("Perdiste la batalla por la casilla, perdiste %i soldados\n", soldados);
        tablero[destino[0]][destino[1]].soldados += soldados;
    }
    return 1;
}

/*Funcion para reforzar una de tus casillas con más soldados
  @param tablero Matriz del mapa de juego
  @param jugador id del Jugador que realiza la accion
  @param comando Parametros de coordenadas y soldados a enviar
  @return 1 si el refuerzo es válido, 0 en caso contrario
*/
int reforzar(struct Casilla tablero[5][5], int jugador, const char comando[]){
    int origen[2], destino[2], soldados;
    char* color = jugador == 1 ? "\033[1;31m" : "\033[1;34m";

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
        return 0;
    }

    tablero[origen[0]][origen[1]].soldados -= soldados;
    tablero[destino[0]][destino[1]].soldados += soldados;
    if (tablero[origen[0]][origen[1]].soldados == 0) {
        tablero[origen[0]][origen[1]].jugador = 0;
    }
    printf("Refuerzo realizado: mandaste %i soldados\n", soldados);
    return 1;
}

/*Funcion para conquistar una casilla vacia
  @param tablero Matriz del mapa de juego
  @param jugador id del Jugador que realiza la accion
  @param comando Parametros de coordenadas y soldados a enviar
  @return 1 si la conquista es válida, 0 en caso contrario
*/
int conquista(struct Casilla tablero[5][5], int jugador, const char comando[]){
    int origen[2], destino[2], soldados;
    char* color = jugador == 1 ? "\033[1;31m" : "\033[1;34m";

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
        return 0;
    }

    tablero[destino[0]][destino[1]].jugador = jugador;
    tablero[destino[0]][destino[1]].soldados = soldados;
    tablero[origen[0]][origen[1]].soldados -= soldados;
    if (tablero[origen[0]][origen[1]].soldados == 0) {
        tablero[origen[0]][origen[1]].jugador = 0;
    }
    printf("Conquistaste la casilla y mandaste %i soldados\n", soldados);
    return 1;
}

/*Muestra la lista de casillas dominadas por el jugador
    @param tablero Matriz del mapa de juego
    @param turno id del jugador actual
*/
void mostrarCasillasDominadas(struct Casilla tablero[5][5], int turno){
    for(int i=0 ; i<5 ; i++){
        for(int j=0 ; j<5 ; j++){
            if(tablero[i][j].jugador == turno%2 + 1){
                traductorCoordenadasI(i, j+1);
                printf(" ");
            }
        }
    }
    printf("\n");
}

/*Pasa las coordenadas del formato (x, y) a A0/A1/.../E4
  @param x Coordenada x de la casilla
  @param y Coordenada y de la casilla
*/
void traductorCoordenadasI(int x, int y){
    if(x >= 0 && x < 26 && y >= 0){
        printf("%c%d", 'A' + x, y);
    }
}

/*Extrae las coordenadas de para que las entienda el programa y extrae la cantidad de soldados usados.
  @param coordenadas Cadena de caracteres con las coordenadas y soldados
  @param origen Array de dos enteros para almacenar las coordenadas (x, y) de origen
  @param destino Array de dos enteros para almacenar las coordenadas (x, y) de destino
  @param soldados Puntero a un entero para almacenar la cantidad de soldados
*/
void traductorCoordenadasC(const char coordenadas[], int origen[2], int destino[2], int *soldados){
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
        origen[0] = columnaOrigen - 'A';
        origen[1] = filaOrigen-1;
        destino[0] = columnaDestino - 'A';
        destino[1] = filaDestino-1;
        *soldados = n_soldados;
    }
    else {
        printf("Formato invalido. Use: B4 A3 soldados\n");
        return;
    }
}

/*Comprueba que la accion es posible para el jugador actual
  @param tablero Matriz del mapa de juego
  @param jugador id del Jugador que realiza la accion
  @param accion id de la accion a realizar (1: atacar, 2: reforzar, 3: conquistar)
  @return 1 si la accion es posible, 0 en caso contrario
*/
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

/*Comprobacion de la condicion de victoria o fin del juego
  @param tablero Matriz del mapa de juego
  @param turno Numero de turnos jugados
  @return 1 si hay un ganador o empate (fin de la partida), 0 en caso contrario
*/
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
    
    if(territoriosJugador1 == 25){
        printf("Gana el jugador 1: ha conquistado todas las casillas.\n");
        return 1;
    }
    if(territoriosJugador2 == 25){
        printf("Gana el jugador 2: ha conquistado todas las casillas.\n");
        return 1;
    }
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

/*Elimina el caracter del salto de linea al final de la cadena si existe
  @param cadena Cadena de caracteres a modificar
  @return Puntero a la cadena modificada
*/
char* quitarn(char* cadena){

    size_t longitud = strlen(cadena);
    if (longitud > 0 && cadena[longitud - 1] == '\n') {
        cadena[longitud - 1] = '\0';
    }
    return cadena;
}

/*Limpiar el buffer para evitar problemas en el fgetc
*/
void limpiarBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}