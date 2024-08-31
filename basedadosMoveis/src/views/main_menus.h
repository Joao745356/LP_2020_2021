//
// Created by joao7 on 08/01/2023.
//

#ifndef UNTITLED3_MAIN_MENUS_H
#define UNTITLED3_MAIN_MENUS_H
#include "../model/state.h"

int displayMenuPrincipal(void *userData);



int displayMenuAdmin(void *userData);//working
int adminExists(array* admins, int number, char* password);
int export_materials(void* userData);//WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOORKING
int displayProductManagement(void *userData);//working
int displayorders (void *userData); //working
int mostrar_clients(void *userData); //working
int add_client(void *userData);//working
int update_client_action(void *userData);//working
int update_client(void *userData);//working
int remove_client_action(void *userData);//working
int remove_client_menu(void *userData);//working
int add_product(void *userData);//working
int update_product(void *userData);//working
int update_product_action(void *userData);//working
int remove_furniture_action(void *userData);//working
int remove_product(void *userData);//working
int displayClientMangement(void *userData);//working
//int selecionar_cliente(array *clientes, int(*action)(void*));
int displayOrderManagement(void *userData);//working



int displayMenuCliente(void *userData);//working
int displayProducts(void *userData);//working
int mostrar_producto(void *userData);//working
int new_order(void *userData);//working
int update_order(void *userData); //working
int add_furniture_to_order_action(void* userData);//working
int finalize_order_action(void* userData);//working
int remove_order(void *userData);//working
int remove_order_action(void *userData);//working
int displayCostumerSupport(void *userData);//working




int displayMenuDono(void *userData);//working
int mostrar_admins(void *userData);//working
int add_admin(void *userData);//working
int update_admin_action(void *userData);//working
int update_admin(void *userData);//working
//int selecionar_admin(array *admins, int(*action)(void*));//working
int remove_admin(void *userData);//working
int remove_admin_action(void *userData);//working


int escreverParaFicheiro(void *userData);//working








#endif //UNTITLED3_MAIN_MENUS_H