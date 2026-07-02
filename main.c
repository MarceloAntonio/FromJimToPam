#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>
#include <windows.h>

#define CONFIG_SUBDIR "\\AppData\\Local\\FromJimToPam\\"
#define CONFIG_FILENAME "targets.conf"
#define MAX_PATH_LEN 512

    void BuildFilePath(char *buffer) {
        buffer[0] = '\0';
        char *userPath = getenv("USERPROFILE");
        if (userPath != NULL) {
            snprintf(buffer, MAX_PATH_LEN, "%s%s%s", userPath, CONFIG_SUBDIR, CONFIG_FILENAME);
        }
    }

    void OpenEditor(const char *pathFile) {
        printf("[INFO] Abrindo editor de texto...\n");
        ShellExecuteA(NULL, "open", "notepad.exe", pathFile, NULL, SW_SHOWNORMAL);
    }

    void CreateDefaultConfig(const char *PathFile) {
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
            OpenEditor(PathFile);
        } else {
            printf("[INFO] Programa finalizado.\n");
        }
    }

    void RunTargets(const char *PathFile) {
        FILE *file = fopen(PathFile, "r");
        if (file == NULL) {
            CreateDefaultConfig(PathFile);
            return;
        }
        
        printf("[INFO] Carregando alvos de: %s\n\n", PathFile);

        char line[MAX_PATH_LEN];
        int count = 0;
        while (fgets(line, sizeof(line), file) != NULL) {
            line[strcspn(line, "\r\n")] = 0;
            if (line[0] == '\0' || line[0] == '#') continue;

            count++;
            printf("[INFO] Executando [%02d]: %s\n", count, line);
            ShellExecuteA(NULL, "open", line, NULL, NULL, SW_SHOWNORMAL);
        }

        printf("\n[SUCESSO] %d alvo(s) executado(s) com sucesso!\n", count);
        fclose(file);
    }

    void ArgumentList(const char *pathFile) {
        FILE *file = fopen(pathFile, "r");
        if (file == NULL) {
            fprintf(stderr, "[ERRO] Arquivo de configuracao nao encontrado: %s\n", pathFile);
            return;
        }
        char line[MAX_PATH_LEN];
        int a = 0;
        printf("[INFO] Alvos cadastrados:\n\n");
        while (fgets(line, sizeof(line), file) != NULL) {
            line[strcspn(line, "\r\n")] = 0;
            if (line[0] == '\0' || line[0] == '#') continue;
            a++;
            printf("  [%02d] %s\n", a, line);
        }
        printf("\n[INFO] Total: %d alvo(s).\n", a);
        fclose(file);
    }

    void ArgumentAdd(const char *pathFile, int argc, char *argv[]) {
        FILE *file = fopen(pathFile, "a");
        if (file == NULL) {
            fprintf(stderr, "[ERRO] Erro ao abrir arquivo de configuracao para escrita.\n");
            return;
        }
        int adicionados = 0;
        for (int i = 2; i < argc; i++) {
            printf("[INFO] Adicionando alvo: %s\n", argv[i]);
            fprintf(file, "\n%s", argv[i]);
            adicionados++;
        }
        printf("[SUCESSO] %d alvo(s) adicionado(s) com sucesso!\n", adicionados);
        fclose(file);
    }

    void ArgumentRemove(const char *pathFile, int argc, char *argv[]) {
        FILE *in = fopen(pathFile, "r");
        FILE *out = fopen("temp.tmp", "w");
        if (!in || !out) {
            fprintf(stderr, "[ERRO] Falha ao manipular arquivos para remocao.\n");
            if (in) fclose(in);
            if (out) fclose(out);
            return;
        }

        char line[MAX_PATH_LEN];
        int a = 0;
        int removidos = 0;
        while (fgets(line, sizeof(line), in)) {
            if (line[0] == '#' || line[0] == '\r' || line[0] == '\n') { fputs(line, out); continue; }
            a++;
            int apagar = 0;
            for (int i = 2; i < argc; i++) if (atoi(argv[i]) == a) apagar = 1;

            if (apagar) {
                line[strcspn(line, "\r\n")] = 0;
                printf("[INFO] Removido item [%02d]: %s\n", a, line);
                removidos++;
            } else {
                fputs(line, out);
            }
        }
        fclose(in); fclose(out);
        remove(pathFile);
        rename("temp.tmp", pathFile);
        printf("[SUCESSO] %d item(ns) removido(s).\n", removidos);
    }

int main(int argc, char *argv[]) {

    char pathFile[MAX_PATH_LEN];

    BuildFilePath(pathFile);
  
    if (argc > 1 && strcmp(argv[1], "-e") == 0) {
        OpenEditor(pathFile);
        return 0;
    }
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        printf("FromJimToPam - Gerenciador Rapido de Rotinas\n\n");
        printf("USO:\n");
        printf("  FromJimToPam.exe [opcoes] [argumentos]\n\n");
        printf("OPCOES:\n");
        printf("  (sem flag)          Executa todos os alvos cadastrados\n");
        printf("  -a <alvo1> [alvo2]  Adiciona novo(s) alvo(s) na lista\n");
        printf("  -l                  Lista todos os alvos numerados\n");
        printf("  -r <id1> [id2]      Remove alvos pelos numeros de indice\n");
        printf("  -e                  Abre o arquivo de configuracao no editor\n");
        printf("  -h                  Exibe esta mensagem de ajuda\n");
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "-a") == 0) {
        ArgumentAdd(pathFile, argc, argv);
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "-l") == 0) {
        ArgumentList(pathFile);
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "-r") == 0) {
        ArgumentRemove(pathFile, argc, argv);
        return 0;
    }

    RunTargets(pathFile);
    return 0;
}