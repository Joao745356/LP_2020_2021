//
// Created by joao7 on 08/01/2023.
//

#ifndef UNTITLED3_CLIENT_H
#define UNTITLED3_CLIENT_H


#include <stdio.h>
#include<stdlib.h>
#include <string.h>

#define ERR_CLIENT_ALREXIST       "O número de cliente já se encontra atribuído."

#define MAX_PASSWORD              15
#define MSG_OBTAIN_CLIENT_NUM     "Insira o numero de cliente : "

#define MAX_CLIENT_NAME           60
#define MAX_RESIDEN               90
#define MSG_OBTAIN_NAME           "Insira o nome do cliente: "
#define MSG_OBTAIN_RESI           "Insira a morada do cliente: "
#define MSG_OBTAIN_NIF            "Insira o NIF do cliente : "
#define MSG_OBTAIN_PASSWORD       "Por favor digite a password desejada : "
#define MSG_NUM_TELEMOVEL         "Por favor introduza o numero de telemóvel do cliente : "

#define NIF_MIN 100000000
#define NIF_MAX 999999999

typedef struct {
    int year,
            month,
            day;
} RegisterDay;                  //struct que dá a data em que se regista o cliente. ( ou outra coisa qq );

typedef struct {
    int number;                  //numero de cliente
    char name[MAX_CLIENT_NAME];
    char residency[MAX_RESIDEN];
    int nif;
    char password[MAX_PASSWORD];
    long int phoneNumber;
    RegisterDay register_day_cliente;    //data em que foram registados no sistema
    int isActive;         // cliente ativo ou desativo
} ClientProfile;

#endif //UNTITLED3_CLIENT_H
