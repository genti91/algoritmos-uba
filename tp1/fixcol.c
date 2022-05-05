#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CANT_MIN_PARAMETROS 2
#define CANT_MAX_PARAMETROS 3
#define POS_MAX_CHAR_EN_ARGV 1
#define POS_NOMBRE_ARCH_EN_ARGV 2

bool chequear_parametros(int argc, char const *argv){
    if (argc < CANT_MIN_PARAMETROS || argc > CANT_MAX_PARAMETROS || !atoi(argv)){
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
    if (!chequear_parametros(argc, argv[POS_MAX_CHAR_EN_ARGV])){
        return -1;
    }
    
    int max_chars = atoi(argv[POS_MAX_CHAR_EN_ARGV]);
    FILE* texto = stdin;

    if (argc == CANT_MAX_PARAMETROS){
        texto = abrir_archivo(argv[POS_NOMBRE_ARCH_EN_ARGV]);
        if (texto == NULL){
            return -1;
        }
    }

    separar_texto(texto, max_chars);

    if (argc == CANT_MAX_PARAMETROS){
        fclose(texto);
    }

    return 0;
}