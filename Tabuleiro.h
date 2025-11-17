#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Core.h"
#include "Peca.h"

class Tabuleiro {
private:
    int m_linhas;
    int m_colunas;
    int m_jogadasParaMate;

    std::vector<std::unique_ptr<Peca>> m_pecas;
    std::vector<std::vector<bool>> m_buracos;

public:
    Tabuleiro(int linhas, int colunas, int jogadas);
    Tabuleiro(const Tabuleiro& other); // O construtor de cópia que já fizemos é VITAL

    void adicionarPeca(std::unique_ptr<Peca> peca);
    void adicionarBuraco(Posicao pos);

    int getLinhas() const { return m_linhas; }
    int getColunas() const { return m_colunas; }
    int getJogadasParaMate() const { return m_jogadasParaMate; }

    bool ePosValida(Posicao p) const;
    Peca* getPecaEm(Posicao p) const;
    bool eInimigoEm(Posicao p, Cor corAliada) const;
    void desenharNoConsole() const;
    
    // === NOVAS FUNÇÕES PARA O SOLVER ===

    /**
     * @brief Retorna uma lista de ponteiros para todas as peças de uma cor.
     */
    std::vector<Peca*> getTodasPecas(Cor cor) const;

    /**
     * @brief Retorna um ponteiro para o Rei de uma cor (ou nullptr).
     */
    Peca* getRei(Cor cor) const;

    /**
     * @brief Verifica se uma posição 'p' está sendo atacada pela 'corAtacante'.
     * Usado para verificar movimentos legais do Rei.
     */
    bool eAtacadoPor(Posicao p, Cor corAtacante) const;

    /**
     * @brief Gera todos os movimentos legais para uma cor, filtrando
     * movimentos que colocariam o próprio Rei em cheque.
     */
    std::vector<Movimento> getMovimentosLegaisParaCor(Cor cor) const;

    /**
     * @brief Aplica um movimento e retorna o NOVO estado do tabuleiro.
     * Esta é a função mais importante para o Solver.
     */
    Tabuleiro fazerMovimento(Movimento mov) const;

    /**
     * @brief Verifica se o Rei da 'cor' está em cheque.
     */
    bool estaEmCheque(Cor corRei) const;

    /**
     * @brief Verifica se o Rei da 'cor' está em cheque-mate.
     */
    bool eChequeMate(Cor corRei) const;

    /**
     * @brief Verifica se o Rei da 'cor' está afogado (stalemate).
     */
    bool eAfogamento(Cor corRei) const;

    void removerPecaEm(Posicao p);
};