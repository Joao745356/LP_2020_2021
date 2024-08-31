//
// Created by joao7 on 08/01/2023.
//

#include "main_menus.h"
#include "../util/menu_manager.h"
#include "../model/state.h"
#include "../model/furniture.h"
#include "../util/input.h"
#include "../model/client.h"
#include "../model/order.h"
#include "../model/admin.h"
#include <float.h>
#include <time.h>

typedef struct
{
    void* ptr1;
    void* ptr2;
} two_ptrs;

int adminExists(array* admins, int number, char* password){

    for(size_t i = 0; i < admins->size; i++){
        Admin* admin = array_get_t(Admin, admins, i);
        if(number == admin->adminnumber)
        {
            return strcmp(password, admin->password) == 0;
        }
    }
    return 0;
}


int escreverParaFicheiro(void *userData) {
    estado *state = (estado *) userData;
    FILE* f = fopen(STATE_FILE, "w");
    int erro = estado_write(f, state);
    if(erro) {
        printf("Nao consegui escrever no ficheiro :(\n");
    } else {
        printf("Consegui escrever no ficheiro :)\n");
    }
    fclose(f);
}

int displayMenuPrincipal(void *userData) {
    menuEntry entries[] = {
            {"Admin Menu",     &displayMenuAdmin, userData},
            {"Dono Menu", &displayMenuDono, userData},
            {"Cliente Menu",   &displayMenuCliente, userData},
            {"Guardar sessao", &escreverParaFicheiro, userData},
    };
    go_to_menu(4, "Móveis for all", entries);
}

int displayMenuDono(void *userData) {
    estado *state = (estado *) userData;
    char password[80];
    readString(password, 80, "inserir password\n");
    if (!dono_password_check(password)) {
        printf("password errada\n");
        return MENU_ACTION_REPRINT;
    }

    menuEntry entries[] = {
            {"Adicionar administrador",   &add_admin, &state->admins},
            {"Remover administrador",   &remove_admin,   &state->admins},
            {"Atualizar Administrador", &update_admin,   &state->admins},
            {"Listar Administradores", &mostrar_admins, &state->admins},
    };
    return go_to_menu(4, "Owner menu", entries);
}

int displayMenuAdmin(void *userData) {

    estado *state = (estado *) userData;

    int number = obtainInt(0,1000, "Por favor insira o seu numero de funcionário.");
    char password[MAX_PASSWORD];
    readString(password, MAX_PASSWORD, "Por favor introduza a sua password");

    if(!adminExists(&state->admins, number, password)){
        puts("numero ou password inválidos.");
        return MENU_ACTION_REPRINT;
    }

    menuEntry entries[] = {
            {"Gerir produtos",   &displayProductManagement, &state->furniture},
            {"Gerir clientes",   &displayClientMangement,   &state->clients},
            {"Gerir encomendas", &displayOrderManagement,   state},
            {"Listar encomendas", &displayorders, state},
            {"Listar clientes", &mostrar_clients, &state->clients},
            {"Exportar materiais encomendas",&export_materials,state},
    };
    return go_to_menu(6, "Admin menu", entries);
}

int datecmp(deliveryday a, deliveryday b) {
    if(a.Dyear != b.Dyear) {
        return b.Dyear - a.Dyear;
    }

    if(a.Dmonth != b.Dmonth) {
        return b.Dmonth - a.Dmonth;
    }

    return b.Dday - a.Dday;
}

int datebetween(deliveryday start, deliveryday end, deliveryday mid) {
    return datecmp(start, mid) >= 0 && datecmp(mid, end) >= 0;
}

