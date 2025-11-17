#include "Solver.h"
#include <iostream> // Para debug

// Função pública que inicia a busca
ResultadoBusca Solver::encontrarSolucao(const Tabuleiro& estadoInicial) {
    std::vector<Movimento> sequencia;
    int jogadasParaMate = estadoInicial.getJogadasParaMate();

    // Inicia a busca. É o turno das Brancas.
    return buscaRecursiva(estadoInicial, jogadasParaMate, true, sequencia);
}

// A função recursiva (o "cérebro")
ResultadoBusca Solver::buscaRecursiva(const Tabuleiro& board, int jogadasBrancasRestantes, bool eTurnoBrancas, std::vector<Movimento>& sequenciaAtual)
{
    // === TURNO DAS BRANCAS (Maximizador) ===
    if (eTurnoBrancas) {
        // Pega todos os movimentos legais das Brancas
        std::vector<Movimento> movimentos = board.getMovimentosLegaisParaCor(Cor::BRANCA);

        for (const auto& mov : movimentos) {
            // 1. "Joga" o movimento e cria um novo tabuleiro
            Tabuleiro proximoBoard = board.fazerMovimento(mov);

            // Adiciona o movimento à nossa sequência de teste
            sequenciaAtual.push_back(mov);

            // 2. Verifica se este movimento deu MATE
            if (proximoBoard.eChequeMate(Cor::PRETA)) {
                // SUCESSO! Achamos um mate.
                return { true, sequenciaAtual };
            }

            // 3. Verifica se a profundidade acabou
            // Se as jogadas acabaram (==1) e não foi mate, este caminho falhou
            if (jogadasBrancasRestantes > 1) {
                // 4. Se não foi mate, passa o turno para as Pretas
                ResultadoBusca resPretas = buscaRecursiva(proximoBoard, jogadasBrancasRestantes, false, sequenciaAtual);

                if (resPretas.achouMate) {
                    // Se a chamada recursiva (Pretas) retornou 'true',
                    // significa que as Pretas não conseguiram escapar.
                    return { true, resPretas.sequencia };
                }
            }

            // 5. Se chegamos aqui, este 'mov' não levou a um mate forçado.
            // Desfaz o movimento (backtracking) e tenta o próximo.
            sequenciaAtual.pop_back();
        }

        // Se testamos todos os movimentos Brancos e NENHUM forçou mate, falhamos.
        return { false, {} };
    }
    // === TURNO DAS PRETAS (Minimizador) ===
    else {
        // Pega todos os movimentos legais das Pretas (só o Rei)
        std::vector<Movimento> movimentos = board.getMovimentosLegaisParaCor(Cor::PRETA);

        // Verifica afogamento (se não está em cheque e não tem movimentos)
        if (movimentos.empty() && !board.estaEmCheque(Cor::PRETA)) {
            // É afogamento, não mate. As brancas falharam.
            return { false, {} };
        }

        // Se o Rei não tem movimentos E está em cheque, já seria pego no
        // `eChequeMate` no turno anterior das Brancas.

        for (const auto& mov : movimentos) {
            // 1. "Joga" o movimento de fuga do Rei
            Tabuleiro proximoBoard = board.fazerMovimento(mov);
            sequenciaAtual.push_back(mov);

            // 2. Passa o turno de volta para as Brancas (com 1 jogada a menos)
            ResultadoBusca resBrancas = buscaRecursiva(proximoBoard, jogadasBrancasRestantes - 1, true, sequenciaAtual);

            // 3. Desfaz o movimento (backtracking)
            sequenciaAtual.pop_back();

            if (!resBrancas.achouMate) {
                // Se as Brancas falharam em *qualquer* um dos ramos,
                // significa que o Rei achou uma fuga.
                // O Minimizador (Pretas) venceu esta linha.
                return { false, {} };
            }
        }

        // Se o loop terminou, significa que TODOS os movimentos de fuga do Rei
        // foram checados e TODOS eles levaram a um mate.
        // O Rei está perdido. As Brancas (Maximizador) venceram.
        return { true, {} };
    }
}