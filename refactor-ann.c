#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float sigmoide(float x)
{
    float result = 1 / (1 + exp(-x));
    return result;
}

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

int main()
{
    int i, j, k, l, ageCount, a;
    int inputLayer;
    int outputLayer;
    int hiddenLayers;
    int hiddenLayersNeuronsAmount;
    int menuOption = 1;
    int ages;
    int dataTrainingAmount;
    float learningRate;

    //----------------------------- DECLARAÇÕES DE ENTRADA E SAÍDA ----------------------------- //

    printf("Insira a quantidade de neuronios na camada de entrada: ");
    scanf("%d", &inputLayer);
    setbuf(stdin, NULL);
    printf("\n");

    printf("Insira a quantidade de camadas ocultas da rede: ");
    scanf("%d", &hiddenLayers);
    setbuf(stdin, NULL);
    printf("\n");

    if (hiddenLayers != 0)
    {
        printf("Insira a quantidade de neuronios em cada camada oculta da rede: ");
        scanf("%d", &hiddenLayersNeuronsAmount);
        setbuf(stdin, NULL);
        printf("\n");
    }

    printf("Insira a quantidade de neuronios na camada de saída: ");
    scanf("%d", &outputLayer);
    setbuf(stdin, NULL);
    printf("\n");

    //---------------------------- ALOCAÇÃO DA REDE NEURAL DINAMICAMENTE --------------------------- //

    //Alocar a rede neural
    Layer *network;

    network = (Layer *)malloc((hiddenLayers + 2) * sizeof(Layer));

    //Alocando entrada da rede
    network[0].neurons = (Neuron *)malloc(inputLayer * sizeof(Neuron));
    for (i = 0; i < inputLayer; i++)
    {
        network[0].neurons[i].inputSum = 0;
        network[0].neurons[i].bias = 0;
        int weightsSize = (hiddenLayers == 0) ? outputLayer : hiddenLayersNeuronsAmount;
        network[0].neurons[i].weights = (float *)malloc(weightsSize * sizeof(float));
        for (j = 0; j < weightsSize; j++)
        {
            network[0].neurons[i].weights[j] = (rand() % 10000) / 10000.0;
        }
    }

    //Alocando camadas ocultas
    if (hiddenLayers != 0)
    {
        int previousLayerSize;
        int nextLayerSize;
        for (i = 1; i < hiddenLayers + 1; i++)
        {
            previousLayerSize = (i == 1) ? inputLayer : hiddenLayersNeuronsAmount;
            nextLayerSize = (i == hiddenLayers) ? outputLayer : hiddenLayersNeuronsAmount;
            network[i].neurons = (Neuron *)malloc(hiddenLayersNeuronsAmount * sizeof(Neuron));
            for (j = 0; j < hiddenLayersNeuronsAmount; j++)
            {
                network[i].neurons[j].inputSum = 0;
                network[i].neurons[j].bias = 0;
                network[i].neurons[j].delta = 0;
                network[i].neurons[j].input = (float *)malloc(previousLayerSize * sizeof(float));
                network[i].neurons[j].weights = (float *)malloc(nextLayerSize * sizeof(float));
                for (k = 0; k < nextLayerSize; k++)
                {
                    network[i].neurons[j].weights[k] = (rand() % 10000) / 10000.0;
                }
            }
        }
    }

    //Alocando saída da rede
    network[hiddenLayers + 1].neurons = (Neuron *)malloc(outputLayer * sizeof(Neuron));
    for (i = 0; i < outputLayer; i++)
    {
        network[hiddenLayers + 1].neurons[i].inputSum = 0;
        network[hiddenLayers + 1].neurons[i].bias = 0;
        network[hiddenLayers + 1].neurons[i].delta = 0;
        network[hiddenLayers + 1].neurons[i].input = (float *)malloc(hiddenLayersNeuronsAmount * sizeof(float));
    }

    //------------------------------------ MENU DE OPÇÕES ------------------------------------- //

    while (menuOption != 0)
    {
        if (menuOption == 1)
        {
            //------------------------------------ TREINAMENTO DA REDE ------------------------------------- //

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
                trainingInput[i] = (float *)malloc((inputLayer + 1) * sizeof(float));
                trainingOutput[i] = (float *)malloc((outputLayer + 1) * sizeof(float));
                for (j = 0; j < inputLayer; j++)
                {
                    printf("Digite a entrada %d do conjunto de treinamento %d: ", j, i);
                    scanf("%f", &trainingInput[i][j]);
                }
                for (j = 0; j < outputLayer; j++)
                {
                    printf("Digite a saida %d esperada do conjunto de treinamento %d: ", j, i);
                    scanf("%f", &trainingOutput[i][j]);
                }
            }

            for (ageCount = 0; ageCount < ages; ageCount++)
            {
                for (l = 0; l < dataTrainingAmount; l++)
                {
                    //Propagação de dados para execução
                    //Camada de entrada
                    for (j = 0; j < inputLayer; j++)
                    {
                        network[0].neurons[j].sigmoidResult = trainingInput[l][j];
                        int nextLayerSize = (hiddenLayers == 0) ? outputLayer : hiddenLayersNeuronsAmount;
                        for (k = 0; k < nextLayerSize; k++)
                        {
                            network[1].neurons[k].input[j] = network[0].neurons[j].sigmoidResult * network[0].neurons[j].weights[k];
                        }
                    }

                    //Camadas ocultas
                    if (hiddenLayers != 0)
                    {
                        for (i = 1; i < hiddenLayers + 1; i++)
                        {
                            int previousLayerSize = (i == 1) ? inputLayer : hiddenLayersNeuronsAmount;
                            int nextLayerSize = (i == hiddenLayers) ? outputLayer : hiddenLayersNeuronsAmount;
                            for (j = 0; j < hiddenLayersNeuronsAmount; j++)
                            {
                                network[i].neurons[j].inputSum = 0;
                                for (k = 0; k < previousLayerSize; k++)
                                {
                                    network[i].neurons[j].inputSum += network[i].neurons[j].input[k];
                                }
                                network[i].neurons[j].sigmoidResult = sigmoide(network[i].neurons[j].inputSum + network[i].neurons[j].bias);
                                for (k = 0; k < nextLayerSize; k++)
                                {
                                    network[i + 1].neurons[k].input[j] = (network[i].neurons[j].sigmoidResult * network[i].neurons[j].weights[k]);
                                }
                            }
                        }
                    }

                    //Camada de saída
                    int previousLayerSize = (hiddenLayers == 0) ? inputLayer : hiddenLayersNeuronsAmount;
                    for (j = 0; j < outputLayer; j++)
                    {
                        network[hiddenLayers + 1].neurons[j].inputSum = 0;
                        for (k = 0; k < previousLayerSize; k++)
                        {
                            network[hiddenLayers + 1].neurons[j].inputSum += network[hiddenLayers + 1].neurons[j].input[k];
                        }
                        network[hiddenLayers + 1].neurons[j].sigmoidResult = sigmoide(network[hiddenLayers + 1].neurons[j].inputSum + network[hiddenLayers + 1].neurons[j].bias);
                    }

                    //Calculo dos deltas da ultima camada
                    for (j = 0; j < outputLayer; j++)
                    {
                        //printf("Calculo do erro: %f \n", trainingOutput[l][j] -  network[hiddenLayers + 1].neurons[j].sigmoidResult);
                        network[hiddenLayers + 1].neurons[j].delta = (trainingOutput[l][j] - network[hiddenLayers + 1].neurons[j].sigmoidResult) * network[hiddenLayers + 1].neurons[j].sigmoidResult * (1 - network[hiddenLayers + 1].neurons[j].sigmoidResult);
                    }

                    //Calculo dos deltas das camadas intermediarias
                    for (j = hiddenLayers; j > 0; j--)
                    {
                        int nextLayerSize = (j == hiddenLayers) ? outputLayer : hiddenLayersNeuronsAmount;
                        for (k = 0; k < hiddenLayersNeuronsAmount; k++)
                        {
                            network[j].neurons[k].delta = 0;
                            for (a = 0; a < nextLayerSize; a++)
                            {
                                network[j].neurons[k].delta += network[j + 1].neurons[a].delta * network[j].neurons[k].weights[a];
                            }
                            network[j].neurons[k].delta = network[j].neurons[k].delta * network[j].neurons[k].sigmoidResult * (1 - network[j].neurons[k].sigmoidResult);
                        }
                    }

                    //Atualizaçao dos pesos e dos bias
                    for (j = 0; j < hiddenLayers + 1; j++)
                    {
                        //printf("j: %d", j);
                        int nextLayerSize = (j == hiddenLayers) ? outputLayer : hiddenLayersNeuronsAmount;
                        int layerSize = (j == 0) ? inputLayer : hiddenLayersNeuronsAmount;
                        for (k = 0; k < layerSize; k++)
                        {

                            // printf("k: %d", k);
                            for (a = 0; a < nextLayerSize; a++)
                            {
                                network[j].neurons[k].weights[a] = network[j].neurons[k].weights[a] + (network[j + 1].neurons[a].delta * network[j].neurons[k].sigmoidResult * learningRate);
                                //   printf("Novo peso: %f\n", network[j].neurons[k].weights[a]);
                            }
                            network[j].neurons[k].bias = network[j].neurons[k].bias + (network[j].neurons[k].delta * learningRate);
                        }
                    }
                    for (j = 0; j < outputLayer; j++)
                    {
                        network[hiddenLayers + 1].neurons[j].bias = network[hiddenLayers + 1].neurons[j].bias + (network[hiddenLayers + 1].neurons[j].delta * learningRate);
                    }
                }
            }
        }
        else if (menuOption == 2)
        {
            //------------------------------------ EXECUÇÃO DA REDE ------------------------------------- //

            //Receber dados de entrada
            float *input = (float *)malloc(inputLayer * sizeof(float));
            for (i = 0; i < inputLayer; i++)
            {
                printf("Digite o valor de entrada %d:", i);
                scanf("%f", &input[i]);
            }

            //Propagação de dados para execução

            //Camada de entrada
            for (j = 0; j < inputLayer; j++)
            {
                network[0].neurons[j].sigmoidResult = input[j];
                int nextLayerSize = (hiddenLayers == 0) ? outputLayer : hiddenLayersNeuronsAmount;
                for (k = 0; k < nextLayerSize; k++)
                {
                    network[1].neurons[k].input[j] = network[0].neurons[j].sigmoidResult * network[0].neurons[j].weights[k];
                }
            }

            //Camadas ocultas
            if (hiddenLayers != 0)
            {
                for (i = 1; i < hiddenLayers + 1; i++)
                {
                    int previousLayerSize = (i == 1) ? inputLayer : hiddenLayersNeuronsAmount;
                    int nextLayerSize = (i == hiddenLayers) ? outputLayer : hiddenLayersNeuronsAmount;
                    for (j = 0; j < hiddenLayersNeuronsAmount; j++)
                    {
                        network[i].neurons[j].inputSum = 0;
                        for (k = 0; k < previousLayerSize; k++)
                        {
                            network[i].neurons[j].inputSum += network[i].neurons[j].input[k];
                        }
                        network[i].neurons[j].sigmoidResult = sigmoide(network[i].neurons[j].inputSum + network[i].neurons[j].bias);
                        for (k = 0; k < nextLayerSize; k++)
                        {
                            network[i + 1].neurons[k].input[j] = (network[i].neurons[j].sigmoidResult * network[i].neurons[j].weights[k]);
                        }
                    }
                }
            }

            //Camada de saída
            int previousLayerSize = (hiddenLayers == 0) ? inputLayer : hiddenLayersNeuronsAmount;
            for (j = 0; j < outputLayer; j++)
            {
                network[hiddenLayers + 1].neurons[j].inputSum = 0;
                for (k = 0; k < previousLayerSize; k++)
                {
                    network[hiddenLayers + 1].neurons[j].inputSum += network[hiddenLayers + 1].neurons[j].input[k];
                }
                network[hiddenLayers + 1].neurons[j].sigmoidResult = sigmoide(network[hiddenLayers + 1].neurons[j].inputSum + network[hiddenLayers + 1].neurons[j].bias);
            }

            //Printar Saída de dados
            printf("SAÍDA DA REDE\n");
            for (j = 0; j < outputLayer; j++)
            {
                printf("NEURONIO %d: %f\n", j, network[hiddenLayers + 1].neurons[j].sigmoidResult);
            }
        }

        printf("Escolha:\n0 - Sair\n1 - Treinar a rede\n2 - Testar a rede\n");
        scanf("%d", &menuOption);
    }
    return 1;
}