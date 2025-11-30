#include <SFML/Graphics.hpp> 
#include <iostream>           
#include <filesystem>        
#include <vector>              

// Inclui todas as peças.
#include "Rei.h"
#include "Dama.h"
#include "Torre.h"
#include "Bispo.h"
#include "Cavalo.h"
#include "Peao.h"
#include "Parser.h"  // Lê os arquivos.
#include "Solver.h"  // O motor que encontra o mate.
#include "GerenciadorGrafico.h" // Classe que desenha o tabuleiro e as peças.

using namespace std;

// Escreve a solução no arquivo .pcout.
void escreverSolucao(const string& arquivoPcout, const vector<Movimento>& solucao) {
    ofstream arquivo(arquivoPcout);
    if (!arquivo.is_open()) {
        cerr << "ERRO: Nao foi possivel criar o arquivo de saida: " << arquivoPcout << endl;
        return;
    }
    for (const auto& mov : solucao) {
        arquivo << mov.toNotacao() << endl; 
    }
    arquivo.close();
}


// Lista todos os arquivos .pcin do diretório.
vector<string> listarArquivosPcin(const string& diretorio) {
    vector<string> arquivos;
    try {
        for (const auto& entry : filesystem::directory_iterator(diretorio)) {
            string nomeArquivo = entry.path().filename().string();
            if (nomeArquivo.length() > 5 && nomeArquivo.substr(nomeArquivo.length() - 5) == ".pcin") {
                arquivos.push_back(nomeArquivo);
            }
        }
    }
    catch (filesystem::filesystem_error& e) {
        cerr << "Erro ao listar arquivos: " << e.what() << endl;
    }
    return arquivos;
}

