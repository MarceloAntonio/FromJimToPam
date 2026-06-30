#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>

int main(int argc, char *argv[]) {



//Variables
    char *userPath = getenv("USERPROFILE");
    FILE *file;
    char command[256];
    char line[100];   
    char dirfile[256];
    sprintf(dirfile, "%s\\AppData\\Local\\FromJimToPam\\targets.conf", userPath);
    file = fopen(dirfile,"r");
    

//Functions
    void OpenNotepad(){
            printf("[INFO] Abrindo arquivo para edicao...\n");
            sprintf(command,"start %s\\AppData\\Local\\FromJimToPam\\targets.conf", userPath);
            system(command);
            printf("[INFO] Finalizando\n");
            exit(0);
    }

//Code
        if (argc > 1 && strcmp(argv[1], "-e") == 0 && file != NULL) {
        OpenNotepad();
    }
    
    printf("[INFO] Verificando arquivo de configuracao: %s\n", dirfile);
    
    //se arquivo ainda n existe
    if (file == NULL){
        printf("[INFO] Arquivo nao encontrado. Inicializando configuracao...\n");
        char response[50];  

        char dirPath[256];
        sprintf(dirPath, "%s\\AppData\\Local\\FromJimToPam", userPath);
        printf("[INFO] Criando diretorio: %s\n", dirPath);
        _mkdir(dirPath);


         file = fopen(dirfile, "w");
         fprintf(file, "# Coloque os links/programas um abaixo do outro");
         fclose(file);
         printf("[INFO] Arquivo criado com sucesso!\n");
         printf("Edite o arquivo targets.conf em AppData\\Local\\FromJimToPam\\targets.conf\n");
         printf("Voce deseja editar o arquivo agora? \n(S/N): ");
         fgets(response, 50, stdin);
         response[0] = tolower(response[0]);

         if (response[0] == 's'){
            OpenNotepad();
         }
         else{
            printf("[INFO] Finalizando...\n");
            return 0;
         }


    }


    printf("[INFO] Arquivo encontrado! Lendo alvos para inicializacao...\n");
    
    while (fgets(line, sizeof(line), file) != NULL) {    
        if(strlen(line) == 0) continue;
        
        if (line[0] == '#'){
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