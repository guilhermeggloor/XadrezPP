#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

const float TAMANHO_CASA = 90.0f; //Define tamanho entree as casas
const sf::Color COR_MADEIRA_CLARA(240, 217, 181);
const sf::Color COR_MADEIRA_ESCURA(181, 136, 99);
const sf::Color COR_BURACO(80, 80, 80);

class Tabuleiro {
public:
    // Enum para definir o tipo de cada casa
    enum class TipoCasa {
        CLARA,
        ESCURA,
        BURACO
    };

    // Construtor
    Tabuleiro(int linhas, int colunas)
        : m_linhas(linhas), m_colunas(colunas), m_tamanhoCasa(TAMANHO_CASA)
    {
        // Redimensiona o grid para o tamanho correto
        m_grid.resize(m_linhas, vector<TipoCasa>(m_colunas));

        m_shapeCasa.setSize(sf::Vector2f(m_tamanhoCasa, m_tamanhoCasa));

        // Inicializa o tabuleiro com o padrão de xadrez
        for (int i = 0; i < m_linhas; ++i) {
            for (int j = 0; j < m_colunas; ++j) {
                if ((i + j) % 2 == 0) {
                    m_grid[i][j] = TipoCasa::CLARA;
                }
                else {
                    m_grid[i][j] = TipoCasa::ESCURA;
                }
            }
        }
    }

    // Marca uma casa como buraco (usa índices 1-baseados da entrada)
    void setBuraco(int linha_1_idx, int coluna_1_idx) {
        // Converte para 0-indexado e verifica limites
        int i = linha_1_idx - 1;
        int j = coluna_1_idx - 1;

        if (i >= 0 && i < m_linhas && j >= 0 && j < m_colunas) {
            m_grid[i][j] = TipoCasa::BURACO;
        }
        else {
            cerr << "Aviso: Coordenada de buraco (" << linha_1_idx
                << "," << coluna_1_idx << ") esta fora dos limites." << std::endl;
        }
    }

    // Desenha o tabuleiro inteiro na janela
    void draw(sf::RenderWindow& window) {
        for (int i = 0; i < m_linhas; ++i) {
            for (int j = 0; j < m_colunas; ++j) {

                // Define a cor da casa baseado no seu tipo
                switch (m_grid[i][j]) {
                case TipoCasa::CLARA:
                    m_shapeCasa.setFillColor(COR_MADEIRA_CLARA);
                    break;
                case TipoCasa::ESCURA:
                    m_shapeCasa.setFillColor(COR_MADEIRA_ESCURA);
                    break;
                case TipoCasa::BURACO:
                    m_shapeCasa.setFillColor(COR_BURACO);
                    break;
                }

                // Define a posição e desenha na tela
                m_shapeCasa.setPosition(sf::Vector2f(j * m_tamanhoCasa, i * m_tamanhoCasa));
                window.draw(m_shapeCasa);
            }
        }
    }

    // Funções 'getter' para o 'Game' saber o tamanho da janela do tabuleiro
    unsigned int getLarguraPixels() const { return m_colunas * m_tamanhoCasa; }
    unsigned int getAlturaPixels() const { return m_linhas * m_tamanhoCasa; }

private:
    int m_linhas;
    int m_colunas;
    float m_tamanhoCasa;

    vector<vector<TipoCasa>> m_grid; // O estado lógico do tabuleiro
    sf::RectangleShape m_shapeCasa;             // Objeto de desenho reutilizado
};

class Game {
public:
    // Construtor Padrão
    Game() : m_jogadasParaMate(0), m_linhas(0), m_colunas(0) {
        // m_board é inicializado como nullptr
    }

    // Método principal de inicialização
    bool init() {
        // Lê e processa a entrada
        if (!parseInput()) {
            cerr << "Erro: Falha ao ler a entrada." << std::endl;
            return false;
        }

        // Valida os dados de entrada
        if (m_linhas <= 0 || m_linhas > 8 || m_colunas <= 0 || m_colunas > 8) {
            cerr << "Erro: Dimensoes do tabuleiro invalidas (Max 8x8 e Min 1x1)." << std::endl;
            return false;
        }

        // 3. Cria o objeto Tabuleiro
        // make_unique para gerenciar a memória automaticamente.
        // O Game agora "possui" o Tabuleiro.
        m_board = make_unique<Tabuleiro>(m_linhas, m_colunas);

        // Configura os buracos no Tabuleiro
        for (const auto& buraco : m_buracos) {
            m_board->setBuraco(buraco.linha, buraco.coluna);
        }

        // Cria a janela SFML com o tamanho exato do tabuleiro
        m_window.create(
            sf::VideoMode({ m_board->getLarguraPixels(), m_board->getAlturaPixels() }),
            "Chess SFML"
        );
        m_window.setFramerateLimit(60);

        return true;
    }

    // O loop principal do jogo
    void run() {
        while (m_window.isOpen()) {
           // processEvents();
            update(); // (Vazio por enquanto)
            render();
        }
    }

private:
    /*void processEvents() {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
        }
    }*/

    void update() {
        // Lógica do jogo utilizando o algoritmo virá aqui
    }

    void render() {
        m_window.clear();

        // Delega o desenho do tabuleiro para o objeto board
        if (m_board) {
            m_board->draw(m_window);
        }

        // (Futuro) Desenhar as peças aqui...

        m_window.display();
    }

    // Estrutura privada para armazenar buracos lidos
    struct BuracoCoord {
        int linha;
        int coluna;
    };

    bool parseInput() {
        // Primeira linha: Linhas, Colunas, Jogadas
        cin >> m_linhas >> m_colunas >> m_jogadasParaMate;
        if (cin.fail()) return false;

        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer

        // Segunda linha: Buracos
        string linhaBuracos;
        getline(cin, linhaBuracos);

        if (!linhaBuracos.empty()) {
            std::stringstream ss(linhaBuracos);
            int i, j;
            while (ss >> i >> j) {
                m_buracos.push_back({ i, j });
            }
        }

        // Próximas linhas: Peças
        std::string notacaoPeca;
        while (cin >> notacaoPeca) {
            m_pecasPosicionadas.push_back(notacaoPeca);
        }

        // (Opcional) Imprimir o que foi lido para debug
        cerr << "--- DADOS LIDOS (OOP) ---" << std::endl;
        cerr << "Tabuleiro: " << m_linhas << "x" << m_colunas << std::endl;
        cerr << "Jogadas: " << m_jogadasParaMate << std::endl;
        cerr << "Buracos: " << m_buracos.size() << std::endl;
        cerr << "Pecas: " << m_pecasPosicionadas.size() << std::endl;
        cerr << "-------------------------" << std::endl;

        return true;
    }
    int m_linhas;
    int m_colunas;
    int m_jogadasParaMate;
    vector<BuracoCoord> m_buracos;
    vector<string> m_pecasPosicionadas;

    // Componentes SFML e do Jogo
    sf::RenderWindow m_window;
    unique_ptr<Tabuleiro> m_board; // Ponteiro inteligente para o objeto Board
};


int main()
{
    Game game;

    if (game.init()) {
        game.run();
    }
    else {
        cerr << "Falha ao inicializar o jogo." << endl;
        return 1;
    }


    return 0;
}