/*
Este é o arquivo principal que coordena a execução do simulador do algoritmo de Tomasulo.
Fluxo de Execução:
Leitura das Instruções: As instruções são lidas do arquivo de texto e convertidas em objetos Instrucao.
Inicialização: As estações de reserva, status dos registradores e registradores são inicializados.
Execução do Ciclo de Tomasulo: O programa entra em um loop onde:
 -As instruções são emitidas, executadas e escritas de volta conforme as condições permitirem.
 -O clock do sistema é incrementado a cada iteração.
Impressão do Estado Final: Após a execução de todas as instruções, o estado final do sistema é impresso, mostrando o status dos registradores, estações de reserva e a tabela de temporização das instruções.
*/

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "EstacoesReservas.h"
#include "Instrucao.h"
#include "Printer.h"
#include "StatusRegistrador.h"

using namespace std;

// Número de Estações de Reserva de ADD
const int NumAddReservationStation = 3;
// Número de Estações de Reserva de Mult
const int NumMultReservationStation = 4;
// Número de Estações de Reserva de Div
const int NumDivReservationStation = 2;

// Número das Operações
enum Operation {
  AddOp = 0,
  SubOp = 1,
  MultOp = 2,
  DivOp = 3,
};

struct OpTime {
  int startTime;
  int endTime;
} OpTime;

// Latência da op de ADD
const int AddLatency = 4;
// Latência da op de MULT
const int MultLatency = 12;
// Latência da op de DIV
const int DivLatency = 38;
// Lantecia da ISSUE
const int IssueLatency = 1;
// Latência de WRITEBACK
const int WritebackLatency = 1;

// Clock total
int Clock = 0;

// Condição de teste completo
bool Completed = true;

// Total de writebacks efetuados
int TotalWritebacks = 0;
// Issue da instrução que esta sendo feita
int CurrentInstIssue = 0;

// Registrador zero
const int RegZero = 5000;
// Status de registrador vazio
const int RegStatusEmpty = 1000;
// Status de Operador Disponivel
const int OperandAvailable = 1001;
// Status de Operador Iniciado
const int OperandInit = 1002;

int ISSUE(vector<Instrucao> &Instructions, vector<EstacoesReservas> &ResStat,
          vector<StatusRegistrador> &RegStat, vector<int> &Register);
void EXECUTE(vector<Instrucao> &Inst, vector<EstacoesReservas> &ResStat,
             vector<StatusRegistrador> &RegStat, vector<int> &Register);
void WRITEBACK(vector<Instrucao> &Inst, vector<EstacoesReservas> &ResStat,
               vector<StatusRegistrador> &RegStat, vector<int> &Register);

// Seta os tempos de cada operação
void SetOpTimes(struct OpTime &AddTime, struct OpTime &SubTime,
                struct OpTime &MultTime, struct OpTime &DivTime);
// Faz a operação da Estação de Reserva
void execOperation(EstacoesReservas &rs, Operation op);
// Reseta os valores da Estação de Reserva quando for concluida sua operação
void resetEstacoesReservas(EstacoesReservas &rs);
// Tratar a linha de instrução do txt
Instrucao parseLine(string line);