int export_materials(void* userData){
    estado *state = (estado *) userData;
    deliveryday date_start;
    deliveryday date_end;
    int correctDates;
    do {
        correctDates = 1;
        date_start.Dday = obtainInt(1, 31, "inserir dia de inicio de procura");
        date_start.Dmonth = obtainInt(1, 12, "inserir mes de inicio de procura");
        date_start.Dyear = obtainInt(2023, INT_MAX, "inserir ano de inicio de procura");
        date_end.Dday = obtainInt(1, 31, "inserir dia de fim de procura");
        date_end.Dmonth = obtainInt(1, 12, "inserir mes de fim de procura");
        date_end.Dyear = obtainInt(2023, INT_MAX, "inserir ano de fim de procura");
        if(datecmp(date_start, date_end) < 0) {
            correctDates = 0;
            puts("erro: data de inicio e depois de data de fim");
        }
    } while (!correctDates);

    // Construir array de apontadores para todas as encomendas criadas entre as datas
    array ordersFound = array_new(sizeof(Order*));
    for(size_t i = 0; i < state->orders.size; i++) {
        Order * order = array_get_t(Order, &state->orders, i);
        if(datebetween(date_start, date_end, order->makeOrderDay)) {
            array_add_elem(&ordersFound, &order);
        }
    }

    array materialQuantities = array_new(sizeof(Material_Quantity));
    for(size_t i = 0; i < ordersFound.size; i++) {
        Order * order = *array_get_t(Order*, &ordersFound, i);
        for (size_t productCode_i = 0; productCode_i < order->products.size; ++productCode_i) {
            char* productCode = array_get_t(char, &order->products, productCode_i);
            int found = 0;
            for(size_t product_i = 0; product_i < state->furniture.size; product_i++) {
                Furniture* elementFurniture = array_get_t(Furniture, &state->furniture, product_i);
                if(strcmp(productCode, elementFurniture->furtnitureCode) == 0) {
                    found = 1;

                    for(size_t materialQuantity_i = 0; materialQuantity_i < elementFurniture->materials.size; materialQuantity_i++) {
                        Material_Quantity* quantityCurrent = array_get_t(Material_Quantity, &elementFurniture->materials, materialQuantity_i);

                        int foundQuantityCounter = 0;
                        for(size_t existingMQ_i = 0; existingMQ_i < materialQuantities.size; existingMQ_i++) {
                            Material_Quantity* quantityCounter = array_get_t(Material_Quantity, &materialQuantities, existingMQ_i);
                            if(strcmp(quantityCounter->materialCode, quantityCurrent->materialCode) == 0) {
                                foundQuantityCounter = 1;
                                quantityCounter->quantity += quantityCurrent->quantity;
                                break;
                            }
                        }
                        if(!foundQuantityCounter){
                            Material_Quantity newMQ = {
                                .quantity = quantityCurrent->quantity,
                                .isPAR = quantityCurrent->isPAR,
                            };
                            strcpy(newMQ.materialCode, quantityCurrent->materialCode);
                            strcpy(newMQ.description, quantityCurrent->description);
                            array_add_elem(&materialQuantities, &newMQ);
                        }
                    }
                    break;
                }
            }
            if(!found){
                printf("product %s [info not found]\n", productCode);
            }
        }
    }

    FILE* f = fopen(MATERIALS_FILE, "w");

    fprintf(f, "MATERIAL_CODE, DESCRIPTION, IS_PAR, QUANTITY\n");

    for (size_t mQ = 0; mQ < materialQuantities.size; ++mQ) {
        Material_Quantity* q = array_get_t(Material_Quantity, &materialQuantities, mQ);
        fprintf(f, "%s, %s, %d, %d\n", q->materialCode, q->description, q->isPAR, q->quantity);
    }

    fclose(f);
    array_free(&materialQuantities);
    array_free(&ordersFound);

    return MENU_ACTION_REPRINT;
}

typedef struct {
    void *ptr;
    size_t idx;
} ptr_and_index;

