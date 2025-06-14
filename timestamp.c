#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "timestamp.h"

Types typeVerify(char *valorStr) {

    if (strcmp(valorStr, "true") == 0 || strcmp(valorStr, "false") == 0) {
        return BOOLEAN;
    }
      
    char* endptr;
    strtod(valorStr, &endptr);
    
    if (*endptr == '\0' || *endptr == '\n') {
        if (strchr(valorStr, '.') != NULL) {
            return DOUBLE;
        } else {
            return INTEGER;
        }
    }
    
    return STRING;
}

int isValidDateTime(int dia, int mes, int ano, int hora, int min, int seg) {

    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (ano > 9999) {
        printf("Ano invalido.\n");
        return 0;
    }
    if (mes < 1 || mes > 12) {
        printf("Mes invalido.\n");
        return 0;
    }
    if (dia < 1 || dia > 31) {
        printf("Dia invalido.\n");
        return 0;
    }
    if (hora < 0 || hora > 23) {
        printf("Hora invalida.\n");
        return 0;
    }
    if (min < 0 || min > 59) {
        printf("Minuto invalido.\n");
        return 0;
    }
    if (seg < 0 || seg > 59) {
        printf("Segundo invalido.\n");
        return 0;
    }
    
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) { //verifica se o ano é bissexto e adiciona +1 dia a fervereiro
        diasNoMes[1] = 29;
    }
    if (dia > diasNoMes[mes - 1]) {
        printf("Dia invalido para o mes especificado.\n");
        return 0;
    }

    return 1;
}

sensor_t getRegister(char *linha) {
    
    sensor_t thisRegister;
    sensor_t err = {0};

    static char buffer[256];
    strncpy(buffer, linha, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " "); 
    if (token == NULL) return err;
    thisRegister.timestamp = atoi(token);
    
    token = strtok(NULL, " "); 
    if (token == NULL) return err;
    strncpy(thisRegister.name, token, sizeof(thisRegister.name) - 1);
    thisRegister.name[sizeof(thisRegister.name) - 1] = '\0';
      token = strtok(NULL, ""); 
    if (token == NULL) return err;
    thisRegister.type = typeVerify(token);

    switch (thisRegister.type) {
        case BOOLEAN:
            if (strcmp(token, "true") == 0) {
                thisRegister.value.boolType = 1;
            } else {
                thisRegister.value.boolType = 0;
            }
            break;
        case INTEGER:
            thisRegister.value.intType = atoi(token);
            break;
        case DOUBLE:
            thisRegister.value.doubleType = atof(token);
            break;        case STRING:
            strncpy(thisRegister.value.stringType, token, sizeof(thisRegister.value.stringType) - 1);
            thisRegister.value.stringType[sizeof(thisRegister.value.stringType) - 1] = '\0';
            break;
        default:
            return err;
            break;
    }

    return thisRegister;
}

time_t converter_para_timestap(int dia, int mes, int ano, int hora, int min, int seg) {

    if (!isValidDateTime(dia, mes, ano, hora, min, seg)) {
        printf("Erro ao converter data para timestamp;");
        return 1;
    }

    struct tm t;

    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    time_t timestamp = mktime(&t);
    if (timestamp == -1) {
        printf("Data inválida. Tente novamente.\n");
    } else {
        return timestamp;
    }
}

time_t gerar_timestamp_aleatorio(struct tm * inicial, struct tm * final) {

    inicial->tm_isdst = -1;
    final->tm_isdst= -1;
    
    time_t timestamp_inicial, timestamp_final;
        
    timestamp_inicial = mktime(inicial);
    if (timestamp_inicial == -1) {
        printf("Data inválida: inicial.\n");
        return -1;
    }

    timestamp_final = mktime(final);
    if (timestamp_final == -1) {
        printf("Data inválida: Final.\n");
        return -1;
    }

    time_t timestamp_aleatorio = timestamp_inicial + (time_t)((double)rand() / RAND_MAX * (timestamp_final - timestamp_inicial));
    
    return timestamp_aleatorio;
}

int contarLinhas(char* fileName, int ignore) {
   FILE *fp = fopen(fileName, "r");
   if (fp == NULL) {
      printf("Erro ao abrir o arquivo 2");
       return -1;
   }
   int contador = 0;
   char linha[256]; 
   while (fgets(linha, sizeof(linha), fp) != NULL) {
       contador++; 
   }
   fclose(fp);
   return contador - ignore;
}