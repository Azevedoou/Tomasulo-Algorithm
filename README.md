# Tomasulo-Algorithm
Este projeto é um simulador do algoritmo de Tomasulo implementado em C++. O simulador lê instruções de um arquivo, processa-as seguindo os princípios do algoritmo de Tomasulo e exibe o estado dos registradores e das estações de reserva a cada ciclo.

## `main.cpp`
### Estrutura do Projeto
main.cpp: Contém a lógica principal do simulador.
Printer.h: Contém funções auxiliares para impressão e formatação.
EstacoesReservas.h: Define a estrutura das estações de reserva.
StatusRegistrador.h: Define o status dos registradores.
Instrucao.h: Define a estrutura das instruções.
### Formato do Arquivo de Entrada
O arquivo instrucoes.txt deve conter as instruções no formato:
<opcode> <destino> <operando1> <operando2>
### Detalhes de Implementação
ISSUE: Verifica se há estações de reserva disponíveis e emite a instrução.
EXECUTE: Executa a instrução na estação de reserva quando os operandos estão disponíveis.
WRITEBACK: Escreve o resultado da operação de volta nos registradores e atualiza as estações de reserva.
### Funções Principais
ISSUE: Emite as instruções para as estações de reserva disponíveis.
EXECUTE: Executa as operações quando os operandos estão prontos.
WRITEBACK: Escreve o resultado das operações nos registradores e libera as estações de reserva.
### Latência das Operações
ADD: 4 ciclos
MULT: 12 ciclos
DIV: 38 ciclos
ISSUE: 1 ciclo
WRITEBACK: 1 ciclo
### Tipos de Operações
AddOp: Adição
SubOp: Subtração
MultOp: Multiplicação
DivOp: Divisão

## `EstacoesReservas`
Este arquivo EstacoesReservas.cpp é parte do simulador do algoritmo de Tomasulo. Ele define a classe EstacoesReservas, que representa as estações de reserva utilizadas no processo de execução das instruções.
### Estrutura da Classe
A classe EstacoesReservas encapsula os atributos e comportamentos das estações de reserva. As estações de reserva são responsáveis por armazenar e gerenciar as instruções durante a execução do algoritmo de Tomasulo.
### Uso da Classe
A classe EstacoesReservas é utilizada no simulador do algoritmo de Tomasulo para gerenciar a execução de instruções. Através dos atributos, é possível acompanhar o estado de cada instrução desde a emissão (issue) até a escrita do resultado (writeback).
### Métodos
#### Construtores
EstacoesReservas(): Construtor padrão que inicializa os atributos com valores padrão.
EstacoesReservas(int OP, int RSoperandAvailable): Construtor que inicializa a operação e a disponibilidade dos operandos.

## `StatusRegistrador`
### Descrição da Classe
A classe StatusRegistrador representa o status de um registrador no algoritmo de Tomasulo. Ela armazena informações sobre se o registrador está ocupado e qual estação de reserva, se houver, está associada ao registrador.
### Uso da Classe
A classe StatusRegistrador é utilizada no simulador do algoritmo de Tomasulo para gerenciar o estado dos registradores. Ela permite acompanhar quais registradores estão associados a quais estações de reserva, facilitando a emissão e execução de instruções.

## `Instrucao`
### Descrição da Classe
A classe Instrucao representa uma instrução a ser executada no algoritmo de Tomasulo. Ela armazena informações sobre os registradores fonte e destino, o tipo de operação, e os ciclos de clock associados aos diferentes estágios de execução da instrução.
### Uso da Classe
A classe Instrucao é utilizada no simulador do algoritmo de Tomasulo para gerenciar e acompanhar o progresso das instruções através dos diferentes estágios de execução: emissão (issue), execução (execute) e escrita (writeback).

## `Printer`
### Descrição da Classe
A classe Printer fornece métodos estáticos que ajudam na visualização dos estados dos registradores, estações de reserva e instruções, além de auxiliar no parsing de linhas de instruções do arquivo de entrada.
### Uso da Classe
A classe Printer é usada no simulador do algoritmo de Tomasulo para fornecer métodos utilitários que facilitam a visualização e o debugging do estado interno do simulador.

## Exemplo de Arquivo de Instruções
O arquivo deve conter N linhas, sendo N o número de instruções, e em cada linha deverá ser escrita cada instrução conforme exemplo abaixo(o arquivo abaixo possui N = 7 instruções):

ADD F1, F2, F3 

ADD F4, F1, F5 

SUB F6, F7, F8 

MULT F9, F4, F10 

DIV F11, F12, F6 

MULT F8, F1, F5 

MULT F7, F2, F3 


## Como Executar
Após colocar no mesmo diretório todos os códigos, bibliotecas e arquivos de texto disponibilizados neste github, compilar o arquivo main.cpp após ter inserido no arquivo instrucoes.txt as instruções que deseja simular.
Quando executar o main, serão exibidas no terminal as Estações de Reserva junto aos valores dos registradores a cada ciclo e, no final será exibido, de forma geral, todos os registradores.
Seguindo esses passos, você será capaz de compilar, configurar e executar o simulador do algoritmo de Tomasulo com sucesso.
