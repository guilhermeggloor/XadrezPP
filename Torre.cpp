#include "Torre.h"
#include "Tabuleiro.h"

// Esta função é idêntica à da Dama.cpp
void Torre::checarDirecao(std::vector<Movimento>& movimentos, const Tabuleiro& tabuleiro, int dr, int dc) const {
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

std::vector<Movimento> Torre::getMovimentosPossiveis(const Tabuleiro& tabuleiro) const {
    std::vector<Movimento> movimentos;

    // Checa as 4 direções retas
    checarDirecao(movimentos, tabuleiro, -1, 0); // Cima
    checarDirecao(movimentos, tabuleiro, 1, 0);  // Baixo
    checarDirecao(movimentos, tabuleiro, 0, -1); // Esquerda
    checarDirecao(movimentos, tabuleiro, 0, 1);  // Direita

    return movimentos;
}

std::unique_ptr<Peca> Torre::clone() const {
    return std::make_unique<Torre>(*this);
}