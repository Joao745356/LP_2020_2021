//
// Created by joao7 on 08/01/2023.
//

#include "array.h"


#include<string.h>
#include <stdlib.h>


#define GROW_MULTIPLIER 2
#define idx_to_ptr(ARR, I) (((char*)((ARR)->_data)) + ((I) * (ARR)->_element_size))

// char* array->_data dá o spot 0 num array, ex . 0 1 2 3 4 5 6 e depois I * element_size dá o contador
// ou seja, char* array->_data = 0 + (I é 5) * element_size = 0 + 5 = ao spot 5 no array

int array_grow(array *arr) //esta funçao recebe um pointer array
{
    size_t newCapacity = arr->_capacity * GROW_MULTIPLIER;  //dita a sua capacidade como o dobro da antiga;
    // edge-case arr has no capacity  (first grow) - on the first run, the array has 0 capacity, thus it gets started at 4.
    if (newCapacity == 0) {
        newCapacity = 4;
    }

    void *newData = realloc(arr->_data, newCapacity * arr->_element_size); /*realloca a informação antiga para um novo espaço na memoria
                                                                            realloc recebe data e tamanho (sizeof)*/
    if (newData == NULL) {
        return 0;
    }

    arr->_data = newData;   //newData é a o conjunto de dados  para onde o array pointer o leva
    arr->_capacity = newCapacity; //newcapacity é a antiga * 2;

    return 1;   //if success = return 1; a condiçao na main é if != suc error;
}

array array_new(size_t elem_size) //array new, recebe um elem_size, cria um array usnado o mesmo
{
    array arr = {._data = NULL, ._element_size=elem_size, .size=0, ._capacity=0}; //retorna um array com td a 0, estando a 0, n tem problema
    //pois a funçao array grow cresce o array para 4 por default se a capacidade for 0
    return arr;
}

array array_new_with_capacity(size_t elem_size, size_t capacity) {
    array arr = {
            ._element_size = elem_size,
            .size = 0,
            ._capacity = capacity,
            ._data = malloc(elem_size * capacity),  
    };

    if (arr._data == NULL && elem_size > 0 && capacity > 0)
    {
        puts("não consegui alocar memória");
    }

    return arr;
}

int array_add_elem(array *arr, void *elem) // recebe um array pointer e um pointer para um elemento
{
    if (arr->_capacity == arr->size) {
        int err = 0; // se a capacidade do array for igual ao seu size, entao temos um erro
        if (!(err = array_grow(
                arr))) { // se o '' erro '' n conseguir aumentar o array  : array_grow(arr) entao aí, retorna o err.
            return err;
        }
    }
    /*memcpy recebe : destino - This is pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*.
     recebe : source - src − This is pointer to the source of data to be copied, type-casted to a pointer of type void*.
     e numero de bytes a ser copiado ( arr->_element_size
    entao memcpy recebe 2 void pointers, o 1º com destino, o 2º com a source, o 3º com o numero de bytes*/
    memcpy(idx_to_ptr(arr, arr->size), elem, arr->_element_size);  // memcpy copia a info para o ultimo index do array
    arr->size++; // size++ é self explanatory aqui.
    return 1;
}

/*O array remove pega na informaçao de um array , pega no elemento a seguir ao que queremos apagar, e depois
 copia o mesmo para o spot do elemento que queremos apagar, depois disso copia todos os outros para trás 1 e no fim faz size--
 onde size, é o size do nosso array*/

void array_remove(array *arr, size_t index) {
    memcpy(idx_to_ptr(arr, index), idx_to_ptr(arr, index + 1), arr->_element_size * (arr->size - index - 1));
    arr->size--;
}

//array get idx_to_ptr(ARR, I) (((char*)ARR->_data) + ((I) * ARR->_element_size)) we typecast char pq qualquer char é 1 byte
void *array_get(array *arr, size_t index) {
    return idx_to_ptr(arr, index);
} // imagina que o array está no spot 5000 na memoria, temos char * 5000 + (index * element size) para nos dar um index no array

void array_free(array *arr) {
    free(arr->_data);
    arr->_data = NULL;
    arr->size = 0;
    arr->_capacity = 0; // free the entire array, data, size e capacidade;
}

int array_write(array *arr, FILE *f, size_t (*objectWriteFn) (void*, FILE*))  // esta funçao recebe um pointer para o array, e o pointer para um file para escrever
{
    size_t bytes_written = fwrite(&arr->_element_size, sizeof(size_t), 1, f);  //escreve o element size
    bytes_written += fwrite(&arr->size, sizeof(size_t), 1, f);  //escreve o tamanho que está preenchido do array
    if (objectWriteFn == NULL) {
        bytes_written += fwrite(arr->_data, arr->_element_size, arr->size, f); //escreve data informaçao
    } else {
        for(size_t i = 0; i < arr->size; i++) {
            bytes_written += (*objectWriteFn)(array_get(arr, i), f);
        }
    }

    if (bytes_written !=  2 + arr->size) {
        return 0; // erro nao conseguiu escrever
    }

    return 1;
}

int array_read(array *arr, FILE *f, size_t (*objectReadFn) (void*, FILE*))  //esta funçao deve correr no inicio do programa para ler toda a informaçao disponivel at the time
{
    if (arr->_capacity > 0) {
        array_free(arr);
    }

    size_t element_size, size;

    size_t bytes_read = fread(&element_size, sizeof(size_t), 1, f);
    if (bytes_read != 1) {
        return 0;
    }

    bytes_read = fread(&size, sizeof(size_t), 1, f);
    if (bytes_read != 1) {
        return 0;
    }

    *arr = array_new_with_capacity(element_size, size);

    if(size == 0) {
        return 1;
    }

    if (objectReadFn == NULL) {
        bytes_read = fread(arr->_data, element_size, size, f);
        if (bytes_read != size) {
            return 0;
        }
        arr->_element_size = element_size;
        arr->size = size;
    } else {
        arr->_element_size = element_size;
        arr->size = 0;
        for(size_t i = 0; i < size; i++) {
            bytes_read = (*objectReadFn)(array_get(arr, i), f);
            if (bytes_read != 1) {
                return 0;
            }

            arr->size++;
        }
    }

    return 1;
}