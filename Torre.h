#pragma once

#include "Peca.h"

class Torre : public Peca {
public:
    Torre(Posicao pos, Cor cor) : Peca(pos, cor, TipoPeca::TORRE) {}

    virtual std::vector<Movimento> getMovimentosPossiveis(const Tabuleiro& tabuleiro) const override;
    virtual std::unique_ptr<Peca> clone() const override;

private:
    // Reutilizamos o mesmo helper da Dama
    void checarDirecao(std::vector<Movimento>& movimentos, const Tabuleiro& tabuleiro, int dr, int dc) const;
};