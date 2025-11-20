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

    Tabuleiro& operator=(const Tabuleiro& other);

    void adicionarPeca(std::unique_ptr<Peca> peca);
    void adicionarBuraco(Posicao pos);

    int getLinhas() const { return m_linhas; }
    int getColunas() const { return m_colunas; }
    int getJogadasParaMate() const { return m_jogadasParaMate; }

    bool ePosValida(Posicao p) const;
    Peca* getPecaEm(Posicao p) const;
    bool eInimigoEm(Posicao p, Cor corAliada) const;
    void desenharNoConsole() const;

    std::vector<Peca*> getTodasPecas(Cor cor) const;

    Peca* getRei(Cor cor) const;

    bool eAtacadoPor(Posicao p, Cor corAtacante) const;

    std::vector<Movimento> getMovimentosLegaisParaCor(Cor cor) const;

    Tabuleiro fazerMovimento(Movimento mov) const;

    bool estaEmCheque(Cor corRei) const;

    bool eChequeMate(Cor corRei) const;

    bool eAfogamento(Cor corRei) const;

    void removerPecaEm(Posicao p);

    // Gera uma string única representando o estado atual (peças e posições)
    std::string getEstadoString() const;
};