int add_product(void *userData) {
    array *furniture = (array *) userData;
    Furniture newProduct = {};
    readString(newProduct.furtnitureCode, PRODUCT_CODE_SIZE, "inserir o codigo do produto");
    readString(newProduct.name, PRODUCT_NAME_SIZE, "inserir nome de produto");
    newProduct.divisao = obtainInt(0,3,"insira a divisao\n 0 - SALA\n 1 - COZINHA\n 2 - WC\n 3 - Quarto");
    newProduct.price = obtainFloat(0 ,FLT_MAX,"Introduza o valor do produto");
    newProduct.isActive = 1;
    newProduct.furniture_measurements.height = obtainInt(0, 5000, "Insira a altura em centimetros do produto");
    newProduct.furniture_measurements.depth = obtainInt(0, 5000, "Insira o comprimento em centimetros do produto");
    newProduct.furniture_measurements.length = obtainInt(0, 5000, "Insira a largura em centimetros do produto");
    newProduct.materials = array_new(sizeof(Material_Quantity));
    while(obtainInt(0,1,"gostaria de adicionar materiais?") == 1){
        Material_Quantity material = {};
        readString(material.materialCode, PRODUCT_CODE_SIZE, " Insira o codigo de material");
        readString(material.description, PRODUCT_NAME_SIZE, " Insira o nome de material");
        material.quantity = obtainInt(0,5000, "Insira a quantidade do material mencionado");
        material.isPAR = obtainInt(0,1,  "e par? 0 - nao \n1 - sim");
        array_add_elem(&newProduct.materials, &material);
    }
    array_add_elem(furniture, &newProduct);
    return MENU_ACTION_REPRINT;
}

int update_product_action(void *userData) {
    ptr_and_index *ptrIdx = (ptr_and_index *) userData;
    array *furniture = (array *) ptrIdx->ptr;
    Furniture *element = array_get_t(Furniture, furniture, ptrIdx->idx);
    readString(element->name, PRODUCT_NAME_SIZE, "inserir nome de produto");
    return MENU_ACTION_REPRINT;
}

int update_product(void *userData) {
    array *furniture = (array *) userData;

    // Construir menu
    array entries = array_new_with_capacity(sizeof(menuEntry), furniture->size);
    array ptrIdxs = array_new_with_capacity(sizeof(ptr_and_index), furniture->size);
    for (size_t i = 0; i < furniture->size; i++) {
        ptr_and_index ptrIdx = {furniture, i};
        array_add_elem(&ptrIdxs, &ptrIdx);
        menuEntry entry = {
                .name = array_get_t(Furniture, furniture, i)->name,
                .menuEntry = &update_product_action,
                .userData = array_get(&ptrIdxs, i),
        };
        array_add_elem(&entries, &entry);
    }

    go_to_menu(furniture->size, "Atualizar Produto", entries._data);
    array_free(&ptrIdxs);
    array_free(&entries);
    return MENU_ACTION_REPRINT;
}

int remove_furniture_action(void *userData) {
    ptr_and_index *ptrIdx = (ptr_and_index *) userData;
    array *arr = (array *) ptrIdx->ptr;
    array_free(&array_get_t(Furniture, arr, ptrIdx->idx)->materials);
    array_remove(arr, ptrIdx->idx);
    return MENU_ACTION_BACK;
}

int remove_product(void *userData) {
    array *furniture = (array *) userData;

    // Construir menu
    array entries = array_new_with_capacity(sizeof(menuEntry), furniture->size);
    array ptrIdxs = array_new_with_capacity(sizeof(ptr_and_index), furniture->size);
    for (size_t i = 0; i < furniture->size; i++) {
        ptr_and_index ptrIdx = {furniture, i};
        array_add_elem(&ptrIdxs, &ptrIdx);
        menuEntry entry = {
                .name = array_get_t(Furniture, furniture, i)->name,
                .menuEntry = &remove_furniture_action,
                .userData = array_get(&ptrIdxs, i),
        };
        array_add_elem(&entries, &entry);
    }

    go_to_menu(furniture->size, "Remover Produto", entries._data);
    array_free(&ptrIdxs);
    array_free(&entries);
    return MENU_ACTION_REPRINT;
}

int displayProductManagement(void *userData) {
    menuEntry entries[] = {
            {"Adicionar produtos", &add_product,    userData},
            {"Atualizar produtos", &update_product, userData},
            {"Apagar produto",     &remove_product, userData},
    };
    return go_to_menu(3, "Móveis for all", entries);
}

