#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
    int atributos_iniciais, tamanho, deixar_resposta;
    int atributos_finais, i, j;
    FILE *fileInput, *fileOutput;
    float content;
    fileInput = fopen("diminuir.txt", "rt");
    fileOutput = fopen("diminuir-output.txt", "wt");
    if (fileInput == NULL || fileOutput == NULL)
    {
        printf("\nPROBLEMAS NOS ARQUIVOS\n\n");
        return 1;
    }
    fscanf(fileInput, "%d", &tamanho);
    fscanf(fileInput, "%d", &atributos_iniciais);
    fscanf(fileInput, "%d", &atributos_finais);
    fscanf(fileInput, "%d", &deixar_resposta);

    for (i = 0; i < tamanho; i++)
    {
        for (j = 0; j < atributos_iniciais; j++)
        {
            fscanf(fileInput, "%f", &content);
            if (j < atributos_finais)
            {
                fprintf(fileOutput, "%f ", content);
            }
            if(deixar_resposta == 1 && j == atributos_iniciais - 1) {
                fprintf(fileOutput, "%f ", content);
            }
        }
        fprintf(fileOutput, "\n");
    }

    fclose(fileInput);
    fclose(fileOutput);
    return 0;
}