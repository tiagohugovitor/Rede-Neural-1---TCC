#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
    FILE *fileInput, *fileAnswer;
    float content, answer;
    int cont, i;
    int correct = 0;

    fileInput = fopen("executions-output.txt", "rt");
    fileAnswer = fopen("answers.txt", "rt");
    if (fileInput == NULL || fileAnswer == NULL)
    {
        printf("\nPROBLEMAS NOS ARQUIVOS\n\n");
        return 1;
    }

    for (i = 0; i < 800; i++)
    {
        fscanf(fileInput, "%f", &content);
        fscanf(fileAnswer, "%f", &answer);
        if(content > 0.5 && answer == 1.000000 ) {
            correct ++;
        }
        if(content < 0.5 && answer == 0.000000 ) {
            correct ++;
        }
    }
    fclose(fileInput);
    fclose(fileAnswer);

    printf("TAXA DE ACERTO: %f", correct/8.0);

    return 0;
}