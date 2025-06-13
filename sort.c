#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "timestamp.h"

#define MAX_SENSORES 100

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

int main (int argc, char *argv[]) {

    char fname[256];
    strcpy(fname, argv[1]);
    printf("String copiada\n");
    printf(fname);
    int fileLineCounter = contarLinhas(fname, 0);

    sensor_t *registros = malloc(sizeof(sensor_t) * fileLineCounter);
    if (registros == NULL) {return 1;}

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo 1\n");
        return 1;
    }

    printf("\nDebug: Teste 1\n");

    fseek(fp, 0, SEEK_SET);
    char linha[256];


    static char *nomeSensores[MAX_SENSORES] = {NULL};
    int registrosValidos = 0;
    for (int i = 0; i < fileLineCounter; i++) {
        if (fgets(linha, sizeof(linha), fp) == NULL) {
            break;
        }

        linha[strcspn(linha, "\n")] = '\0'; //trocando a quebra de linha "\n" pelo finalizador \0
        
        registros[registrosValidos] = getRegister(linha);
        
        int senVerify = 0; //variavel responsavel por verificar se um sensor existe ou nnao
        for (int c = 0; c < MAX_SENSORES; c++) {
            if (nomeSensores[c] != NULL && strcmp(nomeSensores[c], registros[registrosValidos].name) == 0) {
                senVerify = 1;
                break;
            }
        }
        
        
        if (!senVerify) {
            for (int c = 0; c < MAX_SENSORES; c++) {
                if (nomeSensores[c] == NULL) {
                    nomeSensores[c] = malloc(strlen(registros[registrosValidos].name) + 1);
                    strcpy(nomeSensores[c], registros[registrosValidos].name);
                    break;
                }
            }
        }

        if (registros[registrosValidos].timestamp == 0 && registros[registrosValidos].name[0] == '\0') {
            printf("Erro ao processar linha: %s\n", linha);
            continue;
        }
        
        registrosValidos++;
    }    printf("Debug: %d registros válidos processados!\n", registrosValidos);

    int contador, j = 0, k = 0;
    sensor_t tempReg;
    do {
      contador = 0;
      j = 0;
      do {
      if (registros[j].timestamp > registros[j + 1].timestamp) {
        contador++;
        tempReg = registros[j+1];
        registros[j+1] = registros[j];
        registros[j] = tempReg;
      }
      j++;
      } while (j < registrosValidos - 1);
    } while (contador != 0);

    fclose(fp);

    fp = fopen (fname, "w");
    if (fp == NULL) {
        return 1;
    }  
    
    for(int i = 0; i < registrosValidos; i++) {
        switch (registros[i].type) {
            case INTEGER:
                fprintf(fp, "%d %s %d\n", 
                        registros[i].timestamp, 
                        registros[i].name, 
                        registros[i].value.intType);
                break;
                
            case BOOLEAN:
                fprintf(fp, "%d %s %s\n", 
                        registros[i].timestamp, 
                        registros[i].name, 
                        registros[i].value.boolType ? "true" : "false");
                break;
                
            case DOUBLE:
                fprintf(fp, "%d %s %.2f\n", 
                        registros[i].timestamp, 
                        registros[i].name, 
                        registros[i].value.doubleType);
                break;
                
            case STRING:
                fprintf(fp, "%d %s %s\n", 
                        registros[i].timestamp, 
                        registros[i].name, 
                        registros[i].value.stringType);
                break;
                
            default:
                fprintf(stderr, "Erro: Tipo de sensor desconhecido no registro %d\n", i);
                break;
        }
    }

    fclose(fp);

    //separação de arquivos -----------------------------------------------------
    
    char *sFName = NULL;
    int nsi = 0;
    printf("Dbg 1\n");

    while (nomeSensores[nsi] != NULL) {
        sFName = malloc(strlen(nomeSensores[nsi]) + 5);
        sprintf(sFName,"%s.txt", nomeSensores[nsi]);
        printf("Dbg 2\n");
        if (sFName == NULL) {
            printf("Erro ao alocar memoria para o nome do arquivo: %s.txt", nomeSensores[nsi]);
            break;
        }
        printf("Dbg 3\n");
        fp = fopen(sFName, "w");

        if (fp == NULL) {
            printf("Erro ao criar o arquivo %s.txt", sFName);
            break;
        }
        printf("Dbg 4\n");
        for (int i = 0; i < registrosValidos; i++) {
            if (!strcmp(registros[i].name, nomeSensores[nsi])) {
                switch (registros[i].type)  {
                    case INTEGER:
                        fprintf(fp, "%d %s %d\n", 
                                registros[i].timestamp, 
                                registros[i].name, 
                                registros[i].value.intType);
                        break;
                        
                    case BOOLEAN:
                        fprintf(fp, "%d %s %s\n", 
                                registros[i].timestamp, 
                                registros[i].name, 
                                registros[i].value.boolType ? "true" : "false");
                        break;
                        
                    case DOUBLE:
                        fprintf(fp, "%d %s %.2f\n", 
                                registros[i].timestamp, 
                                registros[i].name, 
                                registros[i].value.doubleType);
                        break;
                        
                    case STRING:
                        fprintf(fp, "%d %s %s\n", 
                                registros[i].timestamp, 
                                registros[i].name, 
                                registros[i].value.stringType);
                        break;
                        
                    default:
                        fprintf(stderr, "Erro: Tipo de sensor desconhecido no registro %d\n", i);
                        break;
                }
            };
            
        }
        fclose(fp);
        nsi++;
    }


    fclose(fp);
    free(registros);
    

}