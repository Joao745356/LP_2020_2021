//
// Created by joao7 on 08/01/2023.
//

#ifndef UNTITLED3_INPUT_H
#define UNTITLED3_INPUT_H

void cleanInputBuffer();

float obtainFloat(float minValue, float maxValue, char *msg);

int obtainInt(int minValue, int maxValue, char *msg);

long int obtainLongInt(long minValue, long maxValue, char *msg);

double obtainDouble(int minValue, int maxValue, char *msg);

char obtainChar(char *msg);

void readString(char *string, unsigned int size, char *msg);


#endif //UNTITLED3_INPUT_H
