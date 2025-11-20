// Renderer.h
#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>
#include <filesystem> // Para verificar se o arquivo existe
#include "Tabuleiro.h"

class Renderer {
private:
    std::map<std::string, sf::Texture> m_texturas; // Cache das imagens
    float m_tamanhoCelula;
    sf::Font m_fonte;

public:
    Renderer(float tamanhoCelula) : m_tamanhoCelula(tamanhoCelula) {
        // Tenta carregar fonte para fallback/menu
        if (!m_fonte.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            m_fonte.openFromFile("arial.ttf");
        }
        carregarTexturas();
    }

    void carregarTexturas() {
        // Nomes base conforme sugerido (Rei, Dama, etc.)
        std::string nomes[] = { "Rei", "Dama", "Torre", "Bispo", "Cavalo", "Peao" };
        std::string cores[] = { "Branco", "Preto" };

        for (const auto& nome : nomes) {
            for (const auto& cor : cores) {
                // Monta o nome: "Rei" + "Branco" = "ReiBranco"
                std::string chave = nome + cor;

                // Caminho esperado: assets/ReiBranco.png
                std::string caminho = "assets/" + chave + ".png";

                sf::Texture tex;
                // Habilita smooth para a imagem ficar bonita ao redimensionar
                tex.setSmooth(true);

                if (std::filesystem::exists(caminho) && tex.loadFromFile(caminho)) {
                    m_texturas[chave] = tex;
                }
                else {
                    std::cerr << "Aviso: Imagem nao encontrada: " << caminho << std::endl;
                }
            }
        }
    }

    void desenhar(sf::RenderWindow& window, const Tabuleiro& tabuleiro) {
        int linhas = tabuleiro.getLinhas();
        int cols = tabuleiro.getColunas();

        for (int i = 0; i < linhas; ++i) {
            for (int j = 0; j < cols; ++j) {
                float x = j * m_tamanhoCelula;
                float y = i * m_tamanhoCelula;

                // 1. Desenha o quadrado do tabuleiro
                sf::RectangleShape rect(sf::Vector2f{ m_tamanhoCelula, m_tamanhoCelula });
                rect.setPosition(sf::Vector2f{ x, y });

                Posicao pos = { i, j };

                if (!tabuleiro.ePosValida(pos)) {
                    rect.setFillColor(sf::Color(30, 30, 30)); // Buraco
                }
                else {
                    // Cores estilo "Madeira"
                    if ((i + j) % 2 == 0) rect.setFillColor(sf::Color(235, 236, 208)); // Claro (estilo chess.com)
                    else rect.setFillColor(sf::Color(119, 149, 86)); // Verde (estilo chess.com)
                }
                window.draw(rect);

                // 2. Desenha a peça (Sprite)
                if (tabuleiro.ePosValida(pos)) {
                    Peca* peca = tabuleiro.getPecaEm(pos);
                    if (peca != nullptr) {
                        desenharPecaSprite(window, peca, x, y);
                    }
                }
            }
        }
    }

private:
    void desenharPecaSprite(sf::RenderWindow& window, Peca* peca, float x, float y) {
        // Monta a chave para buscar no mapa (ex: "TorrePreta")
        std::string chave = "";
        switch (peca->getTipo()) {
        case TipoPeca::REI: chave += "Rei"; break;
        case TipoPeca::DAMA: chave += "Dama"; break;
        case TipoPeca::TORRE: chave += "Torre"; break;
        case TipoPeca::BISPO: chave += "Bispo"; break;
        case TipoPeca::CAVALO: chave += "Cavalo"; break;
        case TipoPeca::PEAO: chave += "Peao"; break;
        }
        chave += (peca->getCor() == Cor::BRANCA ? "Branco" : "Preto");

        // Verifica se a textura existe no cache
        if (m_texturas.count(chave)) {
            sf::Sprite sprite(m_texturas[chave]);
            // Descobre o tamanho original da imagem
            auto bounds = sprite.getLocalBounds(); // retorna FloatRect
            float larguraImg = bounds.size.x;
            float alturaImg = bounds.size.y;

            // Queremos que a peça ocupe 90% da célula
            float escalaX = (m_tamanhoCelula * 0.90f) / larguraImg;
            float escalaY = (m_tamanhoCelula * 0.90f) / alturaImg;

            // Aplica a escala
            sprite.setScale(sf::Vector2f{ escalaX, escalaY });

            // Centraliza a peça na célula
            float offsetX = (m_tamanhoCelula - (larguraImg * escalaX)) / 2.0f;
            float offsetY = (m_tamanhoCelula - (alturaImg * escalaY)) / 2.0f;

            sprite.setPosition(sf::Vector2f{ x + offsetX, y + offsetY });

            window.draw(sprite);
        }
        else {
			// caso imagem não carregar desenha uma bola simples
            float raio = m_tamanhoCelula * 0.3f;
            sf::CircleShape ball(raio);
            ball.setPosition(sf::Vector2f{ x + m_tamanhoCelula * 0.2f, y + m_tamanhoCelula * 0.2f });

            if (peca->getCor() == Cor::BRANCA) {
                ball.setFillColor(sf::Color::White);
                ball.setOutlineColor(sf::Color::Black);
            }
            else {
                ball.setFillColor(sf::Color::Black);
                ball.setOutlineColor(sf::Color::White);
            }
            ball.setOutlineThickness(2);
            window.draw(ball);
        }
    }
};