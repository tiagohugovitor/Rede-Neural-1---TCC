#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
    FILE *fileInput;
    float content1, content2, content3, answer;
    int contError1 = 0;
    int contError2 = 0;
    int contError3 = 0;
    int i;
    int errorClassificated1 = 0;
    int errorClassificated2 = 0;
    int errorClassificated3 = 0;
    int correct = 0;

    fileInput = fopen("executions-output.txt", "rt");
    if (fileInput == NULL)
    {
        printf("\nPROBLEMAS NOS ARQUIVOS\n\n");
        return 1;
    }

    for (i = 0; i < 150; i++)
    {
        fscanf(fileInput, "%f", &content1);
        fscanf(fileInput, "%f", &content2);
        fscanf(fileInput, "%f", &content3);
        if (i < 50)
        {
            if (content1 > content2 && content1 > content3)
            {
                correct++;
            }
            else
            {
                contError1++;
                if (content3 > content2 && content3 > content1)
                {
                    errorClassificated3++;
                }
                else
                {
                    if (content2 > content3 && content2 > content1) {
                        errorClassificated2++;
                    }
                }
            }
        }
        if (i > 49 && i < 100)
        {
            if (content2 > content1 && content2 > content3)
            {
                correct++;
            }
            else
            {
                contError2++;
                if (content3 > content2 && content3 > content1)
                {
                    errorClassificated3++;
                }
                else
                {
                    if (content1 > content3 && content1 > content2) {
                        errorClassificated1++;
                    }
                }
            }

        }
        if (i >= 100)
        {
            if (content3 > content1 && content3 > content2)
            {
                correct++;
            }
            else
            {
                contError3++;
                if (content1 > content2 && content1 > content3)
                {
                    errorClassificated1++;
                }
                else
                {
                    if (content2 > content3 && content2 > content1) {
                        errorClassificated2++;
                    }
                }
            }

        }
        
    }
    fclose(fileInput);
    printf("Correto: %d\n", correct);
    printf("contError1: %d\n", contError1);
    printf("contError2: %d\n", contError2);
    printf("contError3: %d\n", contError3);
    printf("errorClassificated1: %d\n", errorClassificated1);
    printf("errorClassificated2: %d\n", errorClassificated2);
    printf("errorClassificated3: %d\n", errorClassificated3);
    
    printf("TAXA DE ACERTO: %f", correct / 1.5);

    return 0;
}