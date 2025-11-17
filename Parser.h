#pragma once

#include <string>
#include <memory>
#include "Tabuleiro.h" // Inclui a definição do nosso modelo

class Parser {
public:
    /**
     * @brief Carrega um problema de um arquivo .pcin e retorna um Tabuleiro.
     * @param caminhoArquivo O caminho para o arquivo .pcin (ex: "teste1.pcin").
     * @return Um unique_ptr para o Tabuleiro. Retorna nullptr se falhar.
     */
    static std::unique_ptr<Tabuleiro> carregarProblema(const std::string& caminhoArquivo);

private:
    /**
     * @brief Converte notação algébrica (ex: "a4") em Posicao (ex: {0, 0}).
     * @param notacao A string da notação (ex: "a4", "b3").
     * @param maxLinhas O número total de linhas do tabuleiro (para inverter o eixo Y).
     * @return A struct Posicao 0-indexada.
     */
    static Posicao converterNotacao(const std::string& notacao, int maxLinhas);
};