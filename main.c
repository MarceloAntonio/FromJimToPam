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
        printf("[INFO] Abrindo arquivo para edicao...\n");
        sprintf(command, "start %s%s%s", userPath, CONFIG_SUBDIR, CONFIG_FILENAME);
        system(command);
        printf("[INFO] Finalizando\n");
        fclose(file);
        exit(0);
    }

    if (argc > 1 && strcmp(argv[1], "-e") == 0 && file != NULL) {
        OpenNotepad();
    }

    printf("[INFO] Verificando arquivo de configuracao: %s\n", dirfile);

    if (file == NULL) {
        printf("[INFO] Arquivo nao encontrado. Inicializando configuracao...\n");
        char response[50];

        char dirPath[MAX_PATH_LEN];
        sprintf(dirPath, "%s%s", userPath, CONFIG_SUBDIR);
        printf("[INFO] Criando diretorio: %s\n", dirPath);
        _mkdir(dirPath);

        file = fopen(dirfile, "w");
        fprintf(file, "# Coloque os links/programas um abaixo do outro");
        fclose(file);
        printf("[INFO] Arquivo criado com sucesso!\n");
        printf("Edite o arquivo %s em %s\n", CONFIG_FILENAME, CONFIG_SUBDIR);
        printf("Voce deseja editar o arquivo agora? \n(S/N): ");
        fgets(response, 50, stdin);
        response[0] = tolower(response[0]);

        if (response[0] == 's') {
            OpenNotepad();
        } else {
            printf("[INFO] Finalizando...\n");
            return 0;
        }
    }

    printf("[INFO] Arquivo encontrado! Lendo alvos para inicializacao...\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strlen(line) == 0) continue;

        if (line[0] == '#') {
            continue;
        }

        printf("[INFO] Iniciando alvo: %s", line);
        sprintf(command, "start \"\" \"%s\"", line);

        system(command);
    }

    printf("\n[INFO] Execucao concluida com sucesso!\n");
    fclose(file);
    return 0;
} 