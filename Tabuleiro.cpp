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

// Implementação do Operador de Atribuição
Tabuleiro& Tabuleiro::operator=(const Tabuleiro& other) {
    if (this == &other) {
        return *this; // Proteção contra auto-atribuição (t = t)
    }

    // Copia os dados simples
    m_linhas = other.m_linhas;
    m_colunas = other.m_colunas;
    m_jogadasParaMate = other.m_jogadasParaMate;
    m_buracos = other.m_buracos;

    // Copia PROFUNDA das peças (Clone)
    m_pecas.clear(); // Limpa as peças antigas
    m_pecas.reserve(other.m_pecas.size()); // Otimização

    for (const auto& peca : other.m_pecas) {
        // Clona cada peça do outro tabuleiro para este
        m_pecas.push_back(peca->clone());
    }

    return *this;
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
                
                switch (p->getTipo()) {
                case TipoPeca::REI:     c = 'R'; break; // R de Rei
                case TipoPeca::DAMA:    c = 'D'; break; // D de Dama
                case TipoPeca::TORRE:   c = 'T'; break; // T de Torre
                case TipoPeca::BISPO:   c = 'B'; break; // B de Bispo
                case TipoPeca::CAVALO:  c = 'C'; break; // C de Cavalo
                case TipoPeca::PEAO:    c = 'P'; break; // P de Peão
                default: break;
                }

                if (p->getCor() == Cor::PRETA) {
                    c = tolower(c); // minúscula para Pretas (r, d, t...)
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
        // Se a peça atacante for um REI, não chamamos getMovimentosPossiveis().
        if (peca->getTipo() == TipoPeca::REI) {
            Posicao posRei = peca->getPosicao();
            // Verifica se o Rei está ao lado da posição 'p' (distância de 1 casa)
            int diffLinha = std::abs(posRei.linha - p.linha);
            int diffCol = std::abs(posRei.col - p.col);

            // Se a distância for 1 em qualquer direção, ele ataca 'p'
            if (diffLinha <= 1 && diffCol <= 1) {
                return true;
            }
        }
        else {
            // Para todas as outras peças (Dama, Torre, etc), usa a lógica padrão
            std::vector<Movimento> movimentos = peca->getMovimentosPossiveis(*this);
            for (const auto& mov : movimentos) {
                if (mov.para == p) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<Movimento> Tabuleiro::getMovimentosLegaisParaCor(Cor cor) const {
    std::vector<Movimento> movimentosLegais;
    std::vector<Peca*> pecasDaCor = getTodasPecas(cor);

    for (Peca* peca : pecasDaCor) {
        // Pega os movimentos da peça (ex: Rei.cpp)
        std::vector<Movimento> movimentosPossiveis = peca->getMovimentosPossiveis(*this);

        // Apenas copia os movimentos para a lista final
        for (const auto& mov : movimentosPossiveis) {
          
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
    // Cria uma cópia exata do tabuleiro atual
   
    Tabuleiro novoTab = *this;

    // Acha a peça a ser movida (na CÓPIA)
    Peca* pecaParaMover = novoTab.getPecaEm(mov.de);
    if (pecaParaMover == nullptr) {
        return novoTab; // Não devia acontecer, mas é uma defesa
    }

    // Acha se há uma peça para capturar (na cópia)
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

    // Atualiza a posição da peça que moveu (na cópia)
    pecaParaMover->setPosicao(mov.para);

    // Retorna o tabuleiro modificado
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

std::string Tabuleiro::getEstadoString() const {
    std::string s = "";
    // Percorre todas as casas
    for (int i = 0; i < m_linhas; ++i) {
        for (int j = 0; j < m_colunas; ++j) {
            Peca* p = getPecaEm({ i, j });
            if (p == nullptr) {
                s += "."; // Vazio
            }
            else {
                // Usa o tipo e a cor para criar o ID (ex: "Ra", "rb")
                char c = '?';
                switch (p->getTipo()) {
                case TipoPeca::REI: c = 'R'; break;
                case TipoPeca::DAMA: c = 'D'; break;
                case TipoPeca::TORRE: c = 'T'; break;
                case TipoPeca::BISPO: c = 'B'; break;
                case TipoPeca::CAVALO: c = 'C'; break;
                case TipoPeca::PEAO: c = 'P'; break;
                default: break;
                }
                if (p->getCor() == Cor::PRETA) c = tolower(c);
                s += c;
            }
        }
    }
    return s;
}