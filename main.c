#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>
#include <windows.h>

#define CONFIG_SUBDIR "\\AppData\\Local\\FromJimToPam\\"
#define CONFIG_FILENAME "targets.conf"
#define MAX_PATH_LEN 512

    void MontarPathFile(char *buffer) {
        buffer[0] = '\0';
        char *userPath = getenv("USERPROFILE");
        if (userPath != NULL) {
            snprintf(buffer, MAX_PATH_LEN, "%s%s%s", userPath, CONFIG_SUBDIR, CONFIG_FILENAME);
        }
    }

    void OpenNotepad(const char *pathFile) {
        printf("[INFO] Abrindo editor de texto...\n");
        ShellExecuteA(NULL, "open", "notepad.exe", pathFile, NULL, SW_SHOWNORMAL);
    }

    void FileDontFinded(const char *PathFile) {
        printf("[INFO] Arquivo nao encontrado. Criando configuracao inicial...\n");
        char PathAppData[MAX_PATH_LEN];
        snprintf(PathAppData, sizeof(PathAppData), "%s%s", getenv("USERPROFILE"), CONFIG_SUBDIR);
        printf("[INFO] Criando pasta: %s\n", PathAppData);
        _mkdir(PathAppData);
        
        FILE *file = fopen(PathFile, "w");
        if (file == NULL) {
            fprintf(stderr, "[ERRO] Erro ao criar o arquivo de configuracao: %s\n", PathFile);
            return;
        }
        fprintf(file, "# Coloque os links ou programas um abaixo do outro\n");
        fclose(file);

        printf("[INFO] Arquivo criado com sucesso!\n");
        printf("[INFO] Caminho do arquivo: %s\n", PathFile);
        printf("Deseja abrir o arquivo para editar agora? [S/N]: ");
        if (tolower(fgetc(stdin)) == 's') {
            OpenNotepad(PathFile);
        } else {
            printf("[INFO] Programa finalizado.\n");
        }
    }

    void Scanning(const char *PathFile) {
        FILE *file = fopen(PathFile, "r");
        if (file == NULL) {
            FileDontFinded(PathFile);
            return;
        }
        
        printf("[INFO] Lendo lista de alvos em: %s\n", PathFile);

        char line[MAX_PATH_LEN];
        while (fgets(line, sizeof(line), file) != NULL) {
            line[strcspn(line, "\r\n")] = 0;
            if (line[0] == '\0' || line[0] == '#') continue;

            printf("[INFO] Abrindo item: %s\n", line);
            ShellExecuteA(NULL, "open", line, NULL, NULL, SW_SHOWNORMAL);
        }

        printf("\n[INFO] Todos os alvos foram abertos.\n");
        fclose(file);
    }
    
int main(int argc, char *argv[]) {

    char pathFile[MAX_PATH_LEN];

    MontarPathFile(pathFile);
 
    if (argc > 1 && strcmp(argv[1], "-e") == 0) {
        OpenNotepad(pathFile);
        return 0;
    }
    
    Scanning(pathFile);
    
    return 0;
} 