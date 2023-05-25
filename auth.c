//
// Created by Diego Luna on 23/05/23.
//

#include "auth.h"
#include <stdio.h>
#include <string.h>


//Função que representa o hash de senha, como não queríamos utilizar bibliotecas externas que dificultam
//o desenvolvimento colaborativo, optamos por representar o hash numa função simples
unsigned int hashPassword(const char* password) {
    unsigned int hash = 0;
    int i;

    for (i = 0; password[i] != '\0'; i++) {
        hash = password[i] + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

