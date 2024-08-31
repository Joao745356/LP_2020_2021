//
// Created by joao7 on 08/01/2023.
//

#ifndef UNTITLED3_STATE_H
#define UNTITLED3_STATE_H

#include"../util/array.h"

#define STATE_FILE "C:\\Users\\joao7\\CLionProjects\\untitled3\\state.bin"
#define FURNITURE_FILE "C:\\Users\\joao7\\CLionProjects\\untitled3\\furniture.csv"
#define MATERIALS_FILE "C:\\Users\\joao7\\CLionProjects\\untitled3\\materials.csv"
//#define MATERIALS_FILE "materials.csv" coloco o CSV na mesma pasta q corro o executável

typedef struct {
    array clients;
    array orders;
    array furniture;
    array admins;
} estado;

estado estado_new();
void estado_free(estado *e);
int estado_write(FILE* f, estado* e);
int cliente_existe(array* arr, int clientNumber);
int admin_existe(array* arr, int adminNumber);
int dono_password_check(char* password);
size_t estado_furniture_write(void* furniture, FILE* f);
size_t estado_order_write(void* order, FILE* f);
estado estado_read(FILE* f);
size_t estado_furniture_read(void* furniture, FILE* f);
size_t estado_order_read(void* order, FILE* f);
void estado_CSV(estado *e);
estado estado_init();

#endif //UNTITLED3_STATE_H