int selecionar_cliente(array *clientes, int(*action)(void*)) {
    // Construir menu
    array entries = array_new_with_capacity(sizeof(menuEntry), clientes->size);
    array ptrIdxs = array_new_with_capacity(sizeof(ptr_and_index), clientes->size);
    for (size_t i = 0; i < clientes->size; i++) {
        ptr_and_index ptrIdx = {clientes, i};
        array_add_elem(&ptrIdxs, &ptrIdx);
        menuEntry entry = {
                .name = array_get_t(ClientProfile, clientes, i)->name,
                .menuEntry = action,
                .userData = array_get(&ptrIdxs, i),
        };
        array_add_elem(&entries, &entry);
    }

    int action_ret = go_to_menu(clientes->size, "Remover Clientes", entries._data);
    array_free(&ptrIdxs);
    array_free(&entries);
    return action_ret;
}
int selecionar_admin(array *admins, int(*action)(void*))
{
    array entries = array_new_with_capacity(sizeof(menuEntry), admins->size);
    array ptrIdxs = array_new_with_capacity(sizeof(ptr_and_index), admins->size);
    for(size_t i = 0; i < admins->size;i++)
    {
        ptr_and_index  ptrIdx = {admins, i};
        array_add_elem(&ptrIdxs,&ptrIdx);
        menuEntry  entry = {
                .name = array_get_t(Admin, admins, i)->name,
                .menuEntry = action,
                .userData = array_get(&ptrIdxs, i),
        };
        array_add_elem(&entries, &entry);
    }
    int action_ret = go_to_menu(admins->size, "Remover Admins", entries._data);
    array_free(&ptrIdxs);
    array_free(&entries);
    return action_ret;
    }


int add_client(void *userData){

    array *clients = (array *) userData;
    ClientProfile newclient = {};
    while (cliente_existe(clients, newclient.number = obtainInt(1,1000,MSG_OBTAIN_CLIENT_NUM)))  {
        puts(ERR_CLIENT_ALREXIST);
    }
    readString(newclient.name, MAX_CLIENT_NAME, MSG_OBTAIN_NAME);
    readString(newclient.residency, MAX_RESIDEN, MSG_OBTAIN_RESI);
    newclient.nif = obtainInt(NIF_MIN,NIF_MAX,  MSG_OBTAIN_NIF);
    readString(newclient.password, MAX_PASSWORD, MSG_OBTAIN_PASSWORD);
    newclient.phoneNumber = obtainLongInt(910000000, 969999999, MSG_NUM_TELEMOVEL);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    newclient.register_day_cliente.day =  tm.tm_mday;
    newclient.register_day_cliente.month =  tm.tm_mon+1;
    newclient.register_day_cliente.year =  tm.tm_year+1900;
    newclient.isActive = 1;
    array_add_elem(clients, &newclient);

    return MENU_ACTION_REPRINT;
}

int update_client_action(void *userData)
{
    ptr_and_index *ptrIdx = (ptr_and_index *) userData;
    array *arr = (array *) ptrIdx->ptr;
    size_t idx = ptrIdx->idx;
    ClientProfile *curr = array_get_t(ClientProfile, arr, idx);
    readString(curr->name, MAX_CLIENT_NAME, "insira o nome do cliente");
    readString(curr->residency, MAX_RESIDEN, "insira a morada do cliente");
    curr->nif = obtainInt(NIF_MIN,NIF_MAX, "insira o nif do cliente");
    readString(curr->password, MAX_PASSWORD, "Qual a password do cliente ? ");
    curr->phoneNumber = obtainLongInt(910000000, 969999999, "Qual o numero de telemovel do cliente?");
    curr->isActive = obtainInt(0,1, "O cliente ficou inativo? \n 0 - sim \n 1 - não");
    return MENU_ACTION_BACK;
}

int update_client(void *userData)
{
    array *clientes = (array *) userData;
    selecionar_cliente(clientes, &update_client_action);
    return MENU_ACTION_REPRINT;
}

