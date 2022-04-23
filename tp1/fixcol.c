#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{   
    if (!atoi(argv[1]))
    {
        perror("Error: Cantidad erronea de parametros\n");
        return -1;
    }
    
    int parametro = argc;

    /*if (strcmp(argv[2], "<"))
    {
        parametro = 3;
    }*/

    printf("%d", parametro);
    return -1;
    
    printf("%s", argv[parametro]);
    return -1;

    FILE* archivo = fopen(argv[parametro], "r");
    FILE* archivo_fixed = fopen("fixed.txt", "w");

    if (archivo == NULL || archivo_fixed == NULL){
        perror("Error: Error: archivo fuente inaccesible\n");
        return -1;
    }

    int char_leido = fgetc(archivo);
    int contador = 1;

    while (char_leido != EOF)
    {   
        if (contador == atoi(argv[1]) + 1)
        {
            printf("\n");
            fputc('\n', archivo_fixed);
            contador = 1;
        }
        
        printf("%c", char_leido);
        fputc(char_leido, archivo_fixed);
        char_leido = fgetc(archivo);
        contador++;
    }

    
    fclose(archivo_fixed);
    fclose(archivo);
    
    return 0;
}