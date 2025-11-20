#pragma once

#include <string>
#include <memory>
#include "Tabuleiro.h" // Inclui a definição do nosso modelo

class Parser {
public:

    static std::unique_ptr<Tabuleiro> carregarProblema(const std::string& caminhoArquivo);

private:

    static Posicao converterNotacao(const std::string& notacao, int maxLinhas);
};