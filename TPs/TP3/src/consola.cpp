#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include "consola.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include "nodo.hpp"

using namespace std;

#define CMD_LOAD    "load"
#define CMD_ADD     "addAndInc"
#define CMD_MEMBER  "member"
#define CMD_MAXIMUM "maximum"
#define CMD_QUIT    "quit"
#define CMD_SQUIT   "q"

static unsigned int np;

// Crea un ConcurrentHashMap distribuido
static void load(list<string> params) {
    char msg[BUFFER_SIZE];

    int i = 0;
    for (list<string>::iterator it=params.begin(); it != params.end(); ++it) {
        strcpy(msg, (*it).c_str());
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,i%(np-1)+1,TAG_LOAD,MPI_COMM_WORLD);
        i++;
    }

    cout << "La lista esta procesada" << endl;
}

// Esta función debe avisar a todos los nodos que deben terminar
static void quit() {
    char msg[BUFFER_SIZE];
    for (unsigned int i = 1; i<np ; i++){
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,i,TAG_QUIT,MPI_COMM_WORLD);
    }
}

// Esta función calcula el máximo con todos los nodos
static void maximum() {

    char msg[BUFFER_SIZE];
    for (unsigned int i = 1; i<np; i++){
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,i,TAG_MAXIMUM,MPI_COMM_WORLD);
    }

    string str("La tabla esta vacia");
    pair<string, unsigned int> result = make_pair(str,0);

    int winner_rank = -1;
    MPI_Status status;   // required variable for receive routines

    unsigned int number;
    for (unsigned int i = 1; i<np; i++){
        MPI_Recv(&number,1,MPI_INT,i,TAG_MAXIMUM_NUMBER_RESPONSE,MPI_COMM_WORLD,&status);
        printf("%d\n",number);
        if(result.second < number){
            result.second = number;
            winner_rank = i;
        }
    }

    MPI_Request request;   // required variable for non-blocking calls
    MPI_Irecv(&msg,BUFFER_SIZE,MPI_CHAR,winner_rank,TAG_MAXIMUM_WORD_RESPONSE,MPI_COMM_WORLD,&request);
    string word(msg);

    result.first = word;

    if(result.second == 0){
        cout << "No hay maximo, la tabla esta vacía." << endl;
    } else {
    cout << "El máximo es <" << result.first <<"," << result.second << ">" << endl;
    }

}

// Esta función busca la existencia de *key* en algún nodo
static void member(string key) {
    bool esta = false;

    // TODO: Implementar

    cout << "El string <" << key << (esta ? ">" : "> no") << " está" << endl;
}


// Esta función suma uno a *key* en algún nodo
static void addAndInc(string key) {

    // TODO: Implementar

    cout << "Agregado: " << key << endl;
}


/* static int procesar_comandos()
La función toma comandos por consola e invoca a las funciones correspondientes
Si devuelve true, significa que el proceso consola debe terminar
Si devuelve false, significa que debe seguir recibiendo un nuevo comando
*/

static bool procesar_comandos() {

    char buffer[BUFFER_SIZE];
    size_t buffer_length;
    char *res, *first_param, *second_param;

    // Mi mamá no me deja usar gets :(
    res = fgets(buffer, sizeof(buffer), stdin);

    // Permitimos salir con EOF
    if (res==NULL)
        return true;

    buffer_length = strlen(buffer);
    // Si es un ENTER, continuamos
    if (buffer_length<=1)
        return false;

    // Sacamos último carácter
    buffer[buffer_length-1] = '\0';

    // Obtenemos el primer parámetro
    first_param = strtok(buffer, " ");

    if (strncmp(first_param, CMD_QUIT, sizeof(CMD_QUIT))==0 ||
        strncmp(first_param, CMD_SQUIT, sizeof(CMD_SQUIT))==0) {

        quit();
        printf("Adios!\n");
        return true;
    }

    if (strncmp(first_param, CMD_MAXIMUM, sizeof(CMD_MAXIMUM))==0) {
        maximum();
        return false;
    }

    // Obtenemos el segundo parámetro
    second_param = strtok(NULL, " ");
    if (strncmp(first_param, CMD_MEMBER, sizeof(CMD_MEMBER))==0) {
        if (second_param != NULL) {
            string s(second_param);
            member(s);
        }
        else {
            printf("Falta un parámetro\n");
        }
        return false;
    }

    if (strncmp(first_param, CMD_ADD, sizeof(CMD_ADD))==0) {
        if (second_param != NULL) {
            string s(second_param);
            addAndInc(s);
        }
        else {
            printf("Falta un parámetro\n");
        }
        return false;
    }

    if (strncmp(first_param, CMD_LOAD, sizeof(CMD_LOAD))==0) {
        list<string> params;
        while (second_param != NULL)
        {
            string s(second_param);
            params.push_back(s);
            second_param = strtok(NULL, " ");
        }

        load(params);
        return false;
    }

    printf("Comando no reconocido");
    return false;
}

void consola(unsigned int np_param) {
    np = np_param;
    printf("Comandos disponibles:\n");
    printf("  "CMD_LOAD" <arch_1> <arch_2> ... <arch_n>\n");
    printf("  "CMD_ADD" <string>\n");
    printf("  "CMD_MEMBER" <string>\n");
    printf("  "CMD_MAXIMUM"\n");
    printf("  "CMD_SQUIT"|"CMD_QUIT"\n");

    bool fin = false;
    while (!fin) {
        printf("> ");
        fflush(stdout);
        fin = procesar_comandos();
    }
}
