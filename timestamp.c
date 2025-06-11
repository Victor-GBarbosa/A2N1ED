#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "timestamp.h"

int isValidDateTime(int dia, int mes, int ano, int hora, int min, int seg) {

    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (ano < 1900 || ano > 9999) {
        printf("Ano inválido.\n");
        return 0;
    }
    if (mes < 1 || mes > 12) {
        printf("Mês inválido.\n");
        return 0;
    }
    if (dia < 1 || dia > 31) {
        printf("Dia inválido.\n");
        return 0;
    }
    if (hora < 0 || hora > 23) {
        printf("Hora inválida.\n");
        return 0;
    }
    if (min < 0 || min > 59) {
        printf("Minuto inválido.\n");
        return 0;
    }
    if (seg < 0 || seg > 59) {
        printf("Segundo inválido.\n");
        return 0;
    }
    
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) { //verifica se o ano é bissexto e adiciona +1 dia a fervereiro
        diasNoMes[1] = 29;
    }
    if (dia > diasNoMes[mes - 1]) {
        printf("Dia inválido para o mês especificado.\n");
        return 0;
    }

    return 1; //retorna "verdadeiro" caso não caia em nenhum if
}

time_t converter_para_timestap(int dia, int mes, int ano, int hora, int min, int seg) {

    isValidDateTime(dia, mes, ano, hora, min, seg);

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

    time_t timestamp_aleatorio = timestamp_inicial + rand() % (timestamp_final - timestamp_inicial + 1);
    
    return timestamp_aleatorio;
}