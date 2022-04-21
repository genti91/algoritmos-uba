#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    FILE* archivo = fopen(argv[2], "r");
    FILE* archivo_escrito = fopen("test.txt", "w");

    if (archivo == NULL || archivo_escrito == NULL){
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
            fputc('\n', archivo_escrito);
            contador = 1;
        }
        
        printf("%c", char_leido);
        fputc(char_leido, archivo_escrito);
        char_leido = fgetc(archivo);
        contador++;
    }

    fclose(archivo);
    fclose(archivo_escrito);
    
    return 0;
}