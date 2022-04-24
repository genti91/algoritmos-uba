#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *archivo = fopen("test.txt", "r");
    if (archivo == NULL)
    {
        printf("error");
    }

    char* linea;
    size_t tam;

    while ((getline(&linea, &tam, archivo)) != EOF)
    {
        printf("%s", linea);
    }
    free(linea);
    fclose(archivo);
    
    return 0;
}
