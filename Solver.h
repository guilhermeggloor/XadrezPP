#pragma once

#include "Tabuleiro.h"
#include <vector>
#include <string>

// Struct para o resultado da busca
struct ResultadoBusca {
    bool achouMate;
    std::vector<Movimento> sequencia; // A sequência de movimentos
};

class Solver {
public:
    ResultadoBusca encontrarSolucao(const Tabuleiro& estadoInicial);

private:
    ResultadoBusca buscaRecursiva(const Tabuleiro& board, int jogadasBrancasRestantes, bool eTurnoBrancas, std::vector<Movimento>& sequenciaAtual, std::vector<std::string>& historico);
};