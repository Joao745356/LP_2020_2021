//
// Created by joao7 on 08/01/2023.
//

#ifndef UNTITLED3_FURNITURE_H
#define UNTITLED3_FURNITURE_H

#include <stdio.h>
#include<stdlib.h>
#include <string.h>


#define PRODUCT_CODE_SIZE 8
#define PRODUCT_NAME_SIZE 80

#define MAX_MATERIAL_AMOUNT      30
#define MAX_CODE                 10
#define MAX_DESCRIPTION          350


#define DESCRIPTION_ERROR_TOOBIG     "A descriçao excede a quantidade permitida de palavras(caracteres actually)"
#define CODE_ERROR                   "ahhh, estavas à procura de um erro? nice try."
#define CODE_ERROR_ALREADY_GIVEN     "este código já foi atribuido"

typedef struct {
    int height;
    int depth;
    int length;
} Measurements;

typedef struct {
    char description[PRODUCT_NAME_SIZE];
    char materialCode[PRODUCT_CODE_SIZE];
    int quantity;
    int isPAR;
} Material_Quantity;

typedef enum {
    SALA,
    COZINHA,
    WC,
    QUARTO,
} divisao_t;

typedef struct {
    char furtnitureCode[PRODUCT_CODE_SIZE];
    char name[PRODUCT_NAME_SIZE];
    divisao_t divisao;
    float price;
    array materials; // sizeof(Material_Quantity)
    int isActive;
    Measurements furniture_measurements;
} Furniture;
#endif //UNTITLED3_FURNITURE_H
