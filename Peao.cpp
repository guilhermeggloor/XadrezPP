#include "Peao.h"
#include "Tabuleiro.h"

std::vector<Movimento> Peao::getMovimentosPossiveis(const Tabuleiro& tabuleiro) const {
    std::vector<Movimento> movimentos;

    // Direção do movimento (Brancas sobem, Pretas descem)
    int direcao = (m_cor == Cor::BRANCA) ? -1 : 1;

    // 1. Movimento para frente
    Posicao posFrente = { m_pos.linha + direcao, m_pos.col };
    if (tabuleiro.ePosValida(posFrente) && tabuleiro.getPecaEm(posFrente) == nullptr) {
        movimentos.push_back({ m_pos, posFrente });
    }

    // 2. Captura diagonal esquerda
    Posicao posCapEsq = { m_pos.linha + direcao, m_pos.col - 1 };
    if (tabuleiro.ePosValida(posCapEsq) &&
        tabuleiro.eInimigoEm(posCapEsq, m_cor)) {
        movimentos.push_back({ m_pos, posCapEsq });
    }

    // 3. Captura diagonal direita
    Posicao posCapDir = { m_pos.linha + direcao, m_pos.col + 1 };
    if (tabuleiro.ePosValida(posCapDir) &&
        tabuleiro.eInimigoEm(posCapDir, m_cor)) {
        movimentos.push_back({ m_pos, posCapDir });
    }

    return movimentos;
}

std::unique_ptr<Peca> Peao::clone() const {
    return std::make_unique<Peao>(*this);
}