int remove_client_action(void *userData) {
    ptr_and_index *ptrIdx = (ptr_and_index *) userData;
    array *arr = (array *) ptrIdx->ptr;
    array_remove(arr, ptrIdx->idx);
    return MENU_ACTION_BACK;
}

int remove_client_menu(void *userData) {  //display current clients + options
    array *clientes = (array *) userData;
    selecionar_cliente(clientes, &remove_client_action);
    return MENU_ACTION_REPRINT;
}

int displayClientMangement(void *userData) {
    menuEntry entries[] = {
            {"Adicionar cliente", &add_client, userData},
            {"Atualizar cliente", &update_client, userData},
            {"Apagar cliente",    &remove_client_menu, userData},
    };
    return go_to_menu(3, "Móveis for all", entries);
}


int do_nothing(void *userData) {
    return MENU_ACTION_BACK;
}

int update_order_action(void *userData) {
    ptr_and_index *ptrIdx = (ptr_and_index *) userData;
    estado *state = (estado *) ptrIdx->ptr;
    Order* orderToEdit = array_get_t(Order, &state->orders, ptrIdx->idx);

    int entregue = obtainInt(0, 1, "a encomenda foi entregue?\n0- nao\n1 - sim");
    if (entregue) {
        time_t y = time(NULL);
        struct tm ym = *localtime(&y);
        orderToEdit->deliveryDay.Dday =  ym.tm_mday;
        orderToEdit->deliveryDay.Dmonth = ym.tm_mon+1;
        orderToEdit->deliveryDay.Dyear =  ym.tm_year+1900;
        orderToEdit->isActive = 0;
    } else {
        orderToEdit->isActive = 1;
        orderToEdit->price = 0;
        array_free(&orderToEdit->products);

        array entries = array_new_with_capacity(sizeof(menuEntry), state->furniture.size + 1);
        array twoPtrs = array_new_with_capacity(sizeof(two_ptrs), state->furniture.size);
        for (size_t i = 0; i < state->furniture.size; i++) {
            two_ptrs twoPtr = {orderToEdit, array_get_t(Furniture, &state->furniture, i)};
            array_add_elem(&twoPtrs, &twoPtr);
            menuEntry entry = {
                    .name = array_get_t(Furniture, &state->furniture, i)->name,
                    .menuEntry = &add_furniture_to_order_action,
                    .userData = array_get(&twoPtrs, i),
            };
            array_add_elem(&entries, &entry);
        }

        two_ptrs ptrs = {orderToEdit, state};
        menuEntry entry = {
                .name = "finalizar encomenda",
                .menuEntry = &do_nothing,
        };
        array_add_elem(&entries, &entry);

        go_to_menu(entries.size, "Escolher Produtos Da Encomenda", entries._data);
        array_free(&twoPtrs);
        array_free(&entries);
    }
    return MENU_ACTION_BACK;

}

int update_order(void *userData) {
    estado *state = (estado *) userData;
    array *orders = &state->orders;

    // Construir menu
    array entries = array_new_with_capacity(sizeof(menuEntry), orders->size);
    array ptrIdxs = array_new_with_capacity(sizeof(ptr_and_index), orders->size);
    for (size_t i = 0; i < orders->size; i++) {
        ptr_and_index ptrIdx = {state, i};
        array_add_elem(&ptrIdxs, &ptrIdx);
        menuEntry entry = {
                .name = array_get_t(Order, orders, i)->orderCode,
                .menuEntry = &update_order_action,
                .userData = array_get(&ptrIdxs, i),
        };
        array_add_elem(&entries, &entry);
    }

    go_to_menu(orders->size, "Editar Encomendas", entries._data);
    array_free(&ptrIdxs);
    array_free(&entries);
    return MENU_ACTION_BACK;
}

