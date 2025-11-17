#include "Parser.h"
#include <fstream>      // Para ler arquivos
#include <sstream>      // Para processar linhas
#include <iostream>     // Para mensagens de erro

// Inclui todas as nossas peças para podermos criá-las
#include "Rei.h"
#include "Dama.h"
#include "Peao.h"
#include "Torre.h"
#include "Bispo.h"
#include "Cavalo.h"

// Implementação do helper
Posicao Parser::converterNotacao(const std::string& notacao, int maxLinhas) {
    // notacao[0] é a coluna (char 'a', 'b', ...)
    // notacao[1] é a linha (char '1', '2', ...)

    int col = notacao[0] - 'a'; // 'a' -> 0, 'b' -> 1, etc.

    // A notação de xadrez é de baixo para cima.
    // Nossas coordenadas são de cima para baixo.
    // Em um tabuleiro de 4 linhas:
    // '4' -> linha 0
    // '3' -> linha 1
    // '2' -> linha 2
    // '1' -> linha 3
    int linha = maxLinhas - (notacao[1] - '0');

    return { linha, col };
}

// Implementação do método principal
std::unique_ptr<Tabuleiro> Parser::carregarProblema(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << caminhoArquivo << std::endl;
        return nullptr;
    }

    std::string linha;
    int linhas, colunas, jogadas;

    // --- 1. Ler Linha 1: Dimensões e Jogadas [cite: 24] ---
    if (!std::getline(arquivo, linha)) return nullptr;
    std::stringstream ssLinha1(linha);
    ssLinha1 >> linhas >> colunas >> jogadas;

    // Cria o tabuleiro com as dimensões lidas
    auto tabuleiro = std::make_unique<Tabuleiro>(linhas, colunas, jogadas);

    // --- 2. Ler Linha 2: Buracos [cite: 25] ---
    if (!std::getline(arquivo, linha)) return nullptr;
    if (!linha.empty()) {
        std::stringstream ssBuracos(linha);
        std::string notacaoBuraco;
        while (ssBuracos >> notacaoBuraco) {
            tabuleiro->adicionarBuraco(converterNotacao(notacaoBuraco, linhas));
        }
    }

    // --- 3. Ler Linhas Seguintes: Peças [cite: 26] ---
    std::string notacaoPeca;
    while (std::getline(arquivo, linha) && !linha.empty()) {

        char tipoChar = linha[0]; // K, D, P, k, ...
        std::string posNotacao = linha.substr(1); // a4, b1, ...

        Posicao pos = converterNotacao(posNotacao, linhas);
        Cor cor = isupper(tipoChar) ? Cor::BRANCA : Cor::PRETA;
        tipoChar = toupper(tipoChar);

        // ATENÇÃO: Este switch é o ponto que o professor critica! 
        // Vamos usá-lo por enquanto e depois melhorar.
        switch (tipoChar) {
        case 'K': // K = King (Rei) - Padrão Oficial
            tabuleiro->adicionarPeca(std::make_unique<Rei>(pos, cor));
            break;

        case 'Q': // Q = Queen (Dama) - Padrão Oficial
        case 'D': // D = Dama (Português, visto em exemplos)
            tabuleiro->adicionarPeca(std::make_unique<Dama>(pos, cor));
            break;

        case 'R': // R = Rook (Torre) - Padrão Oficial
        case 'T': // T = Torre (Português)
            tabuleiro->adicionarPeca(std::make_unique<Torre>(pos, cor));
            break;

        case 'B': // B = Bishop (Bispo) - Padrão Oficial
            tabuleiro->adicionarPeca(std::make_unique<Bispo>(pos, cor));
            break;

        case 'N': // N = Knight (Cavalo) - Padrão Oficial
        case 'C': // C = Cavalo (Português, nossa suposição)
            tabuleiro->adicionarPeca(std::make_unique<Cavalo>(pos, cor));
            break;

        case 'P': // P = Pawn (Peão) - Padrão Oficial (no contexto)
            tabuleiro->adicionarPeca(std::make_unique<Peao>(pos, cor));
            break;

        default:
            std::cerr << "Aviso: Peca desconhecida '" << tipoChar << "'." << std::endl;
            break;
        }
    }

    arquivo.close();
    return tabuleiro;
}