#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct neuron
{
    float *input;
    float *weights;
    float inputSum;
    float sigmoidResult;
    float bias;
    float delta;
} Neuron;

typedef struct layer
{
    Neuron *neurons;
} Layer;

typedef struct ann
{
    int inputLayer;
    int outputLayer;
    int hiddenLayers;
    int hiddenLayersNeuronsAmount;
    float learningRate;
    int ages;
    float minimumError;
    Layer *network;
} Ann;

float sigmoide(float x)
{
    float result = 1 / (1 + exp(-x));
    return result;
}

void alocateAnn(Ann *ann)
{
    int i, j, k;
    ann->network = (Layer *)malloc((ann->hiddenLayers + 2) * sizeof(Layer));

    //Alocando entrada da rede
    ann->network[0].neurons = (Neuron *)malloc(ann->inputLayer * sizeof(Neuron));
    for (i = 0; i < ann->inputLayer; i++)
    {
        ann->network[0].neurons[i].inputSum = 0;
        ann->network[0].neurons[i].bias = 0;
        int weightsSize = (ann->hiddenLayers == 0) ? ann->outputLayer : ann->hiddenLayersNeuronsAmount;
        ann->network[0].neurons[i].weights = (float *)malloc(weightsSize * sizeof(float));
        for (j = 0; j < weightsSize; j++)
        {
            ann->network[0].neurons[i].weights[j] = (rand() % 10000) / 10000.0;
        }
    }

    //Alocando camadas ocultas
    if (ann->hiddenLayers != 0)
    {
        int previousLayerSize;
        int nextLayerSize;
        for (i = 1; i < ann->hiddenLayers + 1; i++)
        {
            previousLayerSize = (i == 1) ? ann->inputLayer : ann->hiddenLayersNeuronsAmount;
            nextLayerSize = (i == ann->hiddenLayers) ? ann->outputLayer : ann->hiddenLayersNeuronsAmount;
            ann->network[i].neurons = (Neuron *)malloc(ann->hiddenLayersNeuronsAmount * sizeof(Neuron));
            for (j = 0; j < ann->hiddenLayersNeuronsAmount; j++)
            {
                ann->network[i].neurons[j].inputSum = 0;
                ann->network[i].neurons[j].bias = 0;
                ann->network[i].neurons[j].delta = 0;
                ann->network[i].neurons[j].input = (float *)malloc(previousLayerSize * sizeof(float));
                ann->network[i].neurons[j].weights = (float *)malloc(nextLayerSize * sizeof(float));
                for (k = 0; k < nextLayerSize; k++)
                {
                    ann->network[i].neurons[j].weights[k] = (rand() % 10000) / 10000.0;
                }
            }
        }
    }

    //Alocando saída da rede
    ann->network[ann->hiddenLayers + 1].neurons = (Neuron *)malloc(ann->outputLayer * sizeof(Neuron));
    for (i = 0; i < ann->outputLayer; i++)
    {
        ann->network[ann->hiddenLayers + 1].neurons[i].inputSum = 0;
        ann->network[ann->hiddenLayers + 1].neurons[i].bias = 0;
        ann->network[ann->hiddenLayers + 1].neurons[i].delta = 0;
        ann->network[ann->hiddenLayers + 1].neurons[i].input = (float *)malloc(ann->hiddenLayersNeuronsAmount * sizeof(float));
    }
}

