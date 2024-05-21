#pragma once

class Instrucao {
public:
  int rd;
  int rs;
  int rt;
  int op;
  int issueClock;
  int execClockBegin;
  int execClockEnd;
  int writebackClock;

  Instrucao();
  Instrucao(int, int, int, int);
};