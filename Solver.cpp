// Solver.cpp
#include "Solver.h"
#include <iostream> 

ResultadoBusca Solver::encontrarSolucao(const Tabuleiro& estadoInicial) {
    std::vector<Movimento> sequencia;
    std::vector<std::string> historico;
    int jogadasParaMate = estadoInicial.getJogadasParaMate();

    // Adiciona o estado inicial ao histórico
    historico.push_back(estadoInicial.getEstadoString());

    // Inicia a busca (Turno Brancas)
    return buscaRecursiva(estadoInicial, jogadasParaMate, true, sequencia, historico);
}

ResultadoBusca Solver::buscaRecursiva(const Tabuleiro& board, int jogadasBrancasRestantes, bool eTurnoBrancas, std::vector<Movimento>& sequenciaAtual, std::vector<std::string>& historico)
{
	// Maximizador: TURNO DAS BRANCAS
    if (eTurnoBrancas) {
        std::vector<Movimento> movimentos = board.getMovimentosLegaisParaCor(Cor::BRANCA);

        for (const auto& mov : movimentos) {
            Tabuleiro proximoBoard = board.fazerMovimento(mov);

            std::string idEstado = proximoBoard.getEstadoString();

            bool cicloDetectado = false;
            for (const auto& s : historico) {
                if (s == idEstado) {
                    cicloDetectado = true;
                    break;
                }
            }
            if (cicloDetectado) continue;

            // Adiciona ao histórico
            historico.push_back(idEstado);

            // 1. Verifica MATE Imediato
            if (proximoBoard.eChequeMate(Cor::PRETA)) {
                // Achamos! a sequencia é esse movimento
                return { true, { mov } };
            }

            // Recursão
            if (jogadasBrancasRestantes > 1) {
                // Chama recursão para as Pretas
                ResultadoBusca resPretas = buscaRecursiva(proximoBoard, jogadasBrancasRestantes, false, sequenciaAtual, historico);

                if (resPretas.achouMate) {
                    // SUCESSO! peças pretas não escapam
                    // Monta a sequência completa:  Movimento + O Resto da Sequência
                    std::vector<Movimento> sequenciaCompleta;
                    sequenciaCompleta.push_back(mov);
                    // Anexa o vetor retornado pelas pretas no final 
                    sequenciaCompleta.insert(sequenciaCompleta.end(), resPretas.sequencia.begin(), resPretas.sequencia.end());

                    return { true, sequenciaCompleta };
                }
            }
            // remove histórico para não atrapalhar
            historico.pop_back();
        }
        return { false, {} }; // Nenhum movimento funcionou
    }

    // minimizador: TURNO DAS PRETAS
    else {
        std::vector<Movimento> movimentos = board.getMovimentosLegaisParaCor(Cor::PRETA);

        // Afogamento?
        if (movimentos.empty() && !board.estaEmCheque(Cor::PRETA)) {
            return { false, {} }; // afogamento
        }

        ResultadoBusca melhorSequencia = { false, {} };

        for (const auto& mov : movimentos) {
            Tabuleiro proximoBoard = board.fazerMovimento(mov);

            // Detecção de ciclos 
            std::string idEstado = proximoBoard.getEstadoString();
            bool cicloDetectado = false;

            for (const auto& s : historico) {
                if (s == idEstado) { cicloDetectado = true; break; }
            }
            if (cicloDetectado) {
                // Se o Rei foge para uma posição repetida, conta como "fuga bem sucedida" (empate por repetição)
                // Logo, as Brancas FALHARAM em dar mate forçado aqui.
                return { false, {} };
            }

            historico.push_back(idEstado);

            // Chama recursão para as Brancas
            ResultadoBusca resBrancas = buscaRecursiva(proximoBoard, jogadasBrancasRestantes - 1, true, sequenciaAtual, historico);

            if (!resBrancas.achouMate) {
                // Se o Rei escapou aqui, então NÃO é mate forçado. Falhamos.
                return { false, {} };
            }

            // Se chegamos aqui, este movimento do Rei levou à morte dele.
            if (melhorSequencia.sequencia.empty()) {
                std::vector<Movimento> seq;
                seq.push_back(mov);
                seq.insert(seq.end(), resBrancas.sequencia.begin(), resBrancas.sequencia.end());
                melhorSequencia = { true, seq };
            }
        }

        // Se testamos todas as fugas e todas deram True (mate), retornamos a sequência salva
        return melhorSequencia;
    }
}