void executeAnn(Ann *ann, float *input)
{
    int i, j, k;
    //Propagação de dados para execução

    //Camada de entrada
    for (j = 0; j < ann->inputLayer; j++)
    {
        ann->network[0].neurons[j].sigmoidResult = input[j];
        int nextLayerSize = (ann->hiddenLayers == 0) ? ann->outputLayer : ann->hiddenLayersNeuronsAmount;
        for (k = 0; k < nextLayerSize; k++)
        {
            ann->network[1].neurons[k].input[j] = ann->network[0].neurons[j].sigmoidResult * ann->network[0].neurons[j].weights[k];
        }
    }

    //Camadas ocultas
    if (ann->hiddenLayers != 0)
    {
        for (i = 1; i < ann->hiddenLayers + 1; i++)
        {
            int previousLayerSize = (i == 1) ? ann->inputLayer : ann->hiddenLayersNeuronsAmount;
            int nextLayerSize = (i == ann->hiddenLayers) ? ann->outputLayer : ann->hiddenLayersNeuronsAmount;
            for (j = 0; j < ann->hiddenLayersNeuronsAmount; j++)
            {
                ann->network[i].neurons[j].inputSum = 0;
                for (k = 0; k < previousLayerSize; k++)
                {
                    ann->network[i].neurons[j].inputSum += ann->network[i].neurons[j].input[k];
                }
                ann->network[i].neurons[j].sigmoidResult = sigmoide(ann->network[i].neurons[j].inputSum + ann->network[i].neurons[j].bias);
                for (k = 0; k < nextLayerSize; k++)
                {
                    ann->network[i + 1].neurons[k].input[j] = (ann->network[i].neurons[j].sigmoidResult * ann->network[i].neurons[j].weights[k]);
                }
            }
        }
    }

    //Camada de saída
    int previousLayerSize = (ann->hiddenLayers == 0) ? ann->inputLayer : ann->hiddenLayersNeuronsAmount;
    for (j = 0; j < ann->outputLayer; j++)
    {
        ann->network[ann->hiddenLayers + 1].neurons[j].inputSum = 0;
        for (k = 0; k < previousLayerSize; k++)
        {
            ann->network[ann->hiddenLayers + 1].neurons[j].inputSum += ann->network[ann->hiddenLayers + 1].neurons[j].input[k];
        }
        ann->network[ann->hiddenLayers + 1].neurons[j].sigmoidResult = sigmoide(ann->network[ann->hiddenLayers + 1].neurons[j].inputSum + ann->network[ann->hiddenLayers + 1].neurons[j].bias);
    }
}

void trainingAnn(Ann *ann, FILE *fileInput)
{
    int i, j, k, l, a, ageCount, dataTrainingAmount;
    //Receber dados de entrada
    fscanf(fileInput, "%f", &ann->learningRate);
    fscanf(fileInput, "%d", &ann->ages);
    fscanf(fileInput, "%f", &ann->minimumError);
    fscanf(fileInput, "%d", &dataTrainingAmount);
    float **trainingInput = (float **)malloc(dataTrainingAmount * sizeof(float *));
    float **trainingOutput = (float **)malloc(dataTrainingAmount * sizeof(float *));
    //Leitura dos dados de entrada
    for (i = 0; i < dataTrainingAmount; i++)
    {
        trainingInput[i] = (float *)malloc((ann->inputLayer + 1) * sizeof(float));
        trainingOutput[i] = (float *)malloc((ann->outputLayer + 1) * sizeof(float));
        for (j = 0; j < ann->inputLayer; j++)
        {
            fscanf(fileInput, "%f", &trainingInput[i][j]);
        }
        for (j = 0; j < ann->outputLayer; j++)
        {
            fscanf(fileInput, "%f", &trainingOutput[i][j]);
        }
    }

    for (ageCount = 0; ageCount < ann->ages; ageCount++)
    {
        float maxError = 0;
        for (l = 0; l < dataTrainingAmount; l++)
        {
            //Execução da rede
            executeAnn(ann, trainingInput[l]);
            float error = 0;

            //Calculo dos deltas da ultima camada
            for (j = 0; j < ann->outputLayer; j++)
            {
                ann->network[ann->hiddenLayers + 1].neurons[j].delta = (trainingOutput[l][j] - ann->network[ann->hiddenLayers + 1].neurons[j].sigmoidResult) * ann->network[ann->hiddenLayers + 1].neurons[j].sigmoidResult * (1 - ann->network[ann->hiddenLayers + 1].neurons[j].sigmoidResult);
                error += (trainingOutput[l][j] - ann->network[ann->hiddenLayers + 1].neurons[j].sigmoidResult) * (trainingOutput[l][j] - ann->network[ann->hiddenLayers + 1].neurons[j].sigmoidResult);
            }
            error = error / 2;
            if (error > maxError)
            {
                maxError = error;
            }
            if (error >= ann->minimumError)
            {

                //Calculo dos deltas das camadas intermediarias
                for (j = ann->hiddenLayers; j > 0; j--)
                {
                    int nextLayerSize = (j == ann->hiddenLayers) ? ann->outputLayer : ann->hiddenLayersNeuronsAmount;
                    for (k = 0; k < ann->hiddenLayersNeuronsAmount; k++)
                    {
                        ann->network[j].neurons[k].delta = 0;
                        for (a = 0; a < nextLayerSize; a++)
                        {
                            ann->network[j].neurons[k].delta += ann->network[j + 1].neurons[a].delta * ann->network[j].neurons[k].weights[a];
                        }
                        ann->network[j].neurons[k].delta = ann->network[j].neurons[k].delta * ann->network[j].neurons[k].sigmoidResult * (1 - ann->network[j].neurons[k].sigmoidResult);
                    }
                }

                //Atualizaçao dos pesos e dos bias
                for (j = 0; j < ann->hiddenLayers + 1; j++)
                {
                    int nextLayerSize = (j == ann->hiddenLayers) ? ann->outputLayer : ann->hiddenLayersNeuronsAmount;
                    int layerSize = (j == 0) ? ann->inputLayer : ann->hiddenLayersNeuronsAmount;
                    for (k = 0; k < layerSize; k++)
                    {
                        for (a = 0; a < nextLayerSize; a++)
                        {
                            ann->network[j].neurons[k].weights[a] = ann->network[j].neurons[k].weights[a] + (ann->network[j + 1].neurons[a].delta * ann->network[j].neurons[k].sigmoidResult * ann->learningRate);
                        }
                        ann->network[j].neurons[k].bias = ann->network[j].neurons[k].bias + (ann->network[j].neurons[k].delta * ann->learningRate);
                    }
                }
                for (j = 0; j < ann->outputLayer; j++)
                {
                    ann->network[ann->hiddenLayers + 1].neurons[j].bias = ann->network[ann->hiddenLayers + 1].neurons[j].bias + (ann->network[ann->hiddenLayers + 1].neurons[j].delta * ann->learningRate);
                }
            }
        }
        if (maxError < ann->minimumError)
        {
            ageCount = ann->ages;
        }
    }
}