int remove_order_action(void *userData) {
    ptr_and_index *ptrIdx = (ptr_and_index *) userData;
    array *arr = (array *) ptrIdx->ptr;
    array_free(&array_get_t(Order, arr, ptrIdx->idx)->products);
    array_remove(arr, ptrIdx->idx);
    return MENU_ACTION_BACK;
}

int remove_order(void *userData) {
    array *orders = (array *) userData;

    // Construir menu
    array entries = array_new_with_capacity(sizeof(menuEntry), orders->size);
    array ptrIdxs = array_new_with_capacity(sizeof(ptr_and_index), orders->size);
    for (size_t i = 0; i < orders->size; i++) {
        ptr_and_index ptrIdx = {orders, i};
        array_add_elem(&ptrIdxs, &ptrIdx);
        menuEntry entry = {
                .name = array_get_t(Order, orders, i)->orderCode,
                .menuEntry = &remove_order_action,
                .userData = array_get(&ptrIdxs, i),
        };
        array_add_elem(&entries, &entry);
    }

    go_to_menu(orders->size, "Remover Encomendas", entries._data);
    array_free(&ptrIdxs);
    array_free(&entries);
    return MENU_ACTION_REPRINT;
}

int     displayOrderManagement(void *userData) {
    estado *state = (estado *) userData;
    menuEntry entries[] = {
            {"Adicionar encomenda", &new_order, state},
            {"Atualizar encomenda", &update_order, state},
            {"Apagar encomenda",    &remove_order, &state->orders},
    };
    return go_to_menu(3, "Móveis for all", entries);
}

int add_admin(void *userData) {
    array *admins = (array *) userData;
    Admin newadmin = {};
    while (admin_existe(admins, newadmin.adminnumber = obtainInt(1,1000,MSG_OBTAIN_ADMIN_NUM)))  {
        puts(ERR_ADMIN_ALREXIST);
    }
    readString(newadmin.name,MAX_ADMIN_NAME,MSG_OBTAIN_ADMIN_NAME);
    readString(newadmin.residency,MAX_ADMIN_RESIDENCY,MSG_OBTAIN_ADMIN_RESI);
    readString(newadmin.password,PASSWORD_SIZE,MSG_OBTAIN_PASSWORD);
    newadmin.phoneNumber = obtainLongInt(910000000,999999999, "por favor introduza o numero de telemovel do administrador");
    time_t y = time(NULL);
    struct tm ym = *localtime(&y);
    newadmin.day_contract.day =  ym.tm_mday;
    newadmin.day_contract.month = ym.tm_mon+1;
    newadmin.day_contract.year =  ym.tm_year+1900;
    array_add_elem(admins, &newadmin);
    return MENU_ACTION_REPRINT;
}


int update_admin_action(void *userData)
{
    ptr_and_index *ptrIdx = (ptr_and_index *) userData;
    array *arr = (array *) ptrIdx->ptr;
    size_t idx = ptrIdx->idx;
    Admin *curr = array_get_t(Admin , arr, idx);
    readString(curr->name, MAX_CLIENT_NAME, MSG_OBTAIN_ADMIN_NAME);
    readString(curr->residency, MAX_RESIDEN, MSG_OBTAIN_ADMIN_RESI);
    readString(curr->password, PASSWORD_SIZE, MSG_OBTAIN_PASSWORD);
    curr->phoneNumber = obtainLongInt(910000000, 969999999, "Qual o numero de telemovel do admin?");
    return MENU_ACTION_BACK;
}

int update_admin(void *userData)
{
    array *admins = (array *) userData;
    selecionar_admin(admins, &update_admin_action);
    return MENU_ACTION_REPRINT;
}


int remove_admin_action(void *userData)
{
    ptr_and_index *ptrIdx = (ptr_and_index *) userData;
    array *arr = (array *) ptrIdx->ptr;
    array_remove(arr, ptrIdx->idx);
    return MENU_ACTION_BACK;
}

