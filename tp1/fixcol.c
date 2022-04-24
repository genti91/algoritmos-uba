#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char const *argv[])
{   
    /*if (argc < 2 || argc > 3 || !atoi(argv[1]))
    {
        fputs("Error: Cantidad erronea de parametros\n", stderr);
        return -1;
    }*/
    printf("amobu %s", argv[1]);
    //int max_chars = atoi(argv[1]) + 1;
    
    //if (argc == 2)
    //{
        char* linea;
        size_t tam;
        while ((getline(&linea, &tam, stdin)) != EOF)
        {
            printf("%s", linea);
        }
        free(linea);
        return 0;
    //}



    /*if (argc == 2)
    {
        char cadena[max_chars];

        while (fgets(cadena, max_chars, stdin) != NULL)
        {   
            fputs(cadena, stdout);

            bool tiene_enter = false;

            for (int i = 0; i < max_chars; i++){
                if (cadena[i] == '\n'){
                    tiene_enter = true;
                }
            }
            if (!tiene_enter){
                fputc('\n', stdout);
            }
        }
        return 0;
    }*/

/*    FILE* archivo = fopen(argv[2], "r");

    if (archivo == NULL){
        fputs("Error: archivo fuente inaccesible\n", stderr);
        return -1;
    }

    char cadena[max_chars];

    while (fgets(cadena, max_chars, archivo) != NULL)
    {
        fputs(cadena, stdout);
        
        bool tiene_enter = false;

        for (int i = 0; i < max_chars; i++){
            if (cadena[i] == '\n'){
                tiene_enter = true;
            }
        }

        if (!tiene_enter && strlen(cadena) != max_chars-1){
            fputc('\n', stdout);
        }

    }

    fclose(archivo);
*/
    return 0;
}