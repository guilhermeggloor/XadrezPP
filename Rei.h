#pragma once

#include "Peca.h"

class Rei : public Peca {
public:
    Rei(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::REI) {}

    // Implementação do "contrato" da classe Peca
    virtual std::vector<Movimento> getMovimentosPossiveis(const Tabuleiro& tabuleiro) const override;
    virtual std::unique_ptr<Peca> clone() const override;
};