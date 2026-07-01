#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>

#define CONFIG_SUBDIR "\\AppData\\Local\\FromJimToPam\\"
#define CONFIG_FILENAME "targets.conf"
#define MAX_PATH_LEN 512

    void MontarPathFile(char *buffer){
        char *userPath = getenv("USERPROFILE");
        if (userPath != NULL){
            snprintf(buffer, MAX_PATH_LEN, "%s%s%s", userPath, CONFIG_SUBDIR, CONFIG_FILENAME);
        }
    }
    
    void MontarPathAppData(char *buffer){
        char *userPath = getenv("USERPROFILE");
        if (userPath != NULL){
            snprintf(buffer, MAX_PATH_LEN, "%s%s", userPath, CONFIG_SUBDIR);
        }
    }

    void OpenNotepad(const char *pathFile) {
        char command[MAX_PATH_LEN];
        printf("[INFO] Abrindo editor de texto...\n");
        snprintf(command,sizeof(command), "start notepad %s", pathFile);
        system(command);
        printf("[INFO] Editor encerrado.\n");
        exit(0);
    }

    void FileDontFinded( const char *PathFile){
        printf("[INFO] Arquivo nao encontrado. Criando configuracao inicial...\n");
        char response[50];
        char PathAppData[MAX_PATH_LEN];
        strcpy(PathAppData, PathFile);

        char *ultima_barra = strrchr(PathAppData, '\\');

        if (ultima_barra != NULL) {
            *(ultima_barra + 1) = '\0'; 
        }
        
        printf("[INFO] Criando pasta: %s\n", PathAppData);
        _mkdir(PathAppData);
        
        FILE *file = fopen(PathFile, "w");
        fprintf(file, "# Coloque os links ou programas um abaixo do outro\n");
        fclose(file);

        printf("[INFO] Arquivo criado com sucesso!\n");
        printf("[INFO] Caminho do arquivo: %s\n", PathFile);
        printf("Deseja abrir o arquivo para editar agora? [S/N]: ");
        fgets(response, 50, stdin);
        response[0] = tolower(response[0]);

        if (response[0] == 's') {
            OpenNotepad(PathFile);
        } else {
            printf("[INFO] Programa finalizado.\n");
            exit(0);
        }
    }

    void Scanning(const char *PathFile){
    char line[MAX_PATH_LEN];
    FILE *file = fopen(PathFile, "r");
    char command[MAX_PATH_LEN];


    if(file == NULL){
        FileDontFinded(PathFile);
    }

    printf("[INFO] Buscando arquivo em: %s\n", PathFile);
    
    printf("[INFO] Arquivo encontrado. Lendo lista de alvos...\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        if (line[0] == '#') {
            continue;
        }

        printf("[INFO] Abrindo item: %s\n", line);
        snprintf(command,sizeof(command) ,"start \"\" \"%s\"", line);

        system(command);
    }

    printf("\n[INFO] Todos os alvos foram abertos.\n");
    fclose(file);
    }
    
int main(int argc, char *argv[]) {

    char pathFile[MAX_PATH_LEN];

    MontarPathFile(pathFile);
 
    if (argc > 1 && strcmp(argv[1], "-e") == 0) {
        OpenNotepad(pathFile);
    }
    
    Scanning(pathFile);
    
    return 0;


} 