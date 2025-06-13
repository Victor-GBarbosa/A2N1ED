#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <time.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum Types {
    INTEGER = 0,
    BOOLEAN,
    DOUBLE,
    STRING
} Types;

typedef struct sensor_t {
    int timestamp;
    char name[16];
    Types type;
    union value
    {
        int intType;
        double doubleType;
        bool boolType;
        char stringType[16]; 
    } value;
    
} sensor_t;

int isValidDateTime (int dia, int mes, int ano, int hora, int min, int seg);

time_t converter_para_timestap(int dia, int mes, int ano, int hora, int min, int seg);
time_t gerar_timestamp_aleatorio(struct tm * inicial, struct tm * final);

sensor_t getRegister(char *linha);

#endif