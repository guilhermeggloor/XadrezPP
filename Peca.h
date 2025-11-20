// Peca.h
#pragma once

#include <vector>
#include <memory>
#include "Core.h"

// Pré-declaração para evitar inclusão circular.
// Peca precisa saber que Tabuleiro existe, mas não precisa de todos os detalhes.
class Tabuleiro;

class Peca {
protected:
    Posicao m_pos;
    Cor m_cor;
    TipoPeca m_tipo;

public:
    // Construtor
    Peca(Posicao pos, Cor cor, TipoPeca tipo)
        : m_pos(pos), m_cor(cor), m_tipo(tipo) {
    }

    // Destrutor virtual é essencial para classes base polimórficas
    virtual ~Peca() = default;

    // (Métodos Virtuais Puros) 

    virtual std::vector<Movimento> getMovimentosPossiveis(const Tabuleiro& tabuleiro) const = 0;

    virtual std::unique_ptr<Peca> clone() const = 0;

    // Getters e Setters Comuns 
    Posicao getPosicao() const { return m_pos; }
    Cor getCor() const { return m_cor; }
    TipoPeca getTipo() const { return m_tipo; }

    void setPosicao(Posicao novaPos) { m_pos = novaPos; }
};