/*
A classe Instrucao é responsável por encapsular todas as informações de uma instrução necessárias para representar uma instrução no simulador do algoritmo de Tomasulo. Os métodos usados para inicializar os objetos da classe com valores padrão ou especificados são definidos na classe.
Em outras palavras, a classe permite ao simulador acompanhar o progresso de cada onstrução através do pipeline, incluindo quando ela é emitida, quando começa e termina a execução, e quando é escrita de volta.
*/

#include "Instrucao.h"
#include <iostream>

using namespace std;

Instrucao::Instrucao() {
  rd = 0; // Registrador de destino.
  rs = 0; // Registrador de origem.
  rt = 0; // Segundo registrador de fonte.
  op = 0; // Operação a ser realizada.
  issueClock = 0; // Ciclo de clock em que a instrução foi emitida.
  execClockBegin = 0; // Ciclo de clock em que a instrução foi iniciada.
  execClockEnd = 0; // Ciclo de clock em que a instrução foi finalizada.
  writebackClock = 0; // Ciclo de clock em que a instrução foi escrita de volta.
}

Instrucao::Instrucao(int RD, int RS, int RT, int OP) { // Define todos os atributos da instrução como  0. Isso inclui os registradores rd, rs, rt, op e os cilcos de clock.
  rd = RD;
  rs = RS;
  rt = RT;
  op = OP;
  issueClock = 0;
  execClockBegin = 0;
  execClockEnd = 0;
  writebackClock = 0;
}