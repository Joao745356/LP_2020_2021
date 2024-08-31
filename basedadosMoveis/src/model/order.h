//
// Created by joao7 on 08/01/2023.
//

#ifndef UNTITLED3_ORDER_H
#define UNTITLED3_ORDER_H

#include "furniture.h"
#include "client.h"

#define ORDER_CODE_SIZE   10


typedef struct {
    int Dday;
    int Dmonth;
    int Dyear;
} deliveryday;

#define ORDER_SIZE 100


typedef struct Encomenda_s {
    char orderCode[ORDER_CODE_SIZE];
    int clientNumber;
    array products; // sizeof(char[PRODUCT_CODE_SIZE])
    deliveryday makeOrderDay; ///< Indica o dia em que foi requesitada a encomenda
    deliveryday deliveryDay;
    float price;
    int isActive; ///< Se ja passou o prazo de entrega ou nao
} Order;

#endif //UNTITLED3_ORDER_H
