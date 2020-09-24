# Rede-Neural-TCC

Rede Neural desenvolvida em C, utiliza o metódo de treinamento supervisionado Backpropagation.

## Como executar

Com o compilador de C instalado, execute: 
  
  ```
  gcc ann.c -o ann.exe   -->   Compilação do arquivo
  
  ann.exe --> Execução no Windows
  
  ./ann.exe --> Execução no Linux
  ```

## Parâmetros

A Rede Neural pergunta no inicio se o usuário deseja construir uma nova rede ou importar uma já criada.
  - Se optar por construir uma nova, a Rede Neural é construída a partir do arquivo *training.txt*. Existe um template explicando cada dado do arquivo 	*training.txt* em 	*training-template.txt*.
  
  - Se o usuário optar por importar uma rede já construída, essa rede deve estar salva em um arquivo de nome 	*saved-ann.txt*, esse arquivo deve seguir o padrão demonstrado no arquivo *saved-ann-template.txt*.
  
  Com a Rede criada, o usuário tem as opções de 
    - Testar a Rede.
    - Executar várias entradas.
    - Exportar a Rede Criada.

### Testar a Rede
  A função de testar a rede permite ao usuário entrar com um conjunto de dados de entrada, via terminal e receber o resultado da execução da rede, também via terminal.

### Executar Várias Entradas
  A função de executar várias entradas, permite ao usuário entrar com um conjunto de entradas por meio de um arquivo e receber todos os resultados das execuções por meio de um arquivo de saída.
  O arquivo de entrada com os dados para execução deve estar salvo com o nome *executions.txt* e deve seguir o padrão demonstrado no arquivo *executions-template.txt*.
  O arquivo de saída com os resultados das execuções é gerado pela Rede com o nome de *executions-output.txt* e segue o padrão demonstrado no arquivo *executions-output-template.txt*

### Exportar a Rede Criada
  A função de exportar a Rede Neural, cria um arquivo denominado *saved-ann.txt* com as informações da Rede Neural criada, esse arquivo segue o padrão demonstrado em *saved-ann-template.txt* e pode ser utilizado para posterior importação.

## To-Do List

 - Melhorar a função randômica de geração dos números aleatórios utilizados como inicialização de pesos da Rede.
  
