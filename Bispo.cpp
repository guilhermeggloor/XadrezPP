#include "Bispo.h"
#include "Tabuleiro.h"

// Esta função é idêntica à da Dama.cpp e Torre.cpp
void Bispo::checarDirecao(std::vector<Movimento>& movimentos, const Tabuleiro& tabuleiro, int dr, int dc) const {
    Posicao proximaPos = { m_pos.linha + dr, m_pos.col + dc };

    while (tabuleiro.ePosValida(proximaPos)) {
        Peca* pecaNoLocal = tabuleiro.getPecaEm(proximaPos);

        if (pecaNoLocal == nullptr) {
            movimentos.push_back({ m_pos, proximaPos });
        }
        else if (pecaNoLocal->getCor() != m_cor) {
            movimentos.push_back({ m_pos, proximaPos });
            break; // Captura e para
        }
        else {
            break; // Peça aliada, para
        }
        proximaPos.linha += dr;
        proximaPos.col += dc;
    }
}

std::vector<Movimento> Bispo::getMovimentosPossiveis(const Tabuleiro& tabuleiro) const {
    std::vector<Movimento> movimentos;

    // Checa as 4 direções diagonais
    checarDirecao(movimentos, tabuleiro, -1, -1); // Diagonal Cima-Esq
    checarDirecao(movimentos, tabuleiro, -1, 1);  // Diaognal. Cima-Dir
    checarDirecao(movimentos, tabuleiro, 1, -1);  // Diagonal Baixo-Esq
    checarDirecao(movimentos, tabuleiro, 1, 1);   // Diagonal Baixo-Dir

    return movimentos;
}

std::unique_ptr<Peca> Bispo::clone() const {
    return std::make_unique<Bispo>(*this);
}