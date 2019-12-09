#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float sigmoide(float x)
{
    float result = 1 / (1 + exp(-x));
    return result;
}

struct neuron
{
    int layer;
    float *input;
    float *weights;
    float bias;
    float delta;
    float *dW;
    float input_sum;
    float sigmoid_result;
};

struct layer
{
    struct neuron *neurons;
};

int main()
{
    int i, j, k, l, a, b, c, d, e;
    int input_layer;
    int output_layer;
    int amount_neurons_hidden_layers;
    int amount_layers;
    int option = 1;
    int ages;
    int quantity_data_train;
    srand((unsigned)time(NULL));

    //Declarações de entrada e saída
    printf("Insira a quantidade de neuronio na camada de entrada: ");
    scanf("%d", &input_layer);
    setbuf(stdin, NULL);
    printf("\n");

    printf("Insira a quantidade de camadas ocultas da rede: ");
    scanf("%d", &amount_layers);
    setbuf(stdin, NULL);
    printf("\n");

    if (amount_layers != 0)
    {
        printf("Insira a quantidade de neuronios em cada camada oculta da rede: ");
        scanf("%d", &amount_neurons_hidden_layers);
        setbuf(stdin, NULL);
        printf("\n");
    }

    printf("Insira a quantidade de neuronios na camada de saída: ");
    scanf("%d", &output_layer);
    setbuf(stdin, NULL);
    printf("\n");

    //Alocar a rede neural
    struct layer *network;

    network = (struct layer *)malloc((amount_layers + 2) * sizeof(struct layer));

    //Alocando entrada da rede
    network[0].neurons = (struct neuron *)malloc(input_layer * sizeof(struct neuron));
    for (i = 0; i < input_layer; i++)
    {
        if (amount_layers == 0)
        {
            network[0].neurons[i].input_sum = 0;
            network[0].neurons[i].bias = 0;
            network[0].neurons[i].weights = (float *)malloc(output_layer * sizeof(float));
            for (j = 0; j < output_layer; j++)
            {
                network[0].neurons[i].weights[j] = (rand() % 10000) / 10000.0;
            }
        }
        else
        {
            network[0].neurons[i].input_sum = 0;
            network[0].neurons[i].bias = 0;
            network[0].neurons[i].weights = (float *)malloc(amount_neurons_hidden_layers * sizeof(float));
            for (j = 0; j < amount_neurons_hidden_layers; j++)
            {
                network[0].neurons[i].weights[j] = (rand() % 10000) / 10000.0;
            }
        }
    }

    //Alocando camadas ocultas
    if (amount_layers != 0)
    {
        //Estruturando primeira camada oculta
        network[1].neurons = (struct neuron *)malloc(amount_neurons_hidden_layers * sizeof(struct neuron));
        for (j = 0; j < amount_neurons_hidden_layers; j++)
        {
            network[1].neurons[j].input_sum = 0;
            network[1].neurons[j].bias = 1;
            network[1].neurons[j].delta = 0;
            network[1].neurons[j].dW = (float *)malloc(input_layer * sizeof(float));
            network[1].neurons[j].input = (float *)malloc(input_layer * sizeof(float));
            network[1].neurons[j].weights = (float *)malloc(amount_neurons_hidden_layers * sizeof(float));
            for (k = 0; k < amount_neurons_hidden_layers; k++)
            {
                network[1].neurons[j].weights[k] = (rand() % 10000) / 10000.0;
            }
        }
        //Estruturando camadas ocultas intermediarias

        for (i = 1; i < amount_layers - 1; i++)
        {
            network[i + 1].neurons = (struct neuron *)malloc(amount_neurons_hidden_layers * sizeof(struct neuron));
            for (j = 0; j < amount_neurons_hidden_layers; j++)
            {
                network[i + 1].neurons[j].input_sum = 0;
                network[i + 1].neurons[j].bias = 1;
                network[i + 1].neurons[j].delta = 0;
                network[i + 1].neurons[j].dW = (float *)malloc(amount_neurons_hidden_layers * sizeof(float));
                network[i + 1].neurons[j].input = (float *)malloc(amount_neurons_hidden_layers * sizeof(float));
                network[i + 1].neurons[j].weights = (float *)malloc(amount_neurons_hidden_layers * sizeof(float));
                for (k = 0; k < amount_neurons_hidden_layers; k++)
                {
                    network[i + 1].neurons[j].weights[k] = (rand() % 10000) / 10000.0;
                }
            }
        }
        //Estruturando ultima camada oculta
        network[amount_layers].neurons = (struct neuron *)malloc(amount_neurons_hidden_layers * sizeof(struct neuron));
        for (j = 0; j < amount_neurons_hidden_layers; j++)
        {
            network[amount_layers].neurons[j].input_sum = 0;
            network[amount_layers].neurons[j].bias = 1;
            network[amount_layers].neurons[j].delta = 0;
            network[amount_layers].neurons[j].dW = (float *)malloc(amount_neurons_hidden_layers * sizeof(float));
            network[amount_layers].neurons[j].input = (float *)malloc(amount_neurons_hidden_layers * sizeof(float));
            network[amount_layers].neurons[j].weights = (float *)malloc(output_layer * sizeof(float));
            for (k = 0; k < output_layer; k++)
            {
                network[amount_layers].neurons[j].weights[k] = (rand() % 10000) / 10000.0;
            }
        }
    }

    //Alocando saída da rede
    network[amount_layers + 1].neurons = (struct neuron *)malloc(output_layer * sizeof(struct neuron));
    for (i = 0; i < output_layer; i++)
    {
        network[amount_layers + 1].neurons[i].input_sum = 0;
        network[amount_layers + 1].neurons[i].bias = 1;
        network[amount_layers + 1].neurons[i].delta = 0;
        network[amount_layers + 1].neurons[i].dW = (float *)malloc(amount_neurons_hidden_layers * sizeof(float));
        network[amount_layers + 1].neurons[i].input = (float *)malloc(amount_neurons_hidden_layers * sizeof(float));
    }

    while (option != 0)
    {
        if (option == 1)
        {
            //Treinamento
            printf("Digite a quantidade de epocas para treinar os dados: ");
            scanf("%d", &ages);
            printf("\n");
            printf("Digite a quantidade de dados que serão utilizados para o treinamento: ");
            scanf("%d", &quantity_data_train);
            printf("\n");
            int **training_input = (int **)malloc(quantity_data_train * sizeof(int));
            //Leitura dos dados de entrada
            for (i = 0; i < quantity_data_train; i++)
            {
                training_input[i] = (int *)malloc((input_layer + 1) * sizeof(int));
                for (j = 0; j < input_layer; j++)
                {
                    printf("Digite a entrada %d do conjunto de treinamento %d: ", j, i);
                    scanf("%d", &training_input[i][j]);
                }
                printf("Digite a saida esperada do conjunto de treinamento %d: ", i);
                scanf("%d", &training_input[i][input_layer]);
            }

            for (i = 0; i < ages; i++)
            {
                for (l = 0; l < quantity_data_train; l++)
                {
                    //Propagação de dados
                    //Camada de entrada
                    for (j = 0; j < input_layer; j++)
                    {
                        network[0].neurons[j].sigmoid_result = training_input[l][j];
                        if (amount_layers == 0)
                        {
                            for (k = 0; k < output_layer; k++)
                            {
                                network[1].neurons[k].input[j] = network[0].neurons[j].sigmoid_result * network[0].neurons[j].weights[k];
                            }
                        }
                        else
                        {
                            for (k = 0; k < amount_neurons_hidden_layers; k++)
                            {
                                network[1].neurons[k].input[j] = (network[0].neurons[j].sigmoid_result * network[0].neurons[j].weights[k]);
                            }
                        }
                    }

                    //camadas ocultas
                    if (amount_layers != 0)
                    {
                        if (amount_layers == 1)
                        {
                            //prmeira e ultima camada são a mesma
                            for (j = 0; j < amount_neurons_hidden_layers; j++)
                            {
                                for (k = 0; k < input_layer; k++)
                                {
                                    network[1].neurons[j].input_sum += network[1].neurons[j].input[k];
                                }
                                network[1].neurons[j].sigmoid_result = sigmoide(network[1].neurons[j].input_sum);
                                for (k = 0; k < output_layer; k++)
                                {
                                    network[2].neurons[k].input[j] = (network[1].neurons[j].sigmoid_result * network[1].neurons[j].weights[k]);
                                }
                            }
                        }
                        else
                        {
                            //primeira camada oculta
                            for (j = 0; j < amount_neurons_hidden_layers; j++)
                            {
                                for (k = 0; k < input_layer; k++)
                                {
                                    network[1].neurons[j].input_sum += network[1].neurons[j].input[k];
                                }
                                network[1].neurons[j].sigmoid_result = sigmoide(network[1].neurons[j].input_sum);
                                for (k = 0; k < amount_neurons_hidden_layers; k++)
                                {
                                    network[2].neurons[k].input[j] = (network[1].neurons[j].sigmoid_result * network[1].neurons[j].weights[k]);
                                }
                            }

                            //camadas ocultas intermediarias
                            for (b = 1; b < amount_layers - 1; b++)
                            {
                                for (j = 0; j < amount_neurons_hidden_layers; j++)
                                {
                                    for (k = 0; k < amount_neurons_hidden_layers; k++)
                                    {
                                        network[b + 1].neurons[j].input_sum += network[b + 1].neurons[j].input[k];
                                    }
                                    network[b + 1].neurons[j].sigmoid_result = sigmoide(network[b + 1].neurons[j].input_sum);
                                    for (k = 0; k < amount_neurons_hidden_layers; k++)
                                    {
                                        network[b + 2].neurons[k].input[j] = (network[b + 1].neurons[j].sigmoid_result * network[b + 1].neurons[j].weights[k]);
                                    }
                                }
                            }

                            //ultima camada oculta
                            for (j = 0; j < amount_neurons_hidden_layers; j++)
                            {
                                for (k = 0; k < amount_neurons_hidden_layers; k++)
                                {
                                    network[amount_layers].neurons[j].input_sum += network[amount_layers].neurons[j].input[k];
                                }
                                network[amount_layers].neurons[j].sigmoid_result = sigmoide(network[amount_layers].neurons[j].input_sum);
                                for (k = 0; k < output_layer; k++)
                                {
                                    network[amount_layers + 1].neurons[k].input[j] = (network[amount_layers].neurons[j].sigmoid_result * network[amount_layers].neurons[j].weights[k]);
                                }
                            }
                        }
                    }

                    //camada de saída
                    if (amount_layers == 0)
                    {
                        for (j = 0; j < output_layer; j++)
                        {
                            for (k = 0; k < input_layer; k++)
                            {
                                network[1].neurons[j].input_sum += network[1].neurons[j].input[k];
                            }
                            network[amount_layers + 1].neurons[j].sigmoid_result = sigmoide(network[amount_layers + 1].neurons[j].input_sum);
                        }
                    }
                    else
                    {
                        for (j = 0; j < output_layer; j++)
                        {
                            for (k = 0; k < amount_neurons_hidden_layers; k++)
                            {
                                network[amount_layers + 1].neurons[j].input_sum += network[amount_layers + 1].neurons[j].input[k];
                            }
                            network[amount_layers + 1].neurons[j].sigmoid_result = sigmoide(network[amount_layers + 1].neurons[j].input_sum);
                        }
                    }

                    //Calculo dos deltas da ultima camada
                    for (j = 0; j < output_layer; j++)
                    {
                        network[amount_layers + 1].neurons[j].delta = 0.2 * network[amount_layers + 1].neurons[j].sigmoid_result * (training_input[l][input_layer] - network[amount_layers].neurons[j].sigmoid_result) * (1 - network[amount_layers].neurons[j].sigmoid_result);
                    }

                    //Calculo dos deltas das camadas intermediarias
                    for (j = amount_layers; j > 0; j--)
                    {
                        for (k = 0; k < amount_neurons_hidden_layers; k++)
                        {
                            //ultima camada de saida
                            if (j == amount_layers)
                            {
                                for (a = 0; a < output_layer; a++)
                                {
                                    network[j].neurons[k].delta += 0.2 * network[j + 1].neurons[a].input[k] * (1 - network[j + 1].neurons[a].input[k]) * network[j + 1].neurons[a].delta * network[j].neurons[k].weights[a];
                                    // printf("%f\n", rede.camada[i].neuronio[j].delta);
                                    // printf("%f\n", rede.camada[i + 1].neuronio[k].entrada[j]);
                                }
                            }
                            else
                            {
                                for (a = 0; a < amount_neurons_hidden_layers; a++)
                                {
                                    network[j].neurons[k].delta += 0.2 * network[j + 1].neurons[a].input[k] * (1 - network[j + 1].neurons[a].input[k]) * network[j + 1].neurons[a].delta * network[j].neurons[k].weights[a];
                                    // printf("%f\n", rede.camada[i].neuronio[j].delta);
                                    // printf("%f\n", rede.camada[i + 1].neuronio[k].entrada[j]);
                                }
                            }
                        }
                    }

                    //Calculos dos DW
                    if (ages == 0)
                    {
                        for (k = 0; k < output_layer; k++)
                        {
                            if (amount_layers == 0)
                            {
                                for (a = 0; a < input_layer + 1; a++)
                                {
                                    if (a == input_layer)
                                    {
                                        network[amount_layers + 1].neurons[k].dW[a] = 0.2 * network[amount_layers + 1].neurons[k].bias + 0.2 * network[amount_layers + 1].neurons[k].delta * network[amount_layers + 1].neurons[k].sigmoid_result;
                                    }
                                    else
                                    {
                                        network[amount_layers + 1].neurons[k].dW[a] = 0.2 * network[amount_layers].neurons[a].weights[k] + 0.2 * network[amount_layers + 1].neurons[k].delta * network[amount_layers + 1].neurons[k].sigmoid_result;
                                    }
                                }
                            }
                            else
                            {
                                for (a = 0; a < amount_neurons_hidden_layers + 1; a++)
                                {
                                    if (a == amount_neurons_hidden_layers)
                                    {
                                        network[amount_layers + 1].neurons[k].dW[a] = 0.2 * network[amount_layers + 1].neurons[k].bias + 0.2 * network[amount_layers + 1].neurons[k].delta * network[amount_layers + 1].neurons[k].sigmoid_result;
                                    }
                                    else
                                    {
                                        network[amount_layers + 1].neurons[k].dW[a] = 0.2 * network[amount_layers].neurons[a].weights[k] + 0.2 * network[amount_layers + 1].neurons[k].delta * network[amount_layers + 1].neurons[k].sigmoid_result;
                                    }
                                }
                            }
                        }
                        for (k = amount_layers; k > 0; k--)
                        {
                            for (j = 0; j < amount_neurons_hidden_layers; j++)
                            {
                                // tratar camada de entrada
                                if (k == 1)
                                {
                                    for (a = 0; a < input_layer + 1; a++)
                                    {
                                        if (a == input_layer)
                                        {
                                            network[k].neurons[j].dW[a] = 0.2 * network[k].neurons[j].bias + 0.2 * network[k].neurons[j].delta * network[k].neurons[j].sigmoid_result;
                                        }
                                        else
                                        {
                                            network[k].neurons[j].dW[a] = 0.2 + 0.2 * network[k].neurons[j].delta * network[k].neurons[j].sigmoid_result;
                                        }
                                    }
                                }
                                else
                                {
                                    for (a = 0; a < amount_neurons_hidden_layers; a++)
                                    {
                                        if (a == amount_neurons_hidden_layers)
                                        {
                                            network[k].neurons[j].dW[a] = 0.2 * network[k].neurons[j].bias + 0.2 * network[k].neurons[j].delta * network[k].neurons[j].sigmoid_result;
                                        }
                                        else
                                        {
                                            network[k].neurons[j].dW[a] = 0.2 * network[k - 1].neurons[a].weights[j] + 0.2 * network[k].neurons[j].delta * network[k].neurons[j].sigmoid_result;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        for (k = 0; k < output_layer; k++)
                        {
                            if (amount_layers == 0)
                            {
                                for (a = 0; a < input_layer + 1; a++)
                                {
                                    if (a == input_layer)
                                    {
                                        network[amount_layers + 1].neurons[k].dW[a] = 0.2 * network[amount_layers + 1].neurons[k].bias + 0.2 * network[amount_layers + 1].neurons[k].delta * network[amount_layers + 1].neurons[k].sigmoid_result;
                                    }
                                    else
                                    {
                                        network[amount_layers + 1].neurons[k].dW[a] = 0.2 * network[amount_layers + 1].neurons[k].dW[a] + 0.2 * network[amount_layers + 1].neurons[k].delta * network[amount_layers + 1].neurons[k].sigmoid_result;
                                    }
                                }
                            }
                            else
                            {
                                for (a = 0; a < amount_neurons_hidden_layers + 1; a++)
                                {
                                    if (a == amount_neurons_hidden_layers)
                                    {
                                        network[amount_layers + 1].neurons[k].dW[a] = 0.2 * network[amount_layers + 1].neurons[k].bias + 0.2 * network[amount_layers + 1].neurons[k].delta * network[amount_layers + 1].neurons[k].sigmoid_result;
                                    }
                                    else
                                    {
                                        network[amount_layers + 1].neurons[k].dW[a] = 0.2 * network[amount_layers + 1].neurons[k].dW[a] + 0.2 * network[amount_layers + 1].neurons[k].delta * network[amount_layers + 1].neurons[k].sigmoid_result;
                                    }
                                }
                            }
                        }
                        for (k = amount_layers; k > 0; k--)
                        {
                            for (j = 0; j < amount_neurons_hidden_layers; j++)
                            {
                                if (k == 1)
                                {
                                    for (a = 0; a < input_layer + 1; a++)
                                    {
                                        if (a == input_layer)
                                        {
                                            network[k].neurons[j].dW[a] = 0.2 * network[k].neurons[j].bias + 0.2 * network[k].neurons[j].delta * network[k].neurons[j].sigmoid_result;
                                        }
                                        else
                                        {
                                            network[k].neurons[j].dW[a] = 0.2 * network[k].neurons[j].dW[a] + 0.2 * network[k].neurons[j].delta * network[k].neurons[j].sigmoid_result;
                                        }
                                    }
                                }
                                else
                                {
                                    for (a = 0; a < amount_neurons_hidden_layers + 1; a++)
                                    {
                                        if (a == amount_neurons_hidden_layers)
                                        {
                                            network[k].neurons[j].dW[a] = 0.2 * network[k].neurons[j].bias + 0.2 * network[k].neurons[j].delta * network[k].neurons[j].sigmoid_result;
                                        }
                                        else
                                        {
                                            network[k].neurons[j].dW[a] = 0.2 * network[k].neurons[j].dW[a] + 0.2 * network[k].neurons[j].delta * network[k].neurons[j].sigmoid_result;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    //Atualizaçao dos pesos
                    for (j = 1; j < amount_layers + 2; j++)
                    {
                        //ultima camada
                        if (j == amount_layers + 1)
                        {
                            for (k = 0; k < output_layer; k++)
                            {
                                if (amount_layers == 0)
                                {
                                    for (a = 0; a < input_layer + 1; a++)
                                    {
                                        if (a == input_layer)
                                        {
                                            network[j].neurons[k].bias += network[j].neurons[k].dW[a];
                                        }
                                        else
                                        {
                                            network[j - 1].neurons[a].weights[k] += network[j].neurons[k].dW[a];
                                        }
                                    }
                                }
                                else
                                {
                                    for (a = 0; a < amount_neurons_hidden_layers + 1; a++)
                                    {
                                        if (a == amount_neurons_hidden_layers)
                                        {
                                            network[j].neurons[k].bias += network[j].neurons[k].dW[a];
                                        }
                                        else
                                        {
                                            network[j - 1].neurons[a].weights[k] += network[j].neurons[k].dW[a];
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (k = 0; k < amount_neurons_hidden_layers; k++)
                            {
                                if (j == 1)
                                {
                                    for (a = 0; a < input_layer + 1; a++)
                                    {
                                        if (a == input_layer)
                                        {
                                            network[j].neurons[k].bias += network[j].neurons[k].dW[a];
                                        }
                                        else
                                        {
                                            network[j - 1].neurons[a].weights[k] += network[j].neurons[k].dW[a];
                                        }
                                    }
                                }
                                else
                                {
                                    for (a = 0; a < amount_neurons_hidden_layers + 1; a++)
                                    {
                                        if (a == amount_neurons_hidden_layers)
                                        {
                                            network[j].neurons[k].bias += network[j].neurons[k].dW[a];
                                        }
                                        else
                                        {
                                            network[j - 1].neurons[a].weights[k] += network[j].neurons[k].dW[a];
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (option == 2)
        {
            int *input = (int *)malloc(input_layer * sizeof(int));
            //Execução da rede
            for (i = 0; i < input_layer; i++)
            {
                printf("Digite o valor de entrada %d:", i);
                scanf("%d", &input[i]);
            }

            //Propagação de dados
            //Camada de entrada
            for (j = 0; j < input_layer; j++)
            {
                network[0].neurons[j].sigmoid_result = input[j];
                if (amount_layers == 0)
                {
                    for (k = 0; k < output_layer; k++)
                    {
                        network[1].neurons[k].input[j] = network[0].neurons[j].sigmoid_result * network[0].neurons[j].weights[k];
                    }
                }
                else
                {
                    for (k = 0; k < amount_neurons_hidden_layers; k++)
                    {
                        network[1].neurons[k].input[j] = (network[0].neurons[j].sigmoid_result * network[0].neurons[j].weights[k]);
                    }
                }
            }

            //camadas ocultas
            if (amount_layers != 0)
            {
                if (amount_layers == 1)
                {
                    //prmeira e ultima camada são a mesma
                    for (j = 0; j < amount_neurons_hidden_layers; j++)
                    {
                        for (k = 0; k < input_layer; k++)
                        {
                            network[1].neurons[j].input_sum += network[1].neurons[j].input[k];
                        }
                        network[1].neurons[j].sigmoid_result = sigmoide(network[1].neurons[j].input_sum);
                        for (k = 0; k < output_layer; k++)
                        {
                            network[2].neurons[k].input[j] = (network[1].neurons[j].sigmoid_result * network[1].neurons[j].weights[k]);
                        }
                    }
                }
                else
                {
                    //primeira camada oculta
                    for (j = 0; j < amount_neurons_hidden_layers; j++)
                    {
                        for (k = 0; k < input_layer; k++)
                        {
                            network[1].neurons[j].input_sum += network[1].neurons[j].input[k];
                        }
                        network[1].neurons[j].sigmoid_result = sigmoide(network[1].neurons[j].input_sum);
                        for (k = 0; k < amount_neurons_hidden_layers; k++)
                        {
                            network[2].neurons[k].input[j] = (network[1].neurons[j].sigmoid_result * network[1].neurons[j].weights[k]);
                        }
                    }

                    //camadas ocultas intermediarias
                    for (i = 1; i < amount_layers - 1; i++)
                    {
                        for (j = 0; j < amount_neurons_hidden_layers; j++)
                        {
                            for (k = 0; k < amount_neurons_hidden_layers; k++)
                            {
                                network[i + 1].neurons[j].input_sum += network[i + 1].neurons[j].input[k];
                            }
                            network[i + 1].neurons[j].sigmoid_result = sigmoide(network[i + 1].neurons[j].input_sum);
                            for (k = 0; k < amount_neurons_hidden_layers; k++)
                            {
                                network[i + 2].neurons[k].input[j] = (network[i + 1].neurons[j].sigmoid_result * network[i + 1].neurons[j].weights[k]);
                            }
                        }
                    }

                    //ultima camada oculta
                    for (j = 0; j < amount_neurons_hidden_layers; j++)
                    {
                        for (k = 0; k < amount_neurons_hidden_layers; k++)
                        {
                            network[amount_layers].neurons[j].input_sum += network[amount_layers].neurons[j].input[k];
                        }
                        network[amount_layers].neurons[j].sigmoid_result = sigmoide(network[amount_layers].neurons[j].input_sum);
                        for (k = 0; k < output_layer; k++)
                        {
                            network[amount_layers + 1].neurons[k].input[j] = (network[amount_layers].neurons[j].sigmoid_result * network[amount_layers].neurons[j].weights[k]);
                        }
                    }
                }
            }

            //camada de saída
            if (amount_layers == 0)
            {
                for (j = 0; j < output_layer; j++)
                {
                    for (k = 0; k < input_layer; k++)
                    {
                        network[1].neurons[j].input_sum += network[1].neurons[j].input[k];
                    }
                    network[amount_layers + 1].neurons[j].sigmoid_result = sigmoide(network[amount_layers + 1].neurons[j].input_sum);
                }
            }
            else
            {
                for (j = 0; j < output_layer; j++)
                {
                    for (k = 0; k < amount_neurons_hidden_layers; k++)
                    {
                        network[amount_layers + 1].neurons[j].input_sum += network[amount_layers + 1].neurons[j].input[k];
                    }
                    network[amount_layers + 1].neurons[j].sigmoid_result = sigmoide(network[amount_layers + 1].neurons[j].input_sum);
                }
            }

            //Printar Saída de dados
            printf("SAÍDA DA REDE\n");
            for (j = 0; j < output_layer; j++)
            {
                printf("NEURONIO %d: %f\n", j, network[amount_layers + 1].neurons[j].sigmoid_result);
            }
        }
        //Entrada de dados

        // //Printar pesos da rede
        // printf("PRIMEIRA CAMADA");
        // for (i = 0; i < input_layer; i++)
        // {
        //     printf("\nNEURONIO %d:", i);
        //     if (amount_layers == 0)
        //     {
        //         for (j = 0; j < output_layer; j++)
        //         {
        //             printf("%f,", network[0].neurons[i].weights[j]);
        //         }
        //     }
        //     else
        //     {
        //         for (j = 0; j < amount_neurons_hidden_layers; j++)
        //         {
        //             printf("%f,", network[0].neurons[i].weights[j]);
        //         }
        //     }
        // }
        // for (i = 0; i < amount_layers - 1; i++)
        // {
        //     printf("\nCAMADA OCULTA %d", i);
        //     for (j = 0; j < amount_neurons_hidden_layers; j++)
        //     {
        //         printf("\nNEURONIO %d:", j);
        //         for (k = 0; k < amount_neurons_hidden_layers; k++)
        //             printf("%f,", network[i + 1].neurons[j].weights[k]);
        //     }
        // }
        // printf("\nULTIMA CAMADA OCULTA");
        // for (i = 0; i < amount_neurons_hidden_layers; i++)
        // {
        //     printf("\nNEURONIO %d:", i);
        //     for (k = 0; k < output_layer; k++)
        //         printf("%f,", network[amount_layers].neurons[i].weights[k]);
        // }
        printf("Escolha:\n0 - Sair\n1 - Treinar a rede\n 2 - Testar a rede\n");
        scanf("%d", &option);
    }
    return 0;
}