int remove_admin(void *userData)
{
    array *admins = (array *) userData;

    // Construir menu
    array entries = array_new_with_capacity(sizeof(menuEntry), admins->size);
    array ptrIdxs = array_new_with_capacity(sizeof(ptr_and_index), admins->size);
    for (size_t i = 0; i < admins->size; i++) {
        ptr_and_index ptrIdx = {admins, i};
        array_add_elem(&ptrIdxs, &ptrIdx);
        menuEntry entry = {
                .name = array_get_t(Admin, admins, i)->name,
                .menuEntry = &remove_admin_action,
                .userData = array_get(&ptrIdxs, i),
        };
        array_add_elem(&entries, &entry);
    }

    go_to_menu(admins->size, "Remover Administradores", entries._data);
    array_free(&ptrIdxs);
    array_free(&entries);
    return MENU_ACTION_REPRINT;
}


int add_furniture_to_order_action(void* userData)
{
    two_ptrs* twoPtr = (two_ptrs *) userData;
    Order* order = twoPtr->ptr1;
    Furniture* furniture = twoPtr->ptr2;
    order->price += furniture->price;
    array_add_elem(&order->products, furniture->furtnitureCode);
    return MENU_ACTION_REPRINT;
}

int finalize_order_action(void* userData){
    two_ptrs* ptrs = (two_ptrs*) userData;
    Order* order = ptrs->ptr1;
    estado* state = ptrs->ptr2;
    array_add_elem(&state->orders, order);
    return MENU_ACTION_BACK;
}

int new_order(void *userData) {

    estado *state = (estado *) userData;
    Order newOrder = {
            .isActive = 1,
            .products = array_new(sizeof(char[PRODUCT_CODE_SIZE])),
    };

    time_t y = time(NULL);
    struct tm ym = *localtime(&y);
    newOrder.makeOrderDay.Dday =  ym.tm_mday;
    newOrder.makeOrderDay.Dmonth = ym.tm_mon+1;
    newOrder.makeOrderDay.Dyear =  ym.tm_year+1900;
    y = time(NULL) + 7*24*60*60;
    ym = *localtime(&y);
    newOrder.deliveryDay.Dday =  ym.tm_mday;
    newOrder.deliveryDay.Dmonth = ym.tm_mon+1;
    newOrder.deliveryDay.Dyear =  ym.tm_year+1900;
    int correctCode;
    do{
        correctCode = 1;
        readString(newOrder.orderCode, ORDER_CODE_SIZE, "Introduza o codigo da encomenda");
        for (size_t i = 0; i < state->orders.size; ++i) {
            if(strcmp(newOrder.orderCode, array_get_t(Order, &state->orders, i)->orderCode) == 0){
                correctCode = 0;
                break;
            }
        }
        if(!correctCode){
            puts("codigo de encomenda ja atribuido");
        }

    }while(!correctCode);

    newOrder.clientNumber = obtainInt(1,10000, "Introduza o numero do cliente");
    // Construir menu
    array entries = array_new_with_capacity(sizeof(menuEntry), state->furniture.size+1);
    array twoPtrs = array_new_with_capacity(sizeof(two_ptrs), state->furniture.size);
    for (size_t i = 0; i < state->furniture.size; i++) {
        two_ptrs twoPtr = {&newOrder, array_get_t(Furniture, &state->furniture, i)};
        array_add_elem(&twoPtrs, &twoPtr);
        menuEntry entry = {
                .name = array_get_t(Furniture, &state->furniture, i)->name,
                .menuEntry = &add_furniture_to_order_action,
                .userData = array_get(&twoPtrs, i),
        };
        array_add_elem(&entries, &entry);
    }

    two_ptrs ptrs = {&newOrder, state};
    menuEntry entry = {
            .name = "finalizar encomenda",
            .menuEntry = &finalize_order_action,
            .userData = &ptrs,
    };
    array_add_elem(&entries, &entry);

    go_to_menu(entries.size, "Adicionar Produto A Encomenda", entries._data);
    array_free(&twoPtrs);
    array_free(&entries);
    return MENU_ACTION_REPRINT;
}