void multiplesExecutions(Ann *ann)
{
    FILE *fileInput, *fileOutput;
    float *input;
    int i, j, executionsAmount;

    fileInput = fopen("executions.txt", "rt");
    fileOutput = fopen("executions-output.txt", "wt");
    input = (float *)malloc(ann->inputLayer * sizeof(float));
    if (fileInput == NULL || fileOutput == NULL)
    {
        printf("\nPROBLEMAS NOS ARQUIVOS\n\n");
        return;
    }
    //Receber dados de entrada
    fscanf(fileInput, "%d", &executionsAmount);
    for (j = 0; j < executionsAmount; j++)
    {
        for (i = 0; i < ann->inputLayer; i++)
        {
            fscanf(fileInput, "%f", &input[i]);
        }
        //Executar
        executeAnn(ann, input);

        //Escrever Saída de dados
        for (i = 0; i < ann->outputLayer; i++)
        {
            fprintf(fileOutput, "%f ", ann->network[ann->hiddenLayers + 1].neurons[i].sigmoidResult);
        }
        fprintf(fileOutput, "\n");
    }
    printf("\nA Rede gerou um arquivo execution-output com as respostas!!\n\n");
    fclose(fileInput);
    fclose(fileOutput);
}

void importAnn(Ann *ann, FILE *fileInput)
{
    int i, j, k;
    fscanf(fileInput, "%f", &ann->learningRate);
    fscanf(fileInput, "%d", &ann->ages);
    fscanf(fileInput, "%f", &ann->minimumError);

    for (i = 0; i < ann->hiddenLayers + 1; i++)
    {
        int nextLayerSize = i == ann->hiddenLayers ? ann->outputLayer : ann->hiddenLayersNeuronsAmount;
        int layerSize = i == 0 ? ann->inputLayer : ann->hiddenLayersNeuronsAmount;
        for (j = 0; j < layerSize; j++)
        {
            for (k = 0; k < nextLayerSize; k++)
            {
                fscanf(fileInput, "%f", &ann->network[i].neurons[j].weights[k]);
            }
            fscanf(fileInput, "%f", &ann->network[i].neurons[j].bias);
        }
    }

    for (j = 0; j < ann->outputLayer; j++)
    {
        fscanf(fileInput, "%f", &ann->network[ann->hiddenLayers + 1].neurons[j].bias);
    }

    printf("\nA Rede foi importada com sucesso!\n\n");
}

