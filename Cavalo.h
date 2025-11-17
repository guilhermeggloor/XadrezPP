#pragma once

#include "Peca.h"

class Cavalo : public Peca {
public:
    Cavalo(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::CAVALO) {}

    virtual std::vector<Movimento> getMovimentosPossiveis(const Tabuleiro& tabuleiro) const override;
    virtual std::unique_ptr<Peca> clone() const override;
};