int main()
{

    string basePath = "../../Testes e Midias/"; //Ajustar diretório para arquivos de midia e testes


    // Ensina a fabrica a construir cada tipo de peça
    PecaNova fabrica;
    fabrica.registrarPeca('R', [](Cor cor, Posicao pos) { return make_unique<Rei>(cor, pos); });
    fabrica.registrarPeca('D', [](Cor cor, Posicao pos) { return make_unique<Dama>(cor, pos); });
    fabrica.registrarPeca('T', [](Cor cor, Posicao pos) { return make_unique<Torre>(cor, pos); });
    fabrica.registrarPeca('B', [](Cor cor, Posicao pos) { return make_unique<Bispo>(cor, pos); });
    fabrica.registrarPeca('C', [](Cor cor, Posicao pos) { return make_unique<Cavalo>(cor, pos); });
    fabrica.registrarPeca('P', [](Cor cor, Posicao pos) { return make_unique<Peao>(cor, pos); });

    // Inicializa a SFML
    sf::RenderWindow window(sf::VideoMode(950, 700), "Trabalho de PP - Xadrez");
    window.setFramerateLimit(60);

    // Carrega o arquivo de fonte
    sf::Font fonte;
    if (!fonte.loadFromFile(basePath + "arial.ttf")) {
        cerr << "Erro ao carregar a fonte!" << endl;
        return -1;
    }

    // Cria o objeto que vai cuidar de todo o desenho
    GerenciadorGrafico gui;
    gui.carregarTexturas(basePath); // Carrega as imagens 


    // Controla o que está sendo exibido na tela
    enum class EstadoGUI { MENU, EXIBINDO };
    EstadoGUI estadoAtual = EstadoGUI::MENU;

    // Variáveis de estado para navegação
    EstadoJogo estadoAtualParaDesenhar; // O tabuleiro que será modificado
    vector<Movimento> solucaoCompleta;   // A lista de jogadas 
    vector<unique_ptr<Peca>> pecasCapturadasStack; // Pilha para o desfazer
    int indiceJogadaAtual = 0;

    string solucaoStr = ""; // texto da solução 

    // Lógica do menu
    vector<string> arquivosPcin = listarArquivosPcin(basePath);
    vector<sf::Text> menuItens;

    float posY = 10.f;
    for (const string& nomeArquivo : arquivosPcin) {
        sf::Text item;
        item.setFont(fonte);
        item.setString(nomeArquivo);
        item.setCharacterSize(20);
        item.setFillColor(sf::Color::White);
        item.setPosition(10.f, posY);
        menuItens.push_back(item);
        posY += 25.f;
    }

    // Prepara o objeto que mostrará a solução
    sf::Text textoSolucao;
    textoSolucao.setFont(fonte);
    textoSolucao.setCharacterSize(20);
    textoSolucao.setFillColor(sf::Color::Yellow);
    textoSolucao.setPosition(650.f, 100.f);
    textoSolucao.setOutlineColor(sf::Color::Black);
    textoSolucao.setOutlineThickness(2.f);

    // O Loop Principal
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Fechar a janela
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Clique do Mouse
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {

                    sf::Vector2i posMouse = sf::Mouse::getPosition(window);

                    // Verifica o clique nos botões
                    if (estadoAtual == EstadoGUI::MENU) {
                        for (int i = 0; i < menuItens.size(); ++i) {
                            if (menuItens[i].getGlobalBounds().contains((float)posMouse.x, (float)posMouse.y)) {

                                string arquivoProblema = menuItens[i].getString();
                                string arquivoSolucao = arquivoProblema;
                                arquivoSolucao.replace(arquivoSolucao.length() - 5, 5, ".pcout");
                                arquivoSolucao = basePath + arquivoSolucao;

                                cout << "\nResolvendo: " << arquivoProblema << endl;

                                Parser parser;

                                // Carrega o estado para o solver 
                                EstadoJogo estadoParaResolver = parser.carregarArquivo(fabrica, basePath + arquivoProblema);                                // Carrega uma outra cópia para a interface desenhar
                                estadoAtualParaDesenhar = parser.carregarArquivo(fabrica, basePath + arquivoProblema);

                                // Chama o Solver 
                                Solver solver;
                                solver.encontrarMate(estadoParaResolver);

                                // Limpa e armazena a nova solução
                                solucaoCompleta.clear();
                                pecasCapturadasStack.clear();
                                indiceJogadaAtual = 0; // Reseta o índice para o início

                                if (solver.encontrouSolucao) {
                                    solucaoCompleta = solver.solucao; // Guarda a lista de movimentos
                                    escreverSolucao(arquivoSolucao, solucaoCompleta);

                                    // Imprime no console
                                    for (const auto& mov : solver.solucao) {
                                        std::cout << mov.toNotacao() << std::endl;
                                    }
                                }

                                estadoAtual = EstadoGUI::EXIBINDO; // tela de exibição
                            }
                        }
                    }
                    // Qualquer clique volta ao menu
                    else if (estadoAtual == EstadoGUI::EXIBINDO) {
                        estadoAtual = EstadoGUI::MENU;
                    }
                }
            }

            // Lógica de Teclado para a navegação da solução
            if (event.type == sf::Event::KeyPressed) {

                if (estadoAtual == EstadoGUI::EXIBINDO) {

                    // Seta ->
                    if (event.key.code == sf::Keyboard::Right) {
                        if (indiceJogadaAtual < solucaoCompleta.size()) {
                            const Movimento& mov = solucaoCompleta[indiceJogadaAtual];
                            unique_ptr<Peca> capturada = estadoAtualParaDesenhar.fazerMovimento(mov);
                            pecasCapturadasStack.push_back(std::move(capturada));
                            indiceJogadaAtual++;
                        }
                    }
                    // Seta <-
                    else if (event.key.code == sf::Keyboard::Left) {
                        if (indiceJogadaAtual > 0) {
                            indiceJogadaAtual--;
                            const Movimento& mov = solucaoCompleta[indiceJogadaAtual];
                            unique_ptr<Peca> pecaRestaurada = std::move(pecasCapturadasStack.back());
                            pecasCapturadasStack.pop_back();
                            estadoAtualParaDesenhar.desfazerMovimento(mov, std::move(pecaRestaurada));
                        }
                    }
                }

                if (event.key.code == sf::Keyboard::Escape) {
                    estadoAtual = EstadoGUI::MENU;
                }
            }
        } // Fim do loop de eventos

        // Desenho 
        window.clear(sf::Color(50, 50, 50));

        if (estadoAtual == EstadoGUI::MENU) {
            gui.desenharMenu(window, menuItens);
        }
        else if (estadoAtual == EstadoGUI::EXIBINDO) {

            // Atualiza o texto da solução 
            if (solucaoCompleta.empty()) {
                solucaoStr = "Nenhuma solucao encontrada.";
            }
            else {
                solucaoStr = "SOLUCAO (Passo " + to_string(indiceJogadaAtual) + "/" + to_string(solucaoCompleta.size()) + ")\n\n";
                // Mostra a lista de movimentos
                for (const auto& mov : solucaoCompleta) {
                    solucaoStr += mov.toNotacao() + "\n";
                }
            }
            textoSolucao.setString(solucaoStr);

            // Desenha o tabuleiro e as peças no estado atual
            gui.desenharEstado(window, estadoAtualParaDesenhar, fonte);

            // Desenha o texto da solução
            window.draw(textoSolucao);
        }
        window.display();
    }

    return 0;
}