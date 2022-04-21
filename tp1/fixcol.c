#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    FILE* archivo = fopen(argv[2], "r");
    FILE* archivo_auxiliar = fopen("archivo_auxiliar.txt", "w");

    if (archivo == NULL || archivo_auxiliar == NULL){
        perror("No se pudo abrir el archivo!\n");
        return -1;
    }

    int char_leido = fgetc(archivo);
    int contador = 1;

    while (char_leido != EOF)
    {   
        if (contador == atoi(argv[1]) + 1)
        {
            printf("\n");
            fputc('\n', archivo_auxiliar);
            contador = 1;
        }
        
        printf("%c", char_leido);
        fputc(char_leido, archivo_auxiliar);
        char_leido = fgetc(archivo);
        contador++;
    }

    
    fclose(archivo_auxiliar);
    //rename("archivo_auxiliar.txt", argv[2]);
    fclose(archivo);
    
    return 0;
}