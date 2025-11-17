#include "Tabuleiro.h"
#include <iostream> // Para o debug no console
#include <cctype>   // Para tolower()

// Construtor
Tabuleiro::Tabuleiro(int linhas, int colunas, int jogadas)
    : m_linhas(linhas), m_colunas(colunas), m_jogadasParaMate(jogadas)
{
    // Inicializa a matriz de buracos com 'false'
    m_buracos.resize(m_linhas, std::vector<bool>(m_colunas, false));
}

// Construtor de Cópia
Tabuleiro::Tabuleiro(const Tabuleiro& other)
    : m_linhas(other.m_linhas),
    m_colunas(other.m_colunas),
    m_jogadasParaMate(other.m_jogadasParaMate),
    m_buracos(other.m_buracos)
{
    // chama o método clone() de cada peça
    // para criar uma cópia profunda.
    m_pecas.clear();
    for (const auto& peca : other.m_pecas) {
        m_pecas.push_back(peca->clone());
    }
}

void Tabuleiro::adicionarPeca(std::unique_ptr<Peca> peca) {
    m_pecas.push_back(std::move(peca));
}

void Tabuleiro::adicionarBuraco(Posicao pos) {
    if (pos.linha >= 0 && pos.linha < m_linhas && pos.col >= 0 && pos.col < m_colunas) {
        m_buracos[pos.linha][pos.col] = true;
    }
}

bool Tabuleiro::ePosValida(Posicao p) const {
    // Checa limites do tabuleiro
    if (p.linha < 0 || p.linha >= m_linhas || p.col < 0 || p.col >= m_colunas) {
        return false;
    }
    // Checa buracos
    if (m_buracos[p.linha][p.col]) {
        return false;
    }
    return true;
}

Peca* Tabuleiro::getPecaEm(Posicao p) const {
    for (const auto& peca : m_pecas) {
        if (peca->getPosicao() == p) {
            return peca.get(); // Retorna o ponteiro bruto
        }
    }
    return nullptr; // Nenhum
}

bool Tabuleiro::eInimigoEm(Posicao p, Cor corAliada) const {
    Peca* peca = getPecaEm(p);
    if (peca != nullptr && peca->getCor() != corAliada) {
        return true;
    }
    return false;
}