int displayMenuCliente(void *userData) {
    estado *state = (estado *) userData;
    menuEntry entries[] = {
            {"Ver produtos",                      &displayProducts, &state->furniture},
            {"Nova Encomenda",                      &new_order, state},
            {"Ver contactos de apoio ao cliente", &displayCostumerSupport, &state->admins},
    };
    return go_to_menu(3, "Móveis for all", entries);
}

int mostrar_producto(void *userData)
{
    ptr_and_index *data = (ptr_and_index *) userData;
    array *furniture = data->ptr;
    divisao_t tipo = data->idx;

    for (size_t i = 0; i < furniture->size; i++) {
        Furniture *element = array_get_t(Furniture, furniture, i);
        if (element->divisao == tipo) {
            printf("%s\n %2.2f\n", element->name, element->price);
        }
    }
    return MENU_ACTION_REPRINT;
}


int mostrar_admins(void *userData)
{
    array *admins = (array*) userData;

    for (size_t i = 0; i < admins->size; i++) {
        Admin *element = array_get_t(Admin ,admins, i);
        printf("%d -> %s\n %li\n",element->adminnumber, element->name, element->phoneNumber);
    }
    return MENU_ACTION_REPRINT;
}

int mostrar_clients(void *userData)
{
    array *clients = (array*) userData;

    for (size_t i = 0; i < clients->size; i++) {
        ClientProfile *element = array_get_t(ClientProfile ,clients, i);
        printf("%d \n%s\n %li\n status : %d\n",element->number, element->name, element->phoneNumber,element->isActive);
    }
    return MENU_ACTION_REPRINT;
}
int displayorders (void *userData) {

    estado *state = (estado *) userData;

    for (size_t i = 0; i < state->orders.size; i++) {

        Order *element = array_get_t(Order, &state->orders, i);

        if(!element->isActive) {
            continue;
        }

        printf("numero de cliente %d\n", element->clientNumber);
        printf("data em que se realizou a encomenda %d-%d-%d\n", element->makeOrderDay.Dday,
               element->makeOrderDay.Dmonth, element->makeOrderDay.Dyear);
        printf("data de entrega %d-%d-%d\n", element->deliveryDay.Dday, element->deliveryDay.Dmonth,
               element->deliveryDay.Dyear);
        printf("preco %2.2f\n", element->price);
        printf("status da entrega %d\n", element->isActive);


        for(size_t j = 0; j < element->products.size; j++){
            // char[PRODUCT_CODE_SIZE]
            char *productCode = array_get_t(char, &element->products, j);

            int found = 0;
            for(size_t k = 0; k < state->furniture.size; k++) {
                Furniture* elementFurniture = array_get_t(Furniture, &state->furniture, k);
                if(strcmp(productCode, elementFurniture->furtnitureCode) == 0) {
                    printf("\t%s %s %2.2f\n", elementFurniture->furtnitureCode, elementFurniture->name, elementFurniture->price);
                    found = 1;
                    break;
                }
            }

            if(!found){
                printf("%s [info not found]\n", productCode);
            }
        }
    }
    return MENU_ACTION_REPRINT;
}

int displayProducts(void *userData) {
    ptr_and_index dataSala = {userData, SALA};
    ptr_and_index dataCozinha = {userData, COZINHA};
    ptr_and_index dataQuarto = {userData, QUARTO};
    ptr_and_index dataWC = {userData, WC};
    menuEntry entries[] = {
            {"Sala",          &mostrar_producto, &dataSala},
            {"Cozinha",       &mostrar_producto, &dataCozinha},
            {"Quarto",        &mostrar_producto, &dataQuarto},
            {"Casa de Banho", &mostrar_producto, &dataWC},
    };
    return go_to_menu(4, "Móveis for all", entries);
}


int displayCostumerSupport(void *userData) {

    array *admins = (array*) userData;

    for (size_t i = 0; i < admins->size; i++)
    {
        Admin *element = array_get_t(Admin ,admins, i);
        printf(" administrador : %s\n n. telemóvel : %ld\n", element->name, element->phoneNumber);
    }
    return MENU_ACTION_REPRINT;
}
