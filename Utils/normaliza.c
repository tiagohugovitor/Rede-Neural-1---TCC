#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
    int tamanho = 0;
    int atributos;
    FILE *fileInput, *fileOutput;
    fileInput = fopen("normalize.txt", "rt");
    fileOutput = fopen("normalize-output.txt", "wt");
    if (fileInput == NULL || fileOutput == NULL)
    {
        printf("\nPROBLEMAS NOS ARQUIVOS\n\n");
        return 1;
    }
    fscanf(fileInput, "%d", &atributos);
    fscanf(fileInput, "%d", &tamanho);
    float max[atributos];
    float min[atributos];
    int i, j;
    float result;

    float content = 0.0;
    for (j = 0; j < atributos; j++)
    {
        min[j] = 0;
        max[j] = 0;
    }

    for (i = 0; i < tamanho; i++)
    {
        for (j = 0; j < atributos; j++)
        {

            fscanf(fileInput, "%f", &content);
            if (i == 0)
            {
                min[j] = content;
                max[j] = content;
            }
            if (content < min[j])
            {
                min[j] = content;
            }
            else if (content > max[j])
            {
                max[j] = content;
            }
        }
    }
    fclose(fileInput);
    fileInput = fopen("normalize.txt", "rt");
    if (fileInput == NULL)
    {
        printf("\nPROBLEMAS NOS ARQUIVOS\n\n");
        return 1;
    }
    
    fscanf(fileInput, "%d", &atributos);
    fscanf(fileInput, "%d", &tamanho);
    
    for (i = 0; i < tamanho; i++)
    {
        for (j = 0; j < atributos; j++)
        {
            fscanf(fileInput, "%f", &content);
            result = (content - min[j]) / (max[j] - min[j]);
            fprintf(fileOutput, "%f ", result);
        }
        fprintf(fileOutput, "\n");
    }
    fclose(fileInput);
    fclose(fileOutput);
    return 0;
}