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

    // === O CONTRATO (Métodos Virtuais Puros) ===

    /**
     * @brief Retorna todos os movimentos "legais" para esta peça,
     * considerando o estado atual do tabuleiro (bloqueios, capturas, etc.).
     * @param tabuleiro O tabuleiro para verificar as regras.
     * @return Um vetor de movimentos possíveis.
     */
    virtual std::vector<Movimento> getMovimentosPossiveis(const Tabuleiro& tabuleiro) const = 0;

    /**
     * @brief Cria e retorna uma cópia exata (um clone) desta peça.
     * Isso é vital para o Solver (Fase 3) poder copiar o tabuleiro.
     */
    virtual std::unique_ptr<Peca> clone() const = 0;

    // === Getters e Setters Comuns ===
    Posicao getPosicao() const { return m_pos; }
    Cor getCor() const { return m_cor; }
    TipoPeca getTipo() const { return m_tipo; }

    void setPosicao(Posicao novaPos) { m_pos = novaPos; }
};