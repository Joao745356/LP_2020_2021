//
// Created by joao7 on 08/01/2023.
//

#include "input.h"


#include <stdio.h>
#include <string.h>

#define INVALID_VALUE "O valor inserido é inválido."

void cleanInputBuffer() {
    fseek(stdin,0,SEEK_END);
}

float obtainFloat(float minValue, float maxValue, char *msg)
{
    float value;
    printf(msg);

    while (scanf("%f", &value) != 1 || value < minValue || value > maxValue) {
        puts(INVALID_VALUE);
        cleanInputBuffer();
        printf(msg);
    }

    cleanInputBuffer();
    return value;
}
long int obtainLongInt(long minValue, long maxValue, char *msg)
{
    long value;
    puts(msg);

    while (scanf("%ld", &value) != 1 || value < minValue || value > maxValue) {
        puts(INVALID_VALUE);
        cleanInputBuffer();
        printf(msg);
    }

    cleanInputBuffer();
    return value;
}

int obtainInt(int minValue, int maxValue, char *msg)
{
    int value;
    puts(msg);

    while (scanf("%d", &value) != 1 || value < minValue || value > maxValue) {
        puts(INVALID_VALUE);
        cleanInputBuffer();
        printf(msg);
    }

    cleanInputBuffer();
    return value;
}

double obtainDouble(int minValue, int maxValue, char *msg)
{
    double value;
    printf(msg);

    while (scanf("%lf", &value) != 1 || value < minValue || value > maxValue) {
        puts(INVALID_VALUE);
        cleanInputBuffer();
        printf(msg);
    }

    cleanInputBuffer();
    return value;
}

char obtainChar(char *msg)
{
    char value;
    printf(msg);
    value = getchar();
    cleanInputBuffer();
    return value;
}

void readString(char *string, unsigned int size, char *msg)
{
    printf(msg);

    if (fgets(string, size, stdin) != NULL) {
        unsigned int len = strlen(string) - 1;
        if (string[len] == '\n') {
            string[len] = '\0';
        } else {
            cleanInputBuffer();
        }
    };
}