// Função de Debug para vermos o que fizemos
void Tabuleiro::desenharNoConsole() const {
    std::cout << "--- Tabuleiro " << m_linhas << "x" << m_colunas << " ---" << std::endl;
    for (int i = 0; i < m_linhas; ++i) {
        for (int j = 0; j < m_colunas; ++j) {
            if (m_buracos[i][j]) {
                std::cout << " X ";
                continue;
            }

            Peca* p = getPecaEm({ i, j });
            if (p == nullptr) {
                std::cout << " . ";
            }
            else {
                char c = '?';

                // === ATUALIZADO AQUI ===
                // Alinhando com a notação algébrica oficial
                switch (p->getTipo()) {
                case TipoPeca::REI:     c = 'K'; break; // King
                case TipoPeca::DAMA:    c = 'Q'; break; // Queen (Padrão)
                case TipoPeca::TORRE:   c = 'R'; break; // Rook
                case TipoPeca::BISPO:   c = 'B'; break; // Bishop
                case TipoPeca::CAVALO:  c = 'N'; break; // kNight
                case TipoPeca::PEAO:    c = 'P'; break; // Pawn
                default:                c = '?'; break;
                }
                // =======================

                if (p->getCor() == Cor::PRETA) {
                    c = tolower(c); // minúscula para Pretas
                }
                std::cout << " " << c << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------" << std::endl;
}

std::vector<Peca*> Tabuleiro::getTodasPecas(Cor cor) const {
    std::vector<Peca*> pecasDaCor;
    for (const auto& peca : m_pecas) {
        if (peca->getCor() == cor) {
            pecasDaCor.push_back(peca.get());
        }
    }
    return pecasDaCor;
}

Peca* Tabuleiro::getRei(Cor cor) const {
    for (const auto& peca : m_pecas) {
        if (peca->getTipo() == TipoPeca::REI && peca->getCor() == cor) {
            return peca.get();
        }
    }
    return nullptr;
}

bool Tabuleiro::eAtacadoPor(Posicao p, Cor corAtacante) const {
    // Pega todas as peças do atacante
    std::vector<Peca*> pecasAtacantes = getTodasPecas(corAtacante);

    for (Peca* peca : pecasAtacantes) {
        // Pega os movimentos *possíveis* (brutos) de cada peça
        std::vector<Movimento> movimentos = peca->getMovimentosPossiveis(*this);
        for (const auto& mov : movimentos) {
            // Se algum movimento deles pode capturar na posição 'p', ela está atacada
            if (mov.para == p) {
                return true;
            }
        }
    }
    return false;
}

std::vector<Movimento> Tabuleiro::getMovimentosLegaisParaCor(Cor cor) const {
    std::vector<Movimento> movimentosLegais;
    std::vector<Peca*> pecasDaCor = getTodasPecas(cor);

    for (Peca* peca : pecasDaCor) {
        // 1. Pega os movimentos da peça (ex: Rei.cpp)
        std::vector<Movimento> movimentosPossiveis = peca->getMovimentosPossiveis(*this);

        // 2. Apenas copia os movimentos para a lista final
        for (const auto& mov : movimentosPossiveis) {
            // ... (o código que postei antes tinha uma lógica aqui)
            // A sua lógica deve ser:
            if (cor == Cor::BRANCA) {
                movimentosLegais.push_back(mov);
            }
            else {
				movimentosLegais.push_back(mov);
            }
            
        }
    }
    return movimentosLegais;
}

Tabuleiro Tabuleiro::fazerMovimento(Movimento mov) const {
    // 1. Cria uma cópia exata do tabuleiro atual
    //    (Graças ao nosso construtor de cópia e ao peca->clone())
    Tabuleiro novoTab = *this;

    // 2. Acha a peça a ser movida (na CÓPIA)
    Peca* pecaParaMover = novoTab.getPecaEm(mov.de);
    if (pecaParaMover == nullptr) {
        return novoTab; // Não devia acontecer, mas é uma defesa
    }

    // 3. Acha se há uma peça para capturar (na CÓPIA)
    Peca* pecaCapturada = novoTab.getPecaEm(mov.para);
    if (pecaCapturada != nullptr) {
        // Remove a peça capturada da lista m_pecas
        auto& pecas = novoTab.m_pecas;
        pecas.erase(std::remove_if(pecas.begin(), pecas.end(),
            [pecaCapturada](const std::unique_ptr<Peca>& p) {
                return p.get() == pecaCapturada;
            }),
            pecas.end());
    }

    // 4. Atualiza a posição da peça que moveu (na CÓPIA)
    pecaParaMover->setPosicao(mov.para);

    // 5. Retorna o tabuleiro modificado
    return novoTab;
}

bool Tabuleiro::estaEmCheque(Cor corRei) const {
    Peca* rei = getRei(corRei);
    if (rei == nullptr) {
        return false; // Não há rei, não há cheque
    }

    Cor corAtacante = (corRei == Cor::BRANCA) ? Cor::PRETA : Cor::BRANCA;
    return eAtacadoPor(rei->getPosicao(), corAtacante);
}

bool Tabuleiro::eChequeMate(Cor corRei) const {
    // O Rei ESTÁ em cheque?
    bool cheque = estaEmCheque(corRei);

    // O Rei TEM movimentos legais?
    std::vector<Movimento> movimentosRei = getMovimentosLegaisParaCor(corRei);

    std::cout << "  [Debug Mate] Rei " << (corRei == Cor::PRETA ? "PRETO" : "BRANCO")
        << ": Esta em Cheque? " << (cheque ? "SIM" : "NAO")
        << ". Movimentos Legais: " << movimentosRei.size()
        << std::endl;

    // É mate se (cheque == true) E (movimentos == 0)
    return cheque && movimentosRei.empty();
}
bool Tabuleiro::eAfogamento(Cor corRei) const {
    // É afogamento se:
    // O Rei NÃO está em cheque
    // O Rei NÃO TEM movimentos legais
    return !estaEmCheque(corRei) && getMovimentosLegaisParaCor(corRei).empty();
}

void Tabuleiro::removerPecaEm(Posicao p) {
    auto& pecas = m_pecas; // Pega a referência para o vetor
    pecas.erase(
        std::remove_if(pecas.begin(), pecas.end(),
            [p](const std::unique_ptr<Peca>& peca) {
                return peca->getPosicao() == p;
            }),
        pecas.end());
}