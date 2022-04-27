#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool chequear_parametros(int argc, char const *argv){
    if (argc < 2 || argc > 3 || !atoi(argv)){
        fputs("Error: Cantidad erronea de parametros\n", stderr);
        return false;
    }
    return true;
}

FILE* abrir_archivo(char const *nombre_de_archivo){
    FILE* archivo = fopen(nombre_de_archivo, "r");
    if (archivo == NULL){
        fputs("Error: archivo fuente inaccesible\n", stderr);
        return NULL;
    }
    return archivo;
}

void separar_texto(FILE* texto, int max_chars){
    char renglon[max_chars];

    while ((fgets(renglon, max_chars+1, texto)) != NULL){

        if (renglon[0] != '\n'){
            fputs(renglon, stdout);

            if (renglon[strlen(renglon)-1] != '\n'){
                fputc('\n', stdout);
            }
        }
    }
}

int main(int argc, char const *argv[]){
    if (!chequear_parametros(argc, argv[1])){
        return -1;
    }
    
    int max_chars = atoi(argv[1]);
    FILE* texto = stdin;

    if (argc == 3){
        texto = abrir_archivo(argv[2]);
        if (texto == NULL){
            return -1;
        }
    }

    separar_texto(texto, max_chars);

    if (argc == 3){
        fclose(texto);
    }

    return 0;
}