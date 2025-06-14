#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "timestamp.h"

int main (int argc, char  *argv[]) {

    if (argc < 4) {
        printf("Argumentos insuficientes\nEsperado: .\nome.exe dd/mm/yyyy hh:mm:ss arquivo.txt arquivo2.txt...");
        return 1;
    }

    struct tm targetDate;
    if(sscanf(argv[1], "%d/%d/%d", &targetDate.tm_mday, &targetDate.tm_mon, &targetDate.tm_year) == 3 &&
       sscanf(argv[2], "%d:%d:%d", &targetDate.tm_hour, &targetDate.tm_min, &targetDate.tm_sec) == 3) 
       {} else {
        printf("Erro na entrada de dados\n");
        return 1;
        }    

    long int targetTs = converter_para_timestap(targetDate.tm_mday, targetDate.tm_mon, targetDate.tm_year,
                                                targetDate.tm_hour, targetDate.tm_min, targetDate.tm_sec);   

    char linha[256];

    for (int i = 3; i < argc; i++) {
        
        FILE *fp = fopen(argv[i], "r"); 
        if (fp == NULL) {
            printf("Erro ao abrir arquivo: %s\n", argv[i]);
            continue;
        }

        long int fileLineCounter = contarLinhas(argv[i], 0);
        sensor_t *registros = malloc(sizeof(sensor_t) * fileLineCounter);
        if (registros == NULL) {
            printf("Erro ao alocar memória\n");
            fclose(fp);
            continue;
        }

        int registrosValidos = 0;
        for (int j = 0; j < fileLineCounter; j++) {
            if (fgets(linha, sizeof(linha), fp) == NULL) {
                break;
            }

            linha[strcspn(linha, "\n")] = '\0';
            registros[registrosValidos] = getRegister(linha);

            if (registros[registrosValidos].timestamp == 0) {
                printf("Erro ao processar linha: %s\n", linha);
                continue;
            }
            registrosValidos++;    
        } //termina de coletar as registros
        
        fclose(fp);
        
        if (registrosValidos == 0) {
            printf("Nenhum registro válido encontrado no arquivo\n");
            free(registros);
            continue;
        }
             int left = 0;
        int right = registrosValidos - 1;
        int closest = 0;
        long int minDiff = labs(registros[0].timestamp - targetTs);

        while (left <= right) {
            int mid = left + (right - left) / 2;
            long int currentTs = registros[mid].timestamp;
            long int diff = labs(currentTs - targetTs);
            
            if (diff < minDiff) {
                minDiff = diff;
                closest = mid;
            }
            
            if (currentTs == targetTs) {
                closest = mid;
                break;
            } else if (currentTs < targetTs) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }        time_t timestamp = (time_t)registros[closest].timestamp;
        struct tm *date_info = localtime(&timestamp);
        printf("\nSensor encontrado em: %02d/%02d/%04d %02d:%02d:%02d\nNome: %s\nTipo: ", 
        date_info->tm_mday, date_info->tm_mon + 1, date_info->tm_year + 1900,
        date_info->tm_hour, date_info->tm_min, date_info->tm_sec, registros[closest].name);
        
        switch(registros[closest].type) {
            case INTEGER:
                printf("INTEGER\nValor: %d", registros[closest].value.intType);
                break;
            case BOOLEAN:
                printf("BOOLEAN\nValor: %s", registros[closest].value.boolType ? "true" : "false");
                break;
            case DOUBLE:
                printf("DOUBLE\nValor: %.2f", registros[closest].value.doubleType);
                break;
            case STRING:
                printf("STRING\nValor: %s", registros[closest].value.stringType);
                break;
        }
        
        printf("\nNo arquivo: %s\nDiferenca: %ld segundos\n\n----------\n", argv[i], minDiff);
        
        free(registros); //limpa os registros para o proximo arquivo
    } //termina de procurar em 1 arquivo;
    
    return 0;
}