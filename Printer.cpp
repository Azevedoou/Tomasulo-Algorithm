/*
A classe Printer fornece funções para a impressão do estado do sistema e a análise de instruções no simulador do algoritmo de Tomasulo. Estas funções são utilizadas no main para ajudar na visualização e depuração do estado do simulador durante a execução. 
*/

#include "Printer.h"
#include <iostream>

using namespace std;
// Numero das Operacoes
enum Operation {
  AddOp = 0,
  SubOp = 1,
  MultOp = 2,
  DivOp = 3,
};

void Printer::printStatusRegistrador(vector<StatusRegistrador> RegisterStatus) { // Itera sobre o vetor de status dos registradores e imprime o valor de Qi de cada registrador, que indica se o registrador está ocupado e por qual estação de reserva.
  cout << "\nStatus dos registradores: " << endl;
  for (int i = 0; i < RegisterStatus.size(); i++) {
    cout << RegisterStatus[i].Qi << ' ';
  }
  cout << endl;
}

void Printer::printEstacoesReservas( //Itera sobre o vetor de estações de reserva e imprime o valor de Qj, Qk, Vk, Vj, op e busy de cada estação de reserva.
    vector<EstacoesReservas> ReservationStations) {
  for (int i = 0; i < ReservationStations.size(); i++) {
    cout << "RS #: " << i << "  Busy: " << ReservationStations[i].busy
         << "  op: " << ReservationStations[i].op
         << "  Vj: " << ReservationStations[i].Vj
         << "  Vk: " << ReservationStations[i].Vk
         << "  Qj: " << ReservationStations[i].Qj
         << "  Qk: " << ReservationStations[i].Qk << endl;
  }
}

void Printer::printRegistradores(vector<int> Registers) { //Itera sobre o vetor de registradores e imprime o valor de cada registrador.
  cout << "\nRegister Content:" << endl;
  for (int i = 0; i < Registers.size(); i++) {
    cout << Registers[i] << ' ';
  }
  cout << endl;
}

void Printer::printInstrucao(vector<Instrucao> IV) { //Itera sobre o vetor de instruções e imprime o valor de op, rd, rs e rt de cada instrução.
  for (int i = 0; i < IV.size(); i++) {
    cout << "Instruction #: " << i << "  Operation: " << IV[i].op << "  "
         << IV[i].rd << " <- " << IV[i].rs << " op " << IV[i].rt << endl;
  }
}

void Printer::printTabelaTempo(vector<Instrucao> INST, int Clock) { // Imprime os ciclos de emissão, execução e escrita de cada instrução, juntamente com o clock do sistema.
  string separator = "      ";
  cout << "Inst" << separator;
  cout << "Issue" << separator;
  cout << "Execute" << separator;
  cout << "WB" << separator;
  cout << "SystemClock" << endl;
  cout << "                                          " << Clock;
  cout << endl;
  cout << separator;
  cout << endl;

  for (int i = 0; i < INST.size(); i++) {
    cout << i << "         ";
    if (INST[i].issueClock < 10) {
      cout << INST[i].issueClock << "          ";
      cout << INST[i].execClockBegin << "-";
      cout << INST[i].execClockEnd << "          ";
      cout << INST[i].writebackClock << "          ";
    } else {
      cout << INST[i].issueClock << "         ";
      cout << INST[i].execClockBegin << "-";
      cout << INST[i].execClockEnd << "        ";
      cout << INST[i].writebackClock << "        ";
    }
    cout << endl;
  }
}

Instrucao parseLine(string line) { //Analisa a linha de instrução, identifica a operação e os registradores envolvidos e retorna um objeto Instrucao com os valores correspondentes.
  std::istringstream iss(line);
  std::string op, f1, f2, f3;

  // ADD F1, F2, F3
  iss >> op >> f1 >> f2 >> f3;

  int operand1 = std::stoi(f1.substr(1)); // F1 -> 1
  int operand2 = std::stoi(f2.substr(1)); // F2 -> 2
  int operand3 = std::stoi(f3.substr(1)); // F3 -> 3

  Operation operation;
  if (op == "ADD") {
    operation = AddOp;
  } else if (op == "SUB") {
    operation = SubOp;
  } else if (op == "MULT") {
    operation = MultOp;
  } else { // Se não for ADD, SUB ou MULT, é uma DIV
    operation = DivOp;
  }

  return Instrucao(operand1, operand2, operand3, operation);
}