//
// Created by joao7 on 08/01/2023.
//

#ifndef UNTITLED3_ARRAY_H
#define UNTITLED3_ARRAY_H

#include<stddef.h>
#include <stdio.h>

typedef struct {
    size_t size;                    //tamanho do array
    void *_data;                    //apontador a clientes, produtos, admins e encomendas
    size_t _capacity;               //capacidade do array
    size_t _element_size;           //element size de cada struct
} array;

array array_new(size_t elem_size); //usamos quando n sabemos o numero de elementos

int array_grow(array *arr);

array array_new_with_capacity(size_t elem_size, size_t capacity); //usamos quando já sabemos o numero de elementos q queremos no array. o bom disto é q n faz realloc do array

int array_add_elem(array *arr, void *elem);

void array_remove(array *arr, size_t index);

#define array_get_t(T, ARR, I) ((T*) array_get((ARR), (I)))            //devolve um pointer para um index num array especifico

void *array_get(array *arr, size_t index);  //este é chamado por array_get_t e converte o void pointer para o tipo em T*

void array_free(array *arr);

int array_write(array *arr, FILE *f, size_t (*objectWriteFn) (void*, FILE*));

int array_read(array *arr, FILE *f, size_t (*objectReadFn) (void*, FILE*));

#endif //UNTITLED3_ARRAY_H
