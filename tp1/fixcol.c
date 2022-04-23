#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char const *argv[])
{   
    if (argc < 2 || argc > 3)
    {
        perror("Error: Cantidad erronea de parametros\n");
        return -1;
    }

    int max_chars = atoi(argv[1]) + 1;

    if (argc == 2)
    {
        char linea[max_chars];

        while (fgets(linea, max_chars, stdin) != NULL)
        {   
            fputs(linea, stdout);

            bool tiene_enter = false;

            for (int i = 0; i < max_chars; i++){
                if (linea[i] == '\n'){
                    tiene_enter = true;
                }
            }

            if (!tiene_enter){
                fputc('\n', stdout);
            }
        }
        return 0;
    }

    FILE* archivo = fopen(argv[2], "r");

    if (archivo == NULL){
        perror("Error: archivo fuente inaccesible\n");
        return -1;
    }

    char linea[max_chars];

    while (fgets(linea, max_chars, archivo) != NULL)
    {
        fputs(linea, stdout);
        
        bool tiene_enter = false;

        for (int i = 0; i < max_chars; i++){
            if (linea[i] == '\n'){
                tiene_enter = true;
            }
        }

        if (!tiene_enter){
            fputc('\n', stdout);
        }

    }

    fclose(archivo);

    return 0;
}