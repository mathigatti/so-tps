#ifndef _NODO_H
#define _NODO_H

#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>

using namespace std;


#define BUFFER_SIZE 1024


#define TAG_QUIT 0
#define TAG_LOAD 1
#define TAG_ADDANDINC 2
#define TAG_MEMBER 3
#define TAG_MAXIMUM 4
#define TAG_MAXIMUM_WORD 5
#define TAG_MAXIMUM_END 6
#define TAG_ADDANDINC_ACK 7
#define TAG_LOAD_FIN 8

#define CONSOLE_RANK 0

/* Función que maneja un nodo.
 * Recibe el rank del nodo.
 */
void nodo(unsigned int rank);

/* Simula un tiempo de procesamiento no determinístico.
 */
void trabajarArduamente();

bool nodeQuit();
bool nodeLoad(HashMap &h);
bool nodeMaximum(HashMap &h);
bool nodeMember(char msg[], HashMap &h);
bool nodeAddAndInc(char msg[], HashMap &h);

#endif  /* _NODO_H */
