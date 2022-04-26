#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool chequear_parametros(int argc, char const *argv){
    if (argc < 2 || argc > 3 || !atoi(argv))
    {
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
    char* linea = NULL;
    size_t tam;

    while ((getline(&linea, &tam, texto)) != EOF)
    {   
        int contador = 1;
        for (size_t i = 0; i < strlen(linea); i++)
        {   
            if (linea[i] != '\n')
            {
                fputc(linea[i], stdout);
            }

            if (contador == max_chars)
            {
                fputc('\n', stdout);
                contador = 0;
            }
            
            contador++;
        }

        if (strlen(linea)-2 != max_chars)
        {
            fputc('\n', stdout);
                
        }
    }
    free(linea);
}

int main(int argc, char const *argv[])
{   
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