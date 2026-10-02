#include "funciones_juego.h"

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
        x = numeroAleatorio();
        y = numeroAleatorio();
        tablero[x][y].jugador = 2;
        tablero[x][y].soldados = 30;

        //Variables de control
        int opcion=1;
        int turno = 0;
    

    while(1){

        mostrarTablero(tablero); //El tablero se muestra al inicio de cada turno
        
        if(comprobarVictoria(tablero, turno)) //Luego se comprueba la condicion de victoria
            break;


        int accionRealizada = 0; //Booleano para comprobar si la accion es posible
        int jugador = turno % 2 + 1; //Controlador de turno
        char* color = jugador == 1 ? "\033[1;31m" : "\033[1;34m"; //Controlador para el color de los printf para cada jugador
        
        //Muestreo de opciones del menu
        printf("\n\033[1;32mTURNO %s%i\033[0m\n1 -> Atacar\n2 -> Reforzar\n3 -> Conquistar\n4 -> Pasar\n\t¿?: ",color, turno+1);
        scanf("%i", &opcion);
        
        switch(opcion){
            case 1:
                if(accionPermitida(tablero, jugador, 1)){
                    ataque(tablero, jugador);
                    accionRealizada = 1;
                }
            break;

            case 2:
                if(accionPermitida(tablero, jugador, 2)){
                    reforzar(tablero, jugador);
                    accionRealizada = 1;
                }
            break;
            
            case 3:
                if(accionPermitida(tablero, jugador, 3)){
                    conquista(tablero, jugador);
                    accionRealizada = 1;
                }
            break;

            case 4:
                // Pasar turno
                accionRealizada = 1;
            break;
        }

        if(accionRealizada)
            turno++;
        else{
            printf("\n%sAccion imposible para el jugador %i\033[0m\n", color, jugador);
        }
    }

}