#pragma once

#include "Peca.h"

class Peao : public Peca {
public:
    Peao(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::PEAO) {}

    virtual std::vector<Movimento> getMovimentosPossiveis(const Tabuleiro& tabuleiro) const override;
    virtual std::unique_ptr<Peca> clone() const override;
};