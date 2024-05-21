#pragma once

class EstacoesReservas {
public:
  bool busy;
  int Qj;
  int Qk;
  int Vj;
  int Vk;
  int latency;
  int op;
  int result;
  bool completed;
  int instNum;
  int issueLatency;
  int writebackLatency;

public:
  EstacoesReservas();
  EstacoesReservas(int, int);
};