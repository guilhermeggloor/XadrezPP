#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem> // Para listar arquivos do diretório
#include <SFML/Graphics.hpp>

#include "Parser.h"
#include "Tabuleiro.h"
#include "Solver.h"
#include "Renderer.h" 

namespace fs = std::filesystem;

enum class EstadoJogo {
    MENU_SELECAO,
    CALCULANDO,
    VISUALIZANDO
};

// Lista arquivos .pcin no diretório atual
std::vector<std::string> listarArquivosPCIN() {
    std::vector<std::string> arquivos;
    try {
        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.path().extension() == ".pcin") {
                arquivos.push_back(entry.path().filename().string());
            }
        }
    }
    catch (...) {
        std::cerr << "Erro ao listar arquivos." << std::endl;
    }
    return arquivos;
}

// Converte posição para string (ex: a1)
std::string posParaString(Posicao p, int maxLinhas) {
    std::string s = "";
    s += (char)('a' + p.col);
    s += (char)('0' + (maxLinhas - p.linha));
    return s;
}

// Pega sigla da peça em PT-BR
char getCharPeca(TipoPeca tipo) {
    switch (tipo) {
    case TipoPeca::REI: return 'R';
    case TipoPeca::DAMA: return 'D';
    case TipoPeca::TORRE: return 'T';
    case TipoPeca::BISPO: return 'B';
    case TipoPeca::CAVALO: return 'C';
    case TipoPeca::PEAO: return 'P';
    default: return '?';
    }
}

