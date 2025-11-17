#pragma once

// Enums para definir tipos e cores
enum class Cor { BRANCA, PRETA, NENHUMA };
enum class TipoPeca { REI, DAMA, TORRE, BISPO, CAVALO, PEAO, NENHUMA };

// Uma struct simples para posições no tabuleiro (0-indexado)
// (0,0) é o canto superior esquerdo (ex: 'a4' no seu exemplo 4x3)
// (3,0) seria 'a1' no seu exemplo 4x3
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