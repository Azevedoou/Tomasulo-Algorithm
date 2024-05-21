/*
A classe possui dois atributos que indicam o estado e a associação do registrador. Os métodos são usados para inicializar objetos da classe com valores padrão ou especificados. A classe mantem o controle sobre quais registradores estão sendo usados e qual estação de reserva está responsável por produzir o valor de cada registrador, facilitando a execução correta das instruções do algoritmo 
*/

#include "StatusRegistrador.h"
#include <iostream>

using namespace std;

StatusRegistrador::StatusRegistrador() {
    busy = false; // Se está ocupado
    Qi = 0; // Indica a estação de reserva que vai produzir o valor do registrador.
}

StatusRegistrador::StatusRegistrador(int RegStatusEmpty) {
    Qi = RegStatusEmpty;
}