#pragma once

#include "Tabuleiro.h"
#include <vector>

// Struct para o resultado da busca
struct ResultadoBusca {
    bool achouMate;
    std::vector<Movimento> sequencia; // A sequência de movimentos
};

class Solver {
public:
    /**
     * @brief Inicia a busca pela solução de mate.
     * @param estadoInicial O tabuleiro lido do arquivo .pcin.
     * @return Um ResultadoBusca contendo a sequência ou 'achouMate = false'.
     */
    ResultadoBusca encontrarSolucao(const Tabuleiro& estadoInicial);

private:
    /**
     * @brief A função recursiva Minimax.
     * @param board O estado ATUAL do tabuleiro na recursão.
     * @param jogadasBrancasRestantes Quantas jogadas brancas faltam.
     * @param eTurnoBrancas True se for o turno das Brancas, false (Pretas).
     * @param sequenciaAtual A sequência de movimentos que nos trouxe até aqui.
     * @return Um ResultadoBusca (apenas 'achouMate' é relevante na recursão).
     */
    ResultadoBusca buscaRecursiva(const Tabuleiro& board, int jogadasBrancasRestantes, bool eTurnoBrancas, std::vector<Movimento>& sequenciaAtual);
};