int main() {
    // Configuração da Janela
    const int LARGURA_PADRAO = 800;
    const int ALTURA_PADRAO = 600;
    const float TAMANHO_CELULA = 80.0f;

    sf::RenderWindow window(sf::VideoMode({ (unsigned int)LARGURA_PADRAO, (unsigned int)ALTURA_PADRAO }), "Trabalho Xadrez - Menu");
    window.setFramerateLimit(60);

    // Carregar Fonte (Necessária para o Menu)
    sf::Font fonte;
    if (!fonte.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        // Tenta uma fonte local se não achar a do Windows
        if (!fonte.openFromFile("arial.ttf")) {
            std::cerr << "Erro critico: Fonte nao encontrada para o menu." << std::endl;
            return -1;
        }
    }

    // Estado Inicial
    EstadoJogo estadoAtual = EstadoJogo::MENU_SELECAO;

    // Variáveis de Controle
    std::string arquivoSelecionado = "";
    std::vector<std::string> listaArquivos;
    std::unique_ptr<Tabuleiro> tabuleiroInicial = nullptr;
    std::vector<Movimento> movimentosParaAnimar;

    // Variáveis de Animação
    Renderer renderer(TAMANHO_CELULA);
    Tabuleiro tabuleiroAtual(8, 8, 0); // Temporário
    sf::Clock relogio;
    size_t indiceMovimento = 0;
    float tempoPorJogada = 1.5f;
    bool animacaoFinalizada = false;
    std::string mensagemStatus = "";

    // Carrega lista inicial
    listaArquivos = listarArquivosPCIN();

    while (window.isOpen()) {
        // eventos
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Eventos do MENU
            if (estadoAtual == EstadoJogo::MENU_SELECAO) {
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                        // Verifica clique nos nomes dos arquivos
                        float y = 100.0f;
                        for (const auto& arq : listaArquivos) {
                            sf::Text texto(fonte);
                            texto.setString(arq);
                            texto.setPosition({ 50.0f, y });
                            texto.setCharacterSize(24);

                            if (texto.getGlobalBounds().contains(mousePos)) {
                                arquivoSelecionado = arq;
                                estadoAtual = EstadoJogo::CALCULANDO; // Muda estado
                            }
                            y += 40.0f;
                        }
                    }
                }
            }

            // Eventos da VISUALIZAÇÃO
            if (estadoAtual == EstadoJogo::VISUALIZANDO) {
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyEvent->code == sf::Keyboard::Key::R) {
                        // Reinicia animação
                        tabuleiroAtual = *tabuleiroInicial;
                        indiceMovimento = 0;
                        animacaoFinalizada = false;
                        relogio.restart();
                    }
                    if (keyEvent->code == sf::Keyboard::Key::Escape) {
                        // Volta para o Menu
                        estadoAtual = EstadoJogo::MENU_SELECAO;
                        window.setSize({ (unsigned int)LARGURA_PADRAO, (unsigned int)ALTURA_PADRAO });
                        window.setView(window.getDefaultView());
                        listaArquivos = listarArquivosPCIN(); // Atualiza lista
                    }
                }
            }
        }

        // --- LÓGICA ---

        if (estadoAtual == EstadoJogo::CALCULANDO) {
            // 1. Renderiza tela de "Calculando..." uma vez para o usuário ver
            window.clear(sf::Color(50, 50, 50));
            sf::Text txtCarregando(fonte);
            txtCarregando.setString("Lendo arquivo e Calculando Solucao...\nPor favor aguarde.");
            txtCarregando.setPosition({ 50.0f, 250.0f });
            window.draw(txtCarregando);
            window.display();

            // Processamento Pesado
            tabuleiroInicial = Parser::carregarProblema(arquivoSelecionado);

            if (tabuleiroInicial) {
                // Ajusta tamanho da janela ao tabuleiro
                int w = tabuleiroInicial->getColunas() * (int)TAMANHO_CELULA;
                int h = tabuleiroInicial->getLinhas() * (int)TAMANHO_CELULA;
                window.setSize({ (unsigned int)w, (unsigned int)h });
                // Ajusta a View para garantir que o desenho ocupe a janela toda
                sf::View view(sf::FloatRect({ 0.f, 0.f }, { (float)w, (float)h }));
                window.setView(view);

                // Roda Solver
                Solver solver;
                ResultadoBusca solucao = solver.encontrarSolucao(*tabuleiroInicial);

                if (solucao.achouMate) {
                    movimentosParaAnimar = solucao.sequencia;
                    mensagemStatus = "Solucao Encontrada!";

                    // === GERAÇÃO DO ARQUIVO .pcout ===
                    std::filesystem::path pathIn(arquivoSelecionado);
                    std::string arqSaida = pathIn.replace_extension(".pcout").string();
                    std::ofstream fileOut(arqSaida);

                    Tabuleiro tabSim = *tabuleiroInicial;
                    int maxL = tabuleiroInicial->getLinhas();

                    std::cout << "\n--- SOLUCAO (" << arqSaida << ") ---" << std::endl;
                    for (const auto& mov : solucao.sequencia) {
                        Peca* p = tabSim.getPecaEm(mov.de);
                        char sigla = p ? getCharPeca(p->getTipo()) : '?';
                        std::string strJogada = std::string(1, sigla) + posParaString(mov.de, maxL) + "-" + posParaString(mov.para, maxL);

                        std::cout << strJogada << std::endl;
                        if (fileOut.is_open()) fileOut << strJogada << std::endl;

                        tabSim = tabSim.fazerMovimento(mov);
                    }
                    
                }
                else {
                    movimentosParaAnimar.clear();
                    mensagemStatus = "Sem solucao encontrada.";
                }

                // Prepara Visualização
                tabuleiroAtual = *tabuleiroInicial;
                indiceMovimento = 0;
                animacaoFinalizada = false;
                relogio.restart();
                estadoAtual = EstadoJogo::VISUALIZANDO;

            }
            else {
                std::cerr << "Erro ao carregar arquivo." << std::endl;
                estadoAtual = EstadoJogo::MENU_SELECAO; // Volta se der erro
            }
        }

        if (estadoAtual == EstadoJogo::VISUALIZANDO) {
            if (!movimentosParaAnimar.empty() && !animacaoFinalizada) {
                if (relogio.getElapsedTime().asSeconds() > tempoPorJogada) {
                    if (indiceMovimento < movimentosParaAnimar.size()) {
                        tabuleiroAtual = tabuleiroAtual.fazerMovimento(movimentosParaAnimar[indiceMovimento]);
                        indiceMovimento++;
                        relogio.restart();
                    }
                    else {
                        animacaoFinalizada = true;
                    }
                }
            }
        }

        // Desenho da janela 
        window.clear(sf::Color(30, 30, 30));

        if (estadoAtual == EstadoJogo::MENU_SELECAO) {
            sf::Text titulo(fonte);
            titulo.setString("Selecione um arquivo de entrada (.pcin):");
            titulo.setPosition({ 50.0f, 30.0f });
            titulo.setFillColor(sf::Color::Yellow);
            window.draw(titulo);

            float y = 100.0f;
            for (const auto& arq : listaArquivos) {
                sf::Text item(fonte);
                item.setString(arq);
                item.setPosition({ 50.0f, y });

                // Efeito de Hover (mouse em cima)
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (item.getGlobalBounds().contains(mousePos)) {
                    item.setFillColor(sf::Color::Cyan);
                    item.setStyle(sf::Text::Bold);
                }
                else {
                    item.setFillColor(sf::Color::White);
                }

                window.draw(item);
                y += 40.0f;
            }

            if (listaArquivos.empty()) {
                sf::Text aviso(fonte);
                aviso.setString("Nenhum arquivo .pcin encontrado na pasta.");
                aviso.setPosition({ 50.0f, 100.0f });
                aviso.setFillColor(sf::Color::Red);
                window.draw(aviso);
            }
        }
        else if (estadoAtual == EstadoJogo::VISUALIZANDO) {
            renderer.desenhar(window, tabuleiroAtual);

            // Desenhar status na tela se acabou
            if (animacaoFinalizada) {
                sf::Text status(fonte);
                status.setString("FIM (Pressione ESC)");
                status.setCharacterSize(20);
                status.setPosition({ 10.0f, 10.0f });
                status.setOutlineThickness(2);
                status.setOutlineColor(sf::Color::Black);
                window.draw(status);
            }
        }

        window.display();
    }

    return 0;
}