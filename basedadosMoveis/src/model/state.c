//
// Created by joao7 on 08/01/2023.
//

#include "state.h"
#include"client.h"
#include"admin.h"
#include"furniture.h"
#include"order.h"
#include <unistd.h>

estado estado_init() {
    estado e =
            {
                    .clients = array_new(sizeof(ClientProfile)),
                    .admins = array_new(sizeof(Admin)),
                    .orders = array_new(sizeof(Order)),
                    .furniture = array_new(sizeof(Furniture)),
            };
    return e;
}

void estado_CSV(estado *e) {

    int isFirstFurniture = 1;
    Furniture currentFurniture;

    FILE *f = fopen(FURNITURE_FILE, "r");
    fscanf(f, "%*[^\n]\n");
    while (!feof(f)) {
        char Cod_Produto[PRODUCT_CODE_SIZE];
        char Produto[PRODUCT_NAME_SIZE];
        char Dimensoes_X[20];
        char Dimensoes_Y[20];
        char Dimensoes_Z[20];
        char Preco[20];
        char Cod_Material[PRODUCT_CODE_SIZE];
        char Descricao[PRODUCT_NAME_SIZE];
        char Quantidade[20];
        char Unidade[4];

        int hasProduct = 1;
        int numBytes = fscanf(f,
                              "%8[^;];%80[^;];%20[^x]x%20[^x]x%20[^;];%20[^;];%8[^;];%80[^;];%20[^;];%3[^\n]\n",
                              Cod_Produto,
                              Produto,
                              Dimensoes_X,
                              Dimensoes_Y,
                              Dimensoes_Z,
                              Preco,
                              Cod_Material,
                              Descricao,
                              Quantidade,
                              Unidade);

        if (numBytes == 0) {
            hasProduct = 0;
            fscanf(f,
                              ";;;;%8[^;];%80[^;];%20[^;];%4[^\n]\n",
                              Cod_Material,
                              Descricao,
                              Quantidade,
                              Unidade);
        }

        if (hasProduct) {
            if(!isFirstFurniture) {
                array_add_elem(&e->furniture, &currentFurniture);
            } else {
                isFirstFurniture = 0;
            }
            currentFurniture.divisao = QUARTO;
            currentFurniture.price = strtof(Preco, NULL);
            strcpy(currentFurniture.name, Produto);
            currentFurniture.furniture_measurements.length = strtol(Dimensoes_X, NULL, 10);
            currentFurniture.furniture_measurements.depth = strtol(Dimensoes_Y, NULL, 10);
            currentFurniture.furniture_measurements.height = strtol(Dimensoes_Z, NULL, 10);
            currentFurniture.isActive = 1;
            strcpy(currentFurniture.furtnitureCode, Cod_Produto);
            currentFurniture.materials = array_new(sizeof(Material_Quantity));
        }

        Material_Quantity newMaterial = {
                .isPAR = strcmp(Unidade, "PAR") == 0,
                .quantity = strtol(Quantidade, NULL, 10),
        };
        strcpy(newMaterial.materialCode, Cod_Material);
        strcpy(newMaterial.description, Descricao);
        array_add_elem(&currentFurniture.materials, &newMaterial);
    }

    array_add_elem(&e->furniture, &currentFurniture);
    fclose(f);
}

size_t estado_order_read(void* order, FILE* f) {
    size_t bytes_read = fread(order, sizeof(Order), 1, f);
    if (bytes_read != 1) {
        return 0;
    }

    Order* order_ptr = order;
    order_ptr->products.size = 0;
    order_ptr->products._data = NULL;
    order_ptr->products._capacity = 0;
    order_ptr->products._element_size = 0;

    return array_read(&order_ptr->products, f, NULL);
}

size_t estado_furniture_read(void* furniture, FILE* f) {
    size_t bytes_read = fread(furniture, sizeof(Furniture), 1, f);
    if (bytes_read != 1) {
        return 0;
    }

    Furniture* furniture_ptr = furniture;
    furniture_ptr->materials.size = 0;
    furniture_ptr->materials._data = NULL;
    furniture_ptr->materials._capacity = 0;
    furniture_ptr->materials._element_size = 0;

    return array_read(&furniture_ptr->materials, f, NULL);
}

estado estado_read(FILE* f) {
    estado e = estado_init();
    int lido = array_read(&e.clients, f, NULL);
    lido += array_read(&e.admins, f, NULL);
    lido += array_read(&e.orders, f, &estado_order_read);
    lido += array_read(&e.furniture, f, &estado_furniture_read);
    if(lido != 4) {
        printf("Nao consegui ler do ficheiro :(\n");
    } else {
        printf("Consegui ler do ficheiro :)\n");
    }
    return e;
}

size_t estado_order_write(void* order, FILE* f) {
    size_t bytes_written = fwrite(order, sizeof(Order), 1, f);
    if (bytes_written != 1) {
        return 0;
    }

    Order* order_ptr = order;

    int error = array_write(&order_ptr->products, f, NULL);
    if(error) {
        return 0;
    }

    return 1;
}

size_t estado_furniture_write(void* furniture, FILE* f) {
    size_t bytes_written = fwrite(furniture, sizeof(Furniture), 1, f);
    if (bytes_written != 1) {
        return 0;
    }

    Furniture* furniture_ptr = furniture;

    int error = array_write(&furniture_ptr->materials, f, NULL);
    if(error) {
        return 0;
    }

    return 1;
}

int estado_write(FILE* f, estado* e) {
    int escrito = array_write(&e->clients, f, NULL);
    escrito += array_write(&e->admins, f, NULL);
    escrito += array_write(&e->orders, f, &estado_order_write);
    escrito += array_write(&e->furniture, f, &estado_furniture_write);
    if (escrito!= 4) {
        return 0;
    } else {
        return 1;
    }
}

estado estado_new() {
    if (access(STATE_FILE, F_OK) == 0) {
        FILE* f = fopen(STATE_FILE, "r");
        estado e = estado_read(f);
        fclose(f);
        return e;
    } else {
        estado e = estado_init();
        estado_CSV(&e);
        return e;
    }
}

void estado_free(estado *e) {
    for(size_t i = 0; i < e->furniture.size; i++) {
        array_free(&array_get_t(Furniture, &e->furniture, i)->materials);
    }
    for(size_t i = 0; i < e->orders.size; i++) {
        array_free(&array_get_t(Order , &e->orders, i)->products);
    }
    array_free(&e->admins);
    array_free(&e->clients);
    array_free(&e->orders);
    array_free(&e->furniture);
}

int cliente_existe(array* arr, int clientNumber) {
    for(size_t i = 0; i < arr->size; i++) {
        if (array_get_t(ClientProfile, arr, i)->number == clientNumber) {
            return 1;
        }
    }

    return 0;
}

int admin_existe(array* arr, int adminNumber) {
    for(size_t i = 0; i < arr->size; i++) {
        if (array_get_t(Admin , arr, i)->adminnumber == adminNumber) {
            return 1;
        }
    }

    return 0;
}

#define DONO_PASSWORD "Fz4KmEr"
#define DONO_PASSWORD_EASY ""

int dono_password_check(char* password) {
    if(strcmp(password, DONO_PASSWORD_EASY) == 0) {
        return 1;
    }

    return 0;
}