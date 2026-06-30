# FromJimToPam

Um abridor rápido de rotinas escrito em C para Windows. A ideia é simples: você coloca todos os seus links diários, pastas ou programas em um arquivo de texto, e o executável abre tudo de uma vez sem gastar memória do computador.

## Como funciona

O código (main.c) faz tudo de forma bem direta:

1. Procura o arquivo targets.conf na pasta AppData\Local\FromJimToPam\ do seu usuário.
2. Se a pasta ou o arquivo não existirem, ele cria a estrutura automaticamente e pergunta se você já quer abrir o Bloco de Notas para colocar seus links.
3. Quando roda normal, ele lê o arquivo linha por linha, ignora espaços em branco ou comentários que começam com #, e executa cada item usando o comando start do Windows.

## Compilação

Para compilar usando o GCC no terminal do Windows:

```bash
gcc main.c icone.res -o FromJimToPam.exe
```

## Comandos

Rodar normal para abrir todos os itens da sua lista:
```bash
FromJimToPam.exe
```

Usar a flag de edição para abrir direto no Bloco de Notas:
```bash
FromJimToPam.exe -e
```

