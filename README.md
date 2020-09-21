# Rede-Neural-TCC

Rede Neural desenvolvida em C, utiliza o metódo de treinamento supervisionado Backpropagation.

## Como executar

Com o compilador de C instalado, execute: 
  gcc ann.c -o ann.exe   -->   Compilação do arquivo
  
  ann.exe --> Execução no Windows
  
  ./ann.exe --> Execução no Linux
  
## Parâmetros

A Rede Neural pergunta todos seus parâmetros ao usuário. Quantidade de entradas, quantidade de camadas ocultas, quantidade de neurônios em cada camada oculta, quantidade de saídas,
dados para treinamento, taxa de aprendizagem, quantidade de epocas de treinamento


## To-Do List

  Ajuste de leitura de dados para estruturação e treinamento da rede através de um arquivo externo.
  Ajuste para execução de uma quantidade de dados e escrita de respostas em um arquivo de saída.
  Desenvolvimento de condição de parada por Erro mínimo no treinamento.
  
  Desenvolvimento de funcionalidade de importar um arquivo para estruturar a rede neural.
  Desenvolvimento de funcionalidade de exportar um arquivo da rede neural para posterior importação, sem necessidade de re-treinamento.
  
