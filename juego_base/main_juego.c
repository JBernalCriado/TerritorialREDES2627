#include "funciones_juego.h"
#include <string.h>

int main(){
    srand(time(NULL)); //Establece la semilla para los numeros aleatorios

    struct Casilla tablero[5][5]; //Matriz para el mapa


    for(int i=0 ; i<5 ; i++){ //Inicializa el tablero con casillas vacias
        for(int j=0 ; j<5 ; j++){
            tablero[i][j].jugador = 0;
            tablero[i][j].soldados = 0;
        }
    }


    //Inicializacion de la partida
        //Casilla inicial del jugador 1
        int x = numeroAleatorio();
        int y = numeroAleatorio();
        tablero[x][y].jugador = 1;
        tablero[x][y].soldados = 30;
        
        //Casilla inicial del jugador 2
        while(1){
            x = numeroAleatorio();
            y = numeroAleatorio();
            if(tablero[x][y].jugador == 0){ //Para no solapar las casillas iniciales
                break;
            }
        }
        tablero[x][y].jugador = 2;
        tablero[x][y].soldados = 30;

        //Variables de control
        int turno = 0;
    

    while(1){

        mostrarTablero(tablero); //El tablero se muestra al inicio de cada turno
        
        if(comprobarVictoria(tablero, turno)) //Luego se comprueba la condicion de victoria
            break;

        int accionRealizada = 0; //Booleano para comprobar si la accion es posible
        int jugador = turno % 2 + 1; //Controlador de turno
        char* color = jugador == 1 ? "\033[1;31m" : "\033[1;34m"; //Controlador para el color de los printf para cada jugador
        char entrada[100]; //Guarda todo el comando introducido por el jugador
        char comando[20]; //Guarda el comando
        char parametros[80]; //Guarda los parametros del comando
        int partes;
        int opcion = 0;

        printf("\n\033[1;32mTURNO %s%i\033[0m\n", color, turno + 1);

        printf("%sTus casillas: \033[0m", color);
        mostrarCasillasDominadas(tablero, jugador - 1);

        printf("Introduzca un comando (ATACAR, REFORZAR, CONQUISTAR o PASAR): ");
        if (fgets(entrada, sizeof(entrada), stdin) == NULL)
            break;
        quitarn(entrada);

        parametros[0] = '\0';
        
        //Separa el comando de los parametros, comando guarda una cadena de 19 caracteres
        //parametros guarda 79 caracteres que no sean \n
        partes = sscanf(entrada, " %19s %79[^\n]", comando, parametros);
        
        //Comando vacio
        if (partes < 1) {
            printf("Comando vacio.\n");
            continue;
        }

        //Paso a mayusculas
        for (char *letra = comando; *letra != '\0'; letra++)
            *letra = (char) toupper((unsigned char) *letra);

        //Asignacion de acciones
        if (strcmp(comando, "ATACAR") == 0)
            opcion = 1;
        else if (strcmp(comando, "REFORZAR") == 0)
            opcion = 2;
        else if (strcmp(comando, "CONQUISTAR") == 0)
            opcion = 3;
        else if (strcmp(comando, "PASAR") == 0)
            opcion = 4;

        switch (opcion) {
        case 1:
            if (accionPermitida(tablero, jugador, 1) && partes == 2)
                accionRealizada = ataque(tablero, jugador, parametros);
            break;
        case 2:
            if (accionPermitida(tablero, jugador, 2) && partes == 2)
                accionRealizada = reforzar(tablero, jugador, parametros);
            break;
        case 3:
            if (accionPermitida(tablero, jugador, 3) && partes == 2)
                accionRealizada = conquista(tablero, jugador, parametros);
            break;
        case 4:
            if (partes == 1)
            accionRealizada = 1;
            break;
        default:
            printf("Comando invalido. Use ATACAR, REFORZAR, CONQUISTAR o PASAR.\n");
            break;
        }

        if(accionRealizada)
            turno++;
        else{
            printf("\n%sAccion imposible para el jugador %i\033[0m\n", color, jugador);
        }
    }

}