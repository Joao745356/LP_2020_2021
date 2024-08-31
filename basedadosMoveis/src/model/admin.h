//
// Created by joao7 on 08/01/2023.
//

#ifndef UNTITLED3_ADMIN_H
#define UNTITLED3_ADMIN_H


#include <stdio.h>
#include<stdlib.h>
#include <string.h>

#define MAX_ADMIN_NAME      90
#define PASSWORD_SIZE       20
#define MAX_ADMIN_RESIDENCY 100
#define ERR_ADMIN_ALREXIST "O admin já existe"
#define MSG_OBTAIN_ADMIN_NUM "Insira o numero do administrador: "


#define MSG_OBTAIN_ADMIN_NAME           "Insira o nome do administrador: "
#define MSG_OBTAIN_ADMIN_RESI           "Insira a morada do administrador: "
#define MSG_OBTAIN_PASSWORD             "Por favor digite a password desejada : "


typedef struct {
    int year,
            month,
            day;
} ContractDay;  //struct que pertence a Administrador qd começou a trabalhar.


typedef struct {
    int adminnumber; //numero de trabalhador
    char name[MAX_ADMIN_NAME];//nome do admin
    char residency[MAX_ADMIN_RESIDENCY]; // maybe n interessa
    char password[PASSWORD_SIZE]; // requerida ao entrar no menu admmin
    long int phoneNumber;
    ContractDay day_contract;
} Admin;

struct CompanyOwner {
    char name[MAX_ADMIN_NAME];
    char residency[MAX_ADMIN_RESIDENCY];
    char password[PASSWORD_SIZE];
    long int PhoneNumber;
};                            //main menu a opçao 78069 abre o menu especial para o dono que permite adicionar e remover admins

/*
struct CompanyOwner dono =
        {
                .name = "Pedro António Santos Ribeiro",
                .residency = "Na loja",
                .password = "Fz4KmEr",
                .phoneNumber = 938255284,
        }; // unico

Admin filhaDono =
        {
                .name = "Carlota Santos Ribeiro",
                .day_contract.day = 03, .day_contract.month = 02, .day_contract.year = 2012,
                .residency = "Rua Pereira, entrada 24",
                .password = "estrela245",
                .phoneNumber = 912242510,
        }; //fixed Admin

*/
#endif //UNTITLED3_ADMIN_H
