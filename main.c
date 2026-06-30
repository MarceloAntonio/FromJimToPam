#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>

#define CONFIG_SUBDIR "\\AppData\\Local\\FromJimToPam\\"
#define CONFIG_FILENAME "targets.conf"
#define MAX_PATH_LEN 512

int main(int argc, char *argv[]) {

    // Variables
    char *userPath = getenv("USERPROFILE");
    FILE *file;
    char command[MAX_PATH_LEN];
    char line[MAX_PATH_LEN];
    char dirfile[MAX_PATH_LEN];
    sprintf(dirfile, "%s%s%s", userPath, CONFIG_SUBDIR, CONFIG_FILENAME);
    file = fopen(dirfile, "r");

    // Functions
    void OpenNotepad() {
        printf("[INFO] Abrindo editor de texto...\n");
        sprintf(command, "start %s%s%s", userPath, CONFIG_SUBDIR, CONFIG_FILENAME);
        system(command);
        printf("[INFO] Editor encerrado.\n");
        exit(0);
    }
    
    // Code 
    if (argc > 1 && strcmp(argv[1], "-e") == 0 && file != NULL) {
        OpenNotepad();
    }

    printf("[INFO] Buscando arquivo em: %s\n", dirfile);

    if (file == NULL) {
        printf("[INFO] Arquivo nao encontrado. Criando configuracao inicial...\n");
        char response[50];

        char dirPath[MAX_PATH_LEN];
        sprintf(dirPath, "%s%s", userPath, CONFIG_SUBDIR);
        printf("[INFO] Criando pasta: %s\n", dirPath);
        _mkdir(dirPath);

        file = fopen(dirfile, "w");
        fprintf(file, "# Coloque os links ou programas um abaixo do outro\n");
        fclose(file);
        printf("[INFO] Arquivo criado com sucesso!\n");
        printf("[INFO] Caminho do arquivo: %s\n", dirfile);
        printf("Deseja abrir o arquivo para editar agora? [S/N]: ");
        fgets(response, 50, stdin);
        response[0] = tolower(response[0]);

        if (response[0] == 's') {
            OpenNotepad();
        } else {
            printf("[INFO] Programa finalizado.\n");
            return 0;
        }
    }

    printf("[INFO] Arquivo encontrado. Lendo lista de alvos...\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        if (line[0] == '#') {
            continue;
        }

        printf("[INFO] Abrindo item: %s\n", line);
        sprintf(command, "start \"\" \"%s\"", line);

        system(command);
    }

    printf("\n[INFO] Todos os alvos foram abertos.\n");
    fclose(file);
    return 0;
} 