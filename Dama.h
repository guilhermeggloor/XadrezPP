#pragma once

#include "Peca.h"

class Dama : public Peca {
public:
    Dama(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::DAMA) {}

    virtual std::vector<Movimento> getMovimentosPossiveis(const Tabuleiro& tabuleiro) const override;
    virtual std::unique_ptr<Peca> clone() const override;

private:
    // Helper para peças "deslizantes" (Dama, Torre, Bispo)
    void checarDirecao(std::vector<Movimento>& movimentos, const Tabuleiro& tabuleiro, int dr, int dc) const;
};