int main(int argc, char *argv[]) {
  // Arquivo para ser lido
  string arq = "instrucoes.txt";
  ifstream file(arq);

  if (!file.is_open()) {
    cerr << "Erro ao abrir arquivo " << arq << endl;
    return 1;
  }

  // Linha da instrução a ser lida
  string instruction;

  // Vetor de instrucoes
  vector<Instrucao> inst = {};

  while (getline(file, instruction)) {
    Instrucao newInst = parseLine(instruction);
    inst.push_back(newInst);
  }

  // Adicionando as reservation stations
  // Para atualizar esse numero deve-se atualizar as variaveis globais tambem
  // NumAddReservationStation = 1
  // NumMultReservationStation = 2
  // NumDivReservationStation = 2
  EstacoesReservas ADD1(AddOp, OperandInit), ADD2(AddOp, OperandInit),
      ADD3(AddOp, OperandInit);
  EstacoesReservas MULT1(MultOp, OperandInit), MULT2(MultOp, OperandInit),
      MULT3(MultOp, OperandInit), MULT4(MultOp, OperandInit);
  EstacoesReservas DIV1(DivOp, OperandInit), DIV2(DivOp, OperandInit);

  // Vetor das reservation stations
  vector<EstacoesReservas> ResStation = {ADD1,  ADD2,  ADD3, MULT1, MULT2,
                                         MULT3, MULT4, DIV1, DIV2};

  // Status do registradores
    StatusRegistrador F0(RegStatusEmpty), F1(RegStatusEmpty), F2(RegStatusEmpty),
      F3(RegStatusEmpty), F4(RegStatusEmpty), F5(RegStatusEmpty),
      F6(RegStatusEmpty), F7(RegStatusEmpty), F8(RegStatusEmpty),
      F9(RegStatusEmpty), F10(RegStatusEmpty), F11(RegStatusEmpty),
      F12(RegStatusEmpty);

  // Vetor dos status dos registrador
  vector<StatusRegistrador> RegisterStatus = {F0, F1, F2, F3,  F4,  F5, F6,
                                           F7, F8, F9, F10, F11, F12};

  // Conteudo dos registradores
  vector<int> Register = {RegZero, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

  // Printar o estado inicial do sistema
  cout << "INITIAL VALUES:" << endl;

  Printer::printInstrucao(inst);
  Printer::printEstacoesReservas(ResStation);
  Printer::printRegistradores(Register);
  Printer::printStatusRegistrador(RegisterStatus);
  cout << endl;

  // Execucao das instrucoes
  do {
    Clock++;

    ISSUE(inst, ResStation, RegisterStatus, Register);
    EXECUTE(inst, ResStation, RegisterStatus, Register);
    WRITEBACK(inst, ResStation, RegisterStatus, Register);

    Printer::printRegistradores(Register);
    Printer::printTabelaTempo(inst, Clock);
    cout << endl;

    Completed = false;

    if (TotalWritebacks == inst.size()) {
      Completed = true;
    }
    cout << endl;
  } while (!Completed);


  return 0;
}

int ISSUE(vector<Instrucao> &Instructions,
          vector<EstacoesReservas> &RegistrationStations,
          vector<StatusRegistrador> &RegisterStatus, vector<int> &Register) { // A próxima instrução é retirada do conjunto de instruções e enviada para uma estação de reserva disponível. Os operandos são verificados e, se disponíveis, são lidos dos registradores ou da estação de reserva.

  int r = 0;
  bool rsFree = false;

  if (CurrentInstIssue >= Instructions.size()) {
    return 0;
  }

  r = Instructions[CurrentInstIssue].op;

  struct OpTime AddTime, SubTime, MultTime, DivTime;
  SetOpTimes(AddTime, SubTime, MultTime, DivTime);

  switch (r) {
  // Issue da operação ADD
  case AddOp:
    for (int i = AddTime.startTime; i < AddTime.endTime; i++) {
      if (!RegistrationStations[i].busy) {
        r = i;
        CurrentInstIssue++;
        RegistrationStations[i].op = AddOp;
        rsFree = true;
        break;
      }
    }

    if (!rsFree) {
      return 1;
    }
    break;

  // Issue da operação SUB
  case SubOp:
    for (int i = SubTime.startTime; i < SubTime.endTime; i++) {
      if (!RegistrationStations[i].busy) {
        r = i;
        CurrentInstIssue++;
        RegistrationStations[i].op = SubOp;
        rsFree = true;
        break;
      }
    }

    if (!rsFree) {
      return 1;
    }
    break;

  // Issue da operação MULT
  case MultOp:
    for (int i = MultTime.startTime; i < MultTime.endTime; i++) {
      if (!RegistrationStations[i].busy) {
        r = i;
        CurrentInstIssue++;
        RegistrationStations[i].op = MultOp;
        rsFree = true;
        break;
      }
    }

    if (!rsFree) {
      return 1;
    }
    break;

  // Issue da operação DIV
  case DivOp:
    for (int i = DivTime.startTime; i < DivTime.endTime; i++) {
      if (!RegistrationStations[i].busy) {
        r = i;
        CurrentInstIssue++;
        RegistrationStations[i].op = DivOp;
        rsFree = true;
        break;
      }
    }

    if (!rsFree) {
      return 1;
    }
    break;

  default:
    break;
  }

  if (RegisterStatus[Instructions[CurrentInstIssue - 1].rs].Qi ==
      RegStatusEmpty) {
    RegistrationStations[r].Vj =
        Register[Instructions[CurrentInstIssue - 1].rs];
    RegistrationStations[r].Qj = OperandAvailable;
  } else {
    RegistrationStations[r].Qj =
        RegisterStatus[Instructions[CurrentInstIssue - 1].rs].Qi;
  }

  if (RegisterStatus[Instructions[CurrentInstIssue - 1].rt].Qi ==
      RegStatusEmpty) {
    RegistrationStations[r].Vk =
        Register[Instructions[CurrentInstIssue - 1].rt];
    RegistrationStations[r].Qk = OperandAvailable;
  } else {
    RegistrationStations[r].Qk =
        RegisterStatus[Instructions[CurrentInstIssue - 1].rt].Qi;
  }

  RegistrationStations[r].busy = true;
  RegistrationStations[r].issueLatency = 0;

  RegistrationStations[r].instNum = CurrentInstIssue - 1;

  Instructions[CurrentInstIssue - 1].issueClock = Clock;

  RegisterStatus[Instructions[CurrentInstIssue - 1].rd].Qi = r;
  return 2;
}

void EXECUTE(vector<Instrucao> &INST, vector<EstacoesReservas> &RESSTATION,
             vector<StatusRegistrador> &REGSTATUS, vector<int> &REG) { // As instruções nas estações de reserva começam a ser executadas, dependendo da disponibilidade dos operandos. O tempo de latência é levado em conta para cada tipo de operação.

  for (int r = 0; r < RESSTATION.size(); r++) {
    if (RESSTATION[r].busy == true) {
      if (RESSTATION[r].issueLatency >= IssueLatency) {
        if (RESSTATION[r].Qj == OperandAvailable &&
            RESSTATION[r].Qk == OperandAvailable) {
          if (INST[RESSTATION[r].instNum].execClockBegin == 0)
            INST[RESSTATION[r].instNum].execClockBegin = Clock;

          RESSTATION[r].latency++;

          switch (RESSTATION[r].op) {
          // Execucao da operação ADD
          case AddOp:
            if (RESSTATION[r].latency == AddLatency) {
              execOperation(RESSTATION[r], AddOp);
              INST[RESSTATION[r].instNum].execClockEnd = Clock;
            }
            break;
          // Execucao da operação SUB
          case SubOp:
            if (RESSTATION[r].latency == AddLatency) {
              execOperation(RESSTATION[r], SubOp);
              INST[RESSTATION[r].instNum].execClockEnd = Clock;
            }
          // Execucao da operação MULT
          case MultOp:
            if (RESSTATION[r].latency == MultLatency) {
              execOperation(RESSTATION[r], MultOp);
              INST[RESSTATION[r].instNum].execClockEnd = Clock;
            }

          // Execucao da operação DIV
          case DivOp:
            if (RESSTATION[r].latency == DivLatency) {
              execOperation(RESSTATION[r], DivOp);
              INST[RESSTATION[r].instNum].execClockEnd = Clock;
            }
          default:
            break;
          }
        }
      } else
        RESSTATION[r].issueLatency++;
    }
  }
}

void WRITEBACK(vector<Instrucao> &INST, vector<EstacoesReservas> &RESSTATION,
               vector<StatusRegistrador> &REGSTATUS, vector<int> &REG) { //Após a execução, os resultados são escritos de volta nos registradores e nas estações de reserva, liberando-os para novas instruções.

  for (int r = 0; r < RESSTATION.size(); r++) {
    if (RESSTATION[r].completed) {
      if (RESSTATION[r].writebackLatency == WritebackLatency) {
        if (INST[RESSTATION[r].instNum].writebackClock == 0)
          INST[RESSTATION[r].instNum].writebackClock = Clock;

        for (int x = 0; x < REG.size(); x++) {
          if (REGSTATUS[x].Qi == r) {
            REG[x] = RESSTATION[r].result;
            REGSTATUS[x].Qi = RegStatusEmpty;
          }
        }

        for (int y = 0; y < RESSTATION.size(); y++) {
          if (RESSTATION[y].Qj == r) {
            RESSTATION[y].Vj = RESSTATION[r].result;
            RESSTATION[y].Qj = OperandAvailable;
          }
          if (RESSTATION[y].Qk == r) {
            RESSTATION[y].Vk = RESSTATION[r].result;
            RESSTATION[y].Qk = OperandAvailable;
          }
        }

        resetEstacoesReservas(RESSTATION[r]);
        TotalWritebacks++;
      } else
        RESSTATION[r].writebackLatency++;
    }
  }
}

void resetEstacoesReservas(EstacoesReservas &rs) {
  rs.completed = false;
  rs.busy = false;
  rs.Qj = OperandInit;
  rs.Qk = OperandInit;
  rs.Vj = 0;
  rs.Vk = 0;
  rs.writebackLatency = 0;
}

void execOperation(EstacoesReservas &rs, Operation op) {
  switch (op) {
  case AddOp:
    rs.result = rs.Vj + rs.Vk;
    break;
  case SubOp:
    rs.result = rs.Vj - rs.Vk;
    break;
  case MultOp:
    rs.result = rs.Vj * rs.Vk;
    break;
  case DivOp:
    rs.result = rs.Vj / rs.Vk;
    break;
  default:
    break;
  }
  rs.completed = true;
  rs.latency = 0;
  rs.issueLatency = 0;
}

void SetOpTimes(struct OpTime &AddTime, struct OpTime &SubTime,
                struct OpTime &MultTime, struct OpTime &DivTime) {
  AddTime.startTime = NumAddReservationStation - NumAddReservationStation;
  AddTime.endTime = NumAddReservationStation;

  SubTime.startTime = NumAddReservationStation - NumAddReservationStation;
  SubTime.endTime = NumAddReservationStation;

  MultTime.startTime = NumAddReservationStation;
  MultTime.endTime = NumAddReservationStation + NumMultReservationStation;

  DivTime.startTime = NumAddReservationStation + NumMultReservationStation;
  DivTime.endTime = NumAddReservationStation + NumMultReservationStation +
                    NumDivReservationStation;
}