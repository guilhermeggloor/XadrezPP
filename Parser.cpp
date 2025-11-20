// Parser.cpp
#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>

// Includes das peças
#include "Rei.h"
#include "Dama.h"
#include "Peao.h"
#include "Torre.h"
#include "Bispo.h"
#include "Cavalo.h"

// (Função converterNotacao mantida igual...)
Posicao Parser::converterNotacao(const std::string& notacao, int maxLinhas) {
    if (notacao.length() < 2) return { -1, -1 };
    int col = notacao[0] - 'a';
    int linha = maxLinhas - (notacao[1] - '0');
    return { linha, col };
}

std::unique_ptr<Tabuleiro> Parser::carregarProblema(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << caminhoArquivo << std::endl;
        return nullptr;
    }

    std::string linha;
    int linhas, colunas, jogadas;

    // 1. Leitura do Cabeçalho
    if (!std::getline(arquivo, linha)) return nullptr;
    std::stringstream ssLinha1(linha);
    ssLinha1 >> linhas >> colunas >> jogadas;
    auto tabuleiro = std::make_unique<Tabuleiro>(linhas, colunas, jogadas);

    // Detecção de Buracos (Mantendo a lógica inteligente anterior)
    std::streampos posAntesDeLer = arquivo.tellg();
    if (std::getline(arquivo, linha)) {
        // Limpa espaços
        linha.erase(0, linha.find_first_not_of(" \t\r\n"));
        if (!linha.empty()) linha.erase(linha.find_last_not_of(" \t\r\n") + 1);

        if (!linha.empty()) {
            std::stringstream ss(linha);
            std::string token;
            ss >> token;
            // Se token for grande (ex: "rc4" = 3 chars) ou não começar com a-h, é peça
            if (token.length() >= 3) {
                arquivo.seekg(posAntesDeLer); // É peça, volta o arquivo
            }
            else {
                // São buracos
                std::stringstream ssBuracos(linha);
                std::string notacaoBuraco;
                while (ssBuracos >> notacaoBuraco) {
                    tabuleiro->adicionarBuraco(converterNotacao(notacaoBuraco, linhas));
                }
            }
        }
    }

    //  3. Leitura das Peças

    bool primeiraPeca = true;

    while (std::getline(arquivo, linha)) {
        linha.erase(0, linha.find_first_not_of(" \t\r\n"));
        if (linha.empty()) continue;

        char charOriginal = linha[0];
        std::string posNotacao = linha.substr(1);
        Posicao pos = converterNotacao(posNotacao, linhas);

        Cor cor;
        char tipoChar;

        // === REGRA DO PDF: A PRIMEIRA PEÇA É O REI PRETO ===
        if (primeiraPeca) {
            cor = Cor::PRETA;
            tipoChar = 'R'; // Forçamos ser um Rei
            primeiraPeca = false;

            // Debug para confirmar a correção
            std::cout << "Primeira Peca detectada: " << linha
                << " -> Interpretada como REI PRETO." << std::endl;
        }
        else {
            // Para as outras, segue a regra de maiúscula/minúscula
            cor = std::isupper(charOriginal) ? Cor::BRANCA : Cor::PRETA;
            tipoChar = std::toupper(charOriginal);
        }
        // ===================================================

        switch (tipoChar) {
        case 'R': case 'K': // Rei (Aceita R ou K)
            tabuleiro->adicionarPeca(std::make_unique<Rei>(pos, cor));
            break;
        case 'D': case 'Q': // Dama
            tabuleiro->adicionarPeca(std::make_unique<Dama>(pos, cor));
            break;
        case 'T':           // Torre
            if (tipoChar == 'R') {
                tabuleiro->adicionarPeca(std::make_unique<Rei>(pos, cor));
            }
            else {
                tabuleiro->adicionarPeca(std::make_unique<Torre>(pos, cor));
            }
            break;
        case 'B':   // Bispo
            tabuleiro->adicionarPeca(std::make_unique<Bispo>(pos, cor));
            break;
        case 'C': case 'N': // Cavalo
            tabuleiro->adicionarPeca(std::make_unique<Cavalo>(pos, cor));
            break;
        case 'P':
            tabuleiro->adicionarPeca(std::make_unique<Peao>(pos, cor));
            break;
        default:
            std::cerr << "Aviso: Peca '" << charOriginal << "' desconhecida." << std::endl;
            break;
        }
    }

    arquivo.close();
    return tabuleiro;
}

