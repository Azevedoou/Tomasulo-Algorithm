/*
 A classe é responsável por encapsular todas as informações necessárias para representar uma estação de reserva do algoritmo de Tomasulo. Os métodos são usados para inicializar objetos da classe com valores padrão ou especificados. Permite ao simulador acompanhar o estado de cada estação de reserva, incluindo se está ocupada, qual operação está sendo realizada, a latência acumulada, os valores dos operandos e o estado da emissão e escrita.
*/

#include "EstacoesReservas.h"
#include <iostream>

using namespace std;

EstacoesReservas::EstacoesReservas() {
    busy = false; // Indica se a estação de reserva está ocupada.
    op = 0; // Código da operação a ser realizada.
    latency = 0; // Latência acumulada para a operação.
    result = 0; // Resultado da operação.
    completed = false; // Indica se a operação foi concluída.
    Qj = 1; // Estado do primeiro operando (disponível ou o índice da estação que irá produzi-lo).
    Qk = 1; // Estado do segundo operando (disponível ou o índice da estação que irá produzi-lo).
    Vj = 0; // Valor do primeiro operando.
    Vk = 0; // Valor do segundo operando.
    instNum = 100000; // Número da instrução associada à estação de reserva.
    issueLatency = 0; // Latência para a emissão da instrução.
    writebackLatency = 0; // Latência para a escrita do resultado.
}

EstacoesReservas::EstacoesReservas(int OP, int RSoperandAvailable){ // Define todos atributos da estação de reserva com valores padrão. 
    busy = false;
    op = OP;
    latency = 0;
    result = 0;
    completed = false;
    Qj = RSoperandAvailable;
    Qk = RSoperandAvailable;
    Vj = 0;
    Vk = 0;
    instNum = 100000;
    issueLatency = 0;
    writebackLatency = 0;
}