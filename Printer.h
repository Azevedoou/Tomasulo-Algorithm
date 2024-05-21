#pragma once

#include <iostream>
#include <vector>
#include "Printer.h"
#include <sstream>
#include "Instrucao.h"
#include "StatusRegistrador.h"
#include "EstacoesReservas.h"

using namespace std;

class Printer {
public:

    // Printar os status dos registradores
    void static printStatusRegistrador(vector<StatusRegistrador> );
    // Printar as reservation stations
    void static printEstacoesReservas(vector<EstacoesReservas> );
    // Printar os registradores
    void static printRegistradores(vector<int> );
    // Printar as instrucoes
    void static printInstrucao(vector<Instrucao> );
    // Printar a tabela de clock
    void static printTabelaTempo(vector<Instrucao>, int);
};