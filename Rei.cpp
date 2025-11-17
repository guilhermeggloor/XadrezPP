#include "Rei.h"
#include "Tabuleiro.h" 

std::vector<Movimento> Rei::getMovimentosPossiveis(const Tabuleiro& tabuleiro) const {
    std::vector<Movimento> movimentos;
    Cor corInimiga = (m_cor == Cor::BRANCA) ? Cor::PRETA : Cor::BRANCA;

    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;

            Posicao novaPos = { m_pos.linha + dr, m_pos.col + dc };

            if (!tabuleiro.ePosValida(novaPos)) {
                continue;
            }

            Peca* pecaNoLocal = tabuleiro.getPecaEm(novaPos);

            // 1. Verifica se é peça aliada
            if (pecaNoLocal != nullptr && pecaNoLocal->getCor() == m_cor) {
                continue; // Pula peça aliada
            }

            // 2. Se a casa está VAZIA ou tem INIMIGO, ela SÓ é válida
            //    se NÃO FOR ATACADA.

            //    (O bug anterior estava aqui: `eAtacadoPor` falhava
            //     ao checar a própria casa da peça).

            //    Vamos tentar a simulação (Fix 3.7) que é mais robusta.

            Tabuleiro tabSimulado = tabuleiro;
            if (pecaNoLocal != nullptr) {
                // Se for captura, remove a peça do tabuleiro simulado
                tabSimulado.removerPecaEm(novaPos); // Requer a função da Fase 3.7
            }

            // Verifica se a casa (agora vazia) é atacada
            if (!tabSimulado.eAtacadoPor(novaPos, corInimiga)) {
                movimentos.push_back({ m_pos, novaPos });
            }
        }
    }
    return movimentos;
}

std::unique_ptr<Peca> Rei::clone() const {
    return std::make_unique<Rei>(*this);
}