void exportAnn(Ann *ann)
{
    int i, j, k;
    FILE *fileOutput;
    fileOutput = fopen("saved-ann.txt", "wt");
    if (fileOutput == NULL)
    {
        printf("\nPROBLEMAS NA CRIACAO DO ARQUIVO\n\n");
        return;
    }
    fprintf(fileOutput, "%d\n", ann->inputLayer);
    fprintf(fileOutput, "%d\n", ann->hiddenLayers);
    if (ann->hiddenLayers > 0)
    {
        fprintf(fileOutput, "%d\n", ann->hiddenLayersNeuronsAmount);
    }
    fprintf(fileOutput, "%d\n", ann->outputLayer);

    fprintf(fileOutput, "%f\n", ann->learningRate);
    fprintf(fileOutput, "%d\n", ann->ages);
    fprintf(fileOutput, "%f\n", ann->minimumError);

    for (i = 0; i < ann->hiddenLayers + 1; i++)
    {
        int nextLayerSize = i == ann->hiddenLayers ? ann->outputLayer : ann->hiddenLayersNeuronsAmount;
        int layerSize = i == 0 ? ann->inputLayer : ann->hiddenLayersNeuronsAmount;
        for (j = 0; j < layerSize; j++)
        {
            for (k = 0; k < nextLayerSize; k++)
            {
                fprintf(fileOutput, "%f ", ann->network[i].neurons[j].weights[k]);
            }
            fprintf(fileOutput, "%f ", ann->network[i].neurons[j].bias);
            fprintf(fileOutput, "\n");
        }
    }

    for (j = 0; j < ann->outputLayer; j++)
    {
        fprintf(fileOutput, "%f ", ann->network[ann->hiddenLayers + 1].neurons[j].bias);
    }

    printf("\nA Rede foi exportada no arquivo saved-ann.txt com sucesso!\n\n");
    fclose(fileOutput);
}

int main()
{
    Ann *ann = (Ann *)malloc(sizeof(Ann));
    int i, fileOption = 0;
    int menuOption = 2;
    FILE *fileInput;
    float *input;

    while (fileOption != 1 && fileOption != 2)
    {
        printf("\nVoce deseja importar uma rede neural ou criar uma nova?\n 1- Importar\n 2- Criar Nova\n\nOpcao: ");
        scanf("%d", &fileOption);

        if (fileOption == 1)
        {
            fileInput = fopen("saved-ann.txt", "rt");
        }
        else if (fileOption == 2)
        {
            fileInput = fopen("training.txt", "rt");
        }
    }

    if (fileInput == NULL)
    {
        printf("PROBLEMAS NA ABERTURA DO ARQUIVO");
        return 1;
    }
    //----------------------------- DECLARAÇÕES DE ENTRADA E SAÍDA ----------------------------- //
    fscanf(fileInput, "%d", &ann->inputLayer);
    fscanf(fileInput, "%d", &ann->hiddenLayers);
    if (ann->hiddenLayers != 0)
    {
        fscanf(fileInput, "%d", &ann->hiddenLayersNeuronsAmount);
    }
    fscanf(fileInput, "%d", &ann->outputLayer);

    //---------------------------- ALOCAÇÃO DA REDE NEURAL DINAMICAMENTE --------------------------- //

    //Alocar a rede neural
    alocateAnn(ann);

    //Inicializar os pesos se já tiver uma rede neural criada
    if (fileOption == 1)
    {
        importAnn(ann, fileInput);
    }
    else if (fileOption == 2)
    {
        //------------------------------------ TREINAMENTO DA REDE ------------------------------------- //
        trainingAnn(ann, fileInput);
    }

    fclose(fileInput);
    //---------------------------------------- MENU DE OPÇÕES ------------------------------------- //
    do
    {
        printf("Escolha uma das opcoes:\n\n0 - Sair\n1 - Testar a rede\n2 - Executar varias entradas\n3 - Exportar a rede\n\nOpcao: ");
        scanf("%d", &menuOption);
        switch (menuOption)
        {
        case 1:
            //------------------------------------ EXECUÇÃO DA REDE ------------------------------------- //

            //Receber dados de entrada
            input = (float *)malloc(ann->inputLayer * sizeof(float));
            for (i = 0; i < ann->inputLayer; i++)
            {
                printf("Digite o valor de entrada %d:", i);
                scanf("%f", &input[i]);
            }
            //Executar
            executeAnn(ann, input);

            //Printar Saída de dados
            printf("\n\nSAIDA DA REDE\n");
            for (i = 0; i < ann->outputLayer; i++)
            {
                printf("NEURONIO %d: %f\n", i, ann->network[ann->hiddenLayers + 1].neurons[i].sigmoidResult);
            }
            break;
        case 2:
            //-------------------------- EXECUÇÃO DA REDE A PARTIR DE UM ARQUIVO -------------------------- //
            multiplesExecutions(ann);
            break;
        case 3:
            //------------------------------------ EXPORTAÇÃO DA REDE ------------------------------------- //
            exportAnn(ann);
            break;
        default:
            break;
        }
    } while (menuOption != 0);

    return 1;
}