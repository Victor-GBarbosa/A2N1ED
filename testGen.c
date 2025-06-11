#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "timestamp.h"

#define SENSOR_QUANTITY 2000;

typedef struct sensorInfo {
    char name[16];
    int type;
} sensorInfo;

void printTm(struct tm *DateTime) {
    printf("\n %i/%i/%i %d:%d:%d", DateTime->tm_mday, DateTime->tm_mon, DateTime->tm_year, DateTime->tm_hour, DateTime->tm_min, DateTime->tm_sec);
}

int main (int argc, char *argv[]) {

    //Validação dos argumentos

    if (argc < 6) {
        printf("Erro: Argumentos insuficientes");
        printf("Esperado: DD/MM/AAAA-00:00:00 DD/MM/AAAA 00:00:00 scan1 scan2 scan3 scan[...]");
        return 0;
    }

    //processando dados inseridos na execução

    srand(time(NULL));

    int sensorCount = argc - 5;

    struct tm start = {0};
    struct tm end = {0};
    
    sensorInfo sensors[sensorCount];

    if(sscanf(argv[1], "%d/%d/%d", &start.tm_mday, &start.tm_mon, &start.tm_year) == 3 && sscanf(argv[2], "%d:%d:%d", &start.tm_hour, &start.tm_min, &start.tm_sec) == 3 &&
       sscanf(argv[3], "%d/%d/%d", &end.tm_mday, &end.tm_mon, &end.tm_year) == 3 && sscanf(argv[4], "%d:%d:%d", &end.tm_hour, &end.tm_min, &end.tm_sec) == 3) {
        start.tm_mon -= 1;
        start.tm_year -= 1900; 
        end.tm_mon -= 1;
        end.tm_year -= 1900;
        printf("Datas Cadastrados com sucesso");
       } else {
        printf("Erro na entrada de dados");
        return 0;
       }
    
    for (int i = 5; i < argc; i++) {
        if (sscanf(argv[i], "%15[^-]-%d", sensors[i-5].name, &sensors[i-5].type) != 2) {
            printf("Falha ao registrar os sensores");
            return 0;
        }
    } 

    //inserindo sensores no arquivo

    FILE *fp = NULL;

    int sensorQuantity = SENSOR_QUANTITY;

    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@&$?"; //caracteres canditatos a string aleatoria 
    int stringSize = 16;
    char stringValue[stringSize];
    char senString[256];

    for (int i = 0; i < sensorCount; i++) {

        char fname[20];
        snprintf(fname, sizeof(fname), "%s.txt", sensors[i].name);

        fp = fopen(fname, "a+");

        if (fp == NULL) {
            printf("Erro ao abrir ou criar o arquivo %s\n", fname);
            return 0;
        }

        for (int j = 0; j < sensorQuantity; j++) {
            sensor_t sensor = {};
            sensor.timestamp = gerar_timestamp_aleatorio(&start, &end);
            strncpy(sensor.name, sensors[i].name, sizeof(sensor.name));
            sensor.type = sensors[i].type;

            switch (sensor.type) {
            case INTEGER:
                sensor.value.intType = rand() % 99999;
                snprintf(senString, sizeof(senString), "%d %s %d\n", 
                 sensor.timestamp, sensor.name, sensor.value.intType);
                break;

            case BOOLEAN:
                sensor.value.boolType = rand() % 2;
                snprintf(senString, sizeof(senString), "%d %s %s\n", 
                 sensor.timestamp, sensor.name, sensor.value.boolType ? "true" : "false");
                break;

            case DOUBLE:
                sensor.value.doubleType = ((float)rand() / (float)RAND_MAX) * 99999;
                snprintf(senString, sizeof(senString), "%d %s %.2f\n", 
                 sensor.timestamp, sensor.name, sensor.value.doubleType);
                break;

            case STRING:
                for (int i = 0; i < stringSize-1; i++) {
                    stringValue[i] = charset[rand() % sizeof(charset) - 1] ;
                }
                stringValue[stringSize - 1] = '\0';
                // sensor.value.stringType = stringValue;
                strncpy(sensor.value.stringType, stringValue, sizeof(sensor.value.stringType));
                snprintf(senString, sizeof(senString), "%d %s %s\n", 
                 sensor.timestamp, sensor.name, sensor.value.stringType);
                break;
            
            default:
                printf("Erro ao cadastrar sensor: tipo invalido");
                return 0;
                break;
            }
            fputs(senString, fp);
        }
        fclose(fp);
    }

    printf("\nfinalizando");
    return 0;

}