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

void trainingAnn(Ann *ann)
{
    int i,j,k,l, ages, a, ageCount, dataTrainingAmount;
    float learningRate;
    //Receber dados de entrada
    printf("Digite a taxa de aprendizado: ");
    scanf("%f", &learningRate);
    printf("\n");
    printf("Digite a quantidade de epocas para treinar os dados: ");
    scanf("%d", &ages);
    printf("\n");
    printf("Digite a quantidade de dados que serão utilizados para o treinamento: ");
    scanf("%d", &dataTrainingAmount);
    printf("\n");
    float **trainingInput = (float **)malloc(dataTrainingAmount * sizeof(float *));
    float **trainingOutput = (float **)malloc(dataTrainingAmount * sizeof(float *));
    //Leitura dos dados de entrada
    for (i = 0; i < dataTrainingAmount; i++)
    {
        trainingInput[i] = (float *)malloc((ann->inputLayer + 1) * sizeof(float));
        trainingOutput[i] = (float *)malloc((ann->outputLayer + 1) * sizeof(float));
        for (j = 0; j < ann->inputLayer; j++)
        {
            printf("Digite a entrada %d do conjunto de treinamento %d: ", j, i);
            scanf("%f", &trainingInput[i][j]);
        }
        for (j = 0; j < ann->outputLayer; j++)
        {
            printf("Digite a saida %d esperada do conjunto de treinamento %d: ", j, i);
            scanf("%f", &trainingOutput[i][j]);
        }
    }

    for (ageCount = 0; ageCount < ages; ageCount++)
    {
        for (l = 0; l < dataTrainingAmount; l++)
        {
            //Execução da rede
            executeAnn(ann, trainingInput[l]);

            //Calculo dos deltas da ultima camada
            for (j = 0; j < ann->outputLayer; j++)
            {
                //printf("Calculo do erro: %f \n", trainingOutput[l][j] -  ann->network[ann->hiddenLayers + 1].neurons[j].sigmoidResult);
                ann->network[ann->hiddenLayers + 1].neurons[j].delta = (trainingOutput[l][j] - ann->network[ann->hiddenLayers + 1].neurons[j].sigmoidResult) * ann->network[ann->hiddenLayers + 1].neurons[j].sigmoidResult * (1 - ann->network[ann->hiddenLayers + 1].neurons[j].sigmoidResult);
            }

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
                //printf("j: %d", j);
                int nextLayerSize = (j == ann->hiddenLayers) ? ann->outputLayer : ann->hiddenLayersNeuronsAmount;
                int layerSize = (j == 0) ? ann->inputLayer : ann->hiddenLayersNeuronsAmount;
                for (k = 0; k < layerSize; k++)
                {

                    // printf("k: %d", k);
                    for (a = 0; a < nextLayerSize; a++)
                    {
                        ann->network[j].neurons[k].weights[a] = ann->network[j].neurons[k].weights[a] + (ann->network[j + 1].neurons[a].delta * ann->network[j].neurons[k].sigmoidResult * learningRate);
                        //   printf("Novo peso: %f\n", ann->network[j].neurons[k].weights[a]);
                    }
                    ann->network[j].neurons[k].bias = ann->network[j].neurons[k].bias + (ann->network[j].neurons[k].delta * learningRate);
                }
            }
            for (j = 0; j < ann->outputLayer; j++)
            {
                ann->network[ann->hiddenLayers + 1].neurons[j].bias = ann->network[ann->hiddenLayers + 1].neurons[j].bias + (ann->network[ann->hiddenLayers + 1].neurons[j].delta * learningRate);
            }
        }
    }
}

int main()
{
    Ann *ann = (Ann *)malloc(sizeof(Ann));
    int i;
    int menuOption = 1;

    //----------------------------- DECLARAÇÕES DE ENTRADA E SAÍDA ----------------------------- //

    printf("Insira a quantidade de neuronios na camada de entrada: ");
    scanf("%d", &ann->inputLayer);
    setbuf(stdin, NULL);
    printf("\n");

    printf("Insira a quantidade de camadas ocultas da rede: ");
    scanf("%d", &ann->hiddenLayers);
    setbuf(stdin, NULL);
    printf("\n");

    if (ann->hiddenLayers != 0)
    {
        printf("Insira a quantidade de neuronios em cada camada oculta da rede: ");
        scanf("%d", &ann->hiddenLayersNeuronsAmount);
        setbuf(stdin, NULL);
        printf("\n");
    }

    printf("Insira a quantidade de neuronios na camada de saída: ");
    scanf("%d", &ann->outputLayer);
    setbuf(stdin, NULL);
    printf("\n");

    //---------------------------- ALOCAÇÃO DA REDE NEURAL DINAMICAMENTE --------------------------- //

    //Alocar a rede neural
    alocateAnn(ann);

    //------------------------------------ MENU DE OPÇÕES ------------------------------------- //

    while (menuOption != 0)
    {
        if (menuOption == 1)
        {
            //------------------------------------ TREINAMENTO DA REDE ------------------------------------- //
            trainingAnn(ann);
        }
        else if (menuOption == 2)
        {
            //------------------------------------ EXECUÇÃO DA REDE ------------------------------------- //

            //Receber dados de entrada
            float *input = (float *)malloc(ann->inputLayer * sizeof(float));
            for (i = 0; i < ann->inputLayer; i++)
            {
                printf("Digite o valor de entrada %d:", i);
                scanf("%f", &input[i]);
            }
            //Executar
            executeAnn(ann, input);

            //Printar Saída de dados
            printf("SAÍDA DA REDE\n");
            for (i = 0; i < ann->outputLayer; i++)
            {
                printf("NEURONIO %d: %f\n", i, ann->network[ann->hiddenLayers + 1].neurons[i].sigmoidResult);
            }
        }

        printf("Escolha:\n0 - Sair\n1 - Treinar a rede\n2 - Testar a rede\n");
        scanf("%d", &menuOption);
    }
    return 1;
}