<div align="center">
  <img src="img\FromJimToPam.jpg" alt="Logo" width="500">
  <h1>FromJimToPam</h1>
</div>

Um abridor de rotinas escrito em C para Windows. A ideia é simples: você coloca os links diários, pastas ou programas em um arquivo de configuração e o executável abre tudo de uma vez.

## Como funciona

O programa procura o arquivo `targets.conf` na pasta `%USERPROFILE%\AppData\Local\FromJimToPam\`.

Se a pasta ou o arquivo não existirem, ele cria a estrutura básica e pergunta se você quer abrir o bloco de notas para editar a lista.

Quando roda sem parâmetros, ele lê o arquivo linha por linha, ignora comentários que começam com `#` e chama a função `ShellExecuteA` do Windows para abrir cada item.

## Compilação

Para compilar usando o GCC no terminal do Windows:

```bash
gcc main.c icone.res -o FromJimToPam.exe
```

## Comandos

Você pode rodar o executável direto no terminal ou criar atalhos:

```text
USO:
  FromJimToPam.exe [opcoes] [argumentos]
```

### Opções disponíveis

| Opção | Argumentos | Descrição | Exemplo |
| :--- | :--- | :--- | :--- |
| *(sem flag)* | N/A | Executa todos os itens da lista | `FromJimToPam.exe` |
| **`-a`** | `<alvo1> [alvo2]` | Adiciona novos itens pelo terminal | `FromJimToPam.exe -a "https://github.com"` |
| **`-l`** | N/A | Lista os itens numerados | `FromJimToPam.exe -l` |
| **`-r`** | `<id1> [id2]` | Remove itens pelos números de índice | `FromJimToPam.exe -r 2 4` |
| **`-e`** | N/A | Abre o arquivo `targets.conf` no editor | `FromJimToPam.exe -e` |
| **`-h`** | N/A | Mostra os comandos disponíveis | `FromJimToPam.exe -h` |

## Formato do arquivo (`targets.conf`)

O arquivo aceita links da internet, diretórios ou executáveis locais. Coloque um item por linha:

```text
# Minhas rotinas
https://mail.google.com
https://github.com
C:\Program Files\Spotify\Spotify.exe
```

