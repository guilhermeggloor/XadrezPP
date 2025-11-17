// main.cpp
#include <iostream>
#include <memory>
#include "Parser.h"
#include "Tabuleiro.h"
#include "Solver.h" // Inclui o Solver

// --- Helper para imprimir a solução ---
// (Precisamos do reverso do Parser::converterNotacao)
std::string converterParaNotacao(Posicao p, int maxLinhas) {
    std::string notacao = "";
    notacao += (char)('a' + p.col);
    notacao += (char)('0' + (maxLinhas - p.linha));
    return notacao;
}


int main() {
    std::string arquivo = "teste1.pcin"; // Ou qualquer outro .pcin

    std::cout << "Carregando problema de '" << arquivo << "'..." << std::endl;

    auto tabuleiro = Parser::carregarProblema(arquivo);
    if (!tabuleiro) {
        std::cerr << "Falha ao carregar o problema." << std::endl;
        return 1;
    }

    std::cout << "Problema carregado. Tabuleiro inicial:" << std::endl;
    tabuleiro->desenharNoConsole();

    // --- CHAMA O SOLVER ---
    std::cout << "\nCalculando solucao para mate em "
        << tabuleiro->getJogadasParaMate() << " jogadas..." << std::endl;

    Solver solver;
    ResultadoBusca solucao = solver.encontrarSolucao(*tabuleiro);

    // --- IMPRIME O RESULTADO ---
    if (solucao.achouMate) {
        std::cout << "\nSOLUCAO ENCONTRADA!" << std::endl;
        std::cout << "Sequencia de jogadas:" << std::endl;

        int maxL = tabuleiro->getLinhas();
        for (const auto& mov : solucao.sequencia) {
            std::cout << "  "
                << converterParaNotacao(mov.de, maxL)
                << " -> "
                << converterParaNotacao(mov.para, maxL)
                << std::endl;
        }

        // TODO: Escrever a solucao no arquivo .pcout
        // (Isso é uma extensão da Fase 2 que podemos fazer depois)

    }
    else {
        std::cout << "\nSolucao nao encontrada." << std::endl;
    }

    return 0;
}