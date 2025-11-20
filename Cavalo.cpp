#include "Cavalo.h"
#include "Tabuleiro.h"

std::vector<Movimento> Cavalo::getMovimentosPossiveis(const Tabuleiro& tabuleiro) const {
    std::vector<Movimento> movimentos;

    // Todos os 8 movimentos possíveis em "L" do cavalo
    const int dr[] = { -2, -2, -1, -1,  1,  1,  2,  2 };
    const int dc[] = { -1,  1, -2,  2, -2,  2, -1,  1 };

    for (int i = 0; i < 8; ++i) {
        Posicao novaPos = { m_pos.linha + dr[i], m_pos.col + dc[i] };

        // Verifica se a posição é válida (dentro do tabuleiro e não é buraco)
        if (tabuleiro.ePosValida(novaPos)) {
            Peca* pecaNoLocal = tabuleiro.getPecaEm(novaPos);

            // Pode mover se estiver vazio OU se for peça inimiga (captura)
            if (pecaNoLocal == nullptr || pecaNoLocal->getCor() != m_cor) {
                movimentos.push_back({ m_pos, novaPos });
            }
            // (Se for peça aliada, não faz nada)
        }
    }

    return movimentos;
}

std::unique_ptr<Peca> Cavalo::clone() const {
    return std::make_unique<Cavalo>(*this);
}