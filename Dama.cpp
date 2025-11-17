// Dama.cpp
#include "Dama.h"
#include "Tabuleiro.h"

// Implementação do helper privado
void Dama::checarDirecao(std::vector<Movimento>& movimentos, const Tabuleiro& tabuleiro, int dr, int dc) const {
    Posicao proximaPos = { m_pos.linha + dr, m_pos.col + dc };

    // Continua na direção até sair do tabuleiro ou encontrar um bloqueio
    while (tabuleiro.ePosValida(proximaPos)) {
        Peca* pecaNoLocal = tabuleiro.getPecaEm(proximaPos);

        if (pecaNoLocal == nullptr) {
            // Vazio, pode mover e continuar checando
            movimentos.push_back({ m_pos, proximaPos });
        }
        else if (pecaNoLocal->getCor() != m_cor) {
            // Inimigo, pode capturar e PARAR
            movimentos.push_back({ m_pos, proximaPos });
            break;
        }
        else {
            // Aliado, PARAR
            break;
        }
        proximaPos.linha += dr;
        proximaPos.col += dc;
    }
}

std::vector<Movimento> Dama::getMovimentosPossiveis(const Tabuleiro& tabuleiro) const {
    std::vector<Movimento> movimentos;

    // Checa as 8 direções (Torre + Bispo)
    checarDirecao(movimentos, tabuleiro, -1, 0); // Cima
    checarDirecao(movimentos, tabuleiro, 1, 0);  // Baixo
    checarDirecao(movimentos, tabuleiro, 0, -1); // Esquerda
    checarDirecao(movimentos, tabuleiro, 0, 1);  // Direita
    checarDirecao(movimentos, tabuleiro, -1, -1); // Diag. Cima-Esq
    checarDirecao(movimentos, tabuleiro, -1, 1);  // Diag. Cima-Dir
    checarDirecao(movimentos, tabuleiro, 1, -1);  // Diag. Baixo-Esq
    checarDirecao(movimentos, tabuleiro, 1, 1);   // Diag. Baixo-Dir

    return movimentos;
}

std::unique_ptr<Peca> Dama::clone() const {
    return std::make_unique<Dama>(*this);
}