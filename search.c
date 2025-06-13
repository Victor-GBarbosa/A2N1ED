#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "timestamp.h"

int main (int argc, char  *argv[]) {

    struct tm targetDate;
    if(sscanf(argv[1], "%d/%d/%d", &targetDate.tm_mday, &targetDate.tm_mon, &targetDate.tm_year) == 3 &&
       sscanf(argv[2], "%d:%d:%d", &targetDate.tm_hour, &targetDate.tm_min, &targetDate.tm_sec) == 3) 
       {
        printf("Data Cadastrada com sucesso\n");

        } else {
        printf("Erro na entrada de dados\n");
        return 1;
        }

    long int targetTs = converter_para_timestap(targetDate.tm_mday, targetDate.tm_mon, targetDate.tm_year,
                                                targetDate.tm_hour, targetDate.tm_min, targetDate.tm_sec);   
                                             
    printf("Timestamp alvo: %ld\n", targetTs);

    char linha[256];

    for (int i = 3; i < argc; i++) {
        printf("\nProcessando arquivo: %s\n", argv[i]);
        
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
           
        //busca binaria para o ts mais proximo

        int left = 0;
        int right = registrosValidos - 1;
        int closest = 0;
        long int minDiff = labs(registros[0].timestamp - targetTs);

        while (left <= right) {
            int mid = left + (right - left) / 2;
            long int currentTs = registros[mid].timestamp;
            long int diff = labs(currentTs - targetTs);
            
            // Atualizar o mais próximo se encontrou diferença menor
            if (diff < minDiff) {
                minDiff = diff;
                closest = mid;
            }
            
            // Se encontrou exato, pode parar
            if (currentTs == targetTs) {
                closest = mid;
                break;
            } else if (currentTs < targetTs) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        printf("\n----------\n\nTimestamp encontrado: %ld\nNo arquivo: %s\nDiferença: %ld segundos\n\n----------\n", 
        registros[closest].timestamp, argv[i], minDiff);
        
        free(registros); //limpa os registros para o proximo arquivo
    } //termina de procurar em 1 arquivo;
    
    return 0;
}