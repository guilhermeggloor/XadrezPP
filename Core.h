#pragma once

// Enums para definir tipos e cores
enum class Cor { BRANCA, PRETA, NENHUMA };
enum class TipoPeca { REI, DAMA, TORRE, BISPO, CAVALO, PEAO, NENHUMA };

// Uma struct simples para posições no tabuleiro (0-indexado)
struct Posicao {
    int linha;
    int col;

    // Operador de igualdade para facilitar comparações
    bool operator==(const Posicao& other) const {
        return linha == other.linha && col == other.col;
    }
};

// Uma struct para representar um movimento
struct Movimento {
    Posicao de;
    Posicao para;
};