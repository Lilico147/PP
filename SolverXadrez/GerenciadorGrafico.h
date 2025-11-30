#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>
#include "EstadoJogo.h" 
#include "Core.h"

//Classe responsável por toda a renderização da interface gráfica

class GerenciadorGrafico {
private:
    // para guardar as imagens
    std::map<char, sf::Texture> m_texturasBrancas;
    std::map<char, sf::Texture> m_texturasPretas;
    sf::Text m_textoVoltarMenu; 

    sf::Vector2f m_tamanhoCasa;
    sf::Vector2f m_origemTabuleiro;


public:
    GerenciadorGrafico() {
        m_tamanhoCasa = { 60.f, 60.f }; // Quadrados de 60x60 pixels
        // Posição na tela para desenhar o tabuleiro
        m_origemTabuleiro = { 150.f, 100.f };
    }

    // Carrega as imagens das peças
    void carregarTexturas(const std::string& basePath) {

        // peças Brancas
        if (!m_texturasBrancas['R'].loadFromFile(basePath + "R_BRANCA.png")) std::cerr << "ERRO: Falha ao carregar R_BRANCA.png" << std::endl;
        if (!m_texturasBrancas['D'].loadFromFile(basePath + "D_BRANCA.png")) std::cerr << "ERRO: Falha ao carregar D_BRANCA.png" << std::endl;
        if (!m_texturasBrancas['T'].loadFromFile(basePath + "T_BRANCA.png")) std::cerr << "ERRO: Falha ao carregar T_BRANCA.png" << std::endl;
        if (!m_texturasBrancas['B'].loadFromFile(basePath + "B_BRANCA.png")) std::cerr << "ERRO: Falha ao carregar B_BRANCA.png" << std::endl;
        if (!m_texturasBrancas['C'].loadFromFile(basePath + "C_BRANCA.png")) std::cerr << "ERRO: Falha ao carregar C_BRANCA.png" << std::endl;
        if (!m_texturasBrancas['P'].loadFromFile(basePath + "P_BRANCA.png")) std::cerr << "ERRO: Falha ao carregar P_BRANCA.png" << std::endl;

        // Rei Preto
        if (!m_texturasPretas['r'].loadFromFile(basePath + "r_PRETA.png")) std::cerr << "ERRO: Falha ao carregar r_PRETA.png" << std::endl;
    }

    // A função principal de desenho
    void desenharEstado(sf::RenderWindow& window, const EstadoJogo& estado, const sf::Font& fonte) {

        m_textoVoltarMenu.setFont(fonte); // Use m_textoVoltarMenu
        m_textoVoltarMenu.setString("Use as setas do teclado (<- / ->) para navegar pelos lances\nClique para voltar ao menu");
        m_textoVoltarMenu.setCharacterSize(18);
        m_textoVoltarMenu.setFillColor(sf::Color::White);
        m_textoVoltarMenu.setOutlineColor(sf::Color::Black);
        m_textoVoltarMenu.setOutlineThickness(1.5f);
        m_textoVoltarMenu.setPosition(150.f, 40.f);

        sf::RectangleShape casa(m_tamanhoCasa);
        sf::Sprite spritePeca;

        // 1. Desenha o Tabuleiro (casas e buracos)
        for (int i = 0; i < estado.m_maxLinhas; ++i) { 
            for (int j = 0; j < estado.m_maxColunas; ++j) { 

                // Posição na tela
                casa.setPosition(m_origemTabuleiro.x + j * m_tamanhoCasa.x,
                    m_origemTabuleiro.y + i * m_tamanhoCasa.y);

                // Cor das casas
                if ((i + j) % 2 == 0) {
                    casa.setFillColor(sf::Color(238, 238, 210)); // Casa clara
                }
                else {
                    casa.setFillColor(sf::Color(118, 150, 86)); // Casa escura
                }

                // Verifica se é um buraco 
                if (estado.m_buracos[i][j]) {
                    casa.setFillColor(sf::Color(50, 50, 50)); 
                }

                window.draw(casa);
            }
        }

        // Desenha as Coordenadas
        sf::Text coord;
        coord.setFont(fonte);
        coord.setCharacterSize(14);
        coord.setFillColor(sf::Color::White);

        // Colunas
        for (int j = 0; j < estado.m_maxColunas; ++j) {
            coord.setString(std::string(1, (char)('a' + j)));
            coord.setPosition(m_origemTabuleiro.x + j * m_tamanhoCasa.x + (m_tamanhoCasa.x / 2) - 5.f,
                m_origemTabuleiro.y + estado.m_maxLinhas * m_tamanhoCasa.y + 5.f);
            window.draw(coord);
        }
        // Linhas
        for (int i = 0; i < estado.m_maxLinhas; ++i) {
            coord.setString(std::to_string(i + 1));
            coord.setPosition(m_origemTabuleiro.x - 20.f,
                m_origemTabuleiro.y + i * m_tamanhoCasa.y + (m_tamanhoCasa.y / 2) - 10.f);
            window.draw(coord);
        }

        // Desenha as Peças
        for (const auto& pecaPtr : estado.m_tabuleiro.getTodasAsPecas()) {
            char simbolo = pecaPtr->getSimbolo(); // 'R', 'T', 'D', 'B', 'C', 'P'
            Posicao pos = pecaPtr->getPosicao();

            // Pega a textura 
            if (pecaPtr->getCor() == Cor::PRETA) {
                char simboloPreto = std::tolower(simbolo); // 'R' vira 'r'
                if (m_texturasPretas.count(simboloPreto)) {
                    spritePeca.setTexture(m_texturasPretas.at(simboloPreto));
                }
                else {
                    continue;
                }
            }
            else {
                // Peças Brancas
                if (m_texturasBrancas.count(simbolo)) {
                    spritePeca.setTexture(m_texturasBrancas.at(simbolo));
                }
                else {
                    continue; 
                }
            }

            // Posiciona o png da peça para caber na casa
            sf::Vector2f tamanhoTextura = (sf::Vector2f)spritePeca.getTexture()->getSize();
            spritePeca.setScale(m_tamanhoCasa.x / tamanhoTextura.x,
                m_tamanhoCasa.y / tamanhoTextura.y);

            // Calcula a posição na tela
            spritePeca.setPosition(m_origemTabuleiro.x + pos.col * m_tamanhoCasa.x,
                m_origemTabuleiro.y + pos.linha * m_tamanhoCasa.y);

            window.draw(spritePeca);
        }
        window.draw(m_textoVoltarMenu);
    }

    // Desenha o menu
    void desenharMenu(sf::RenderWindow& window, const std::vector<sf::Text>& menuItens) {
        window.clear(sf::Color(50, 50, 50));
        for (const auto& item : menuItens) {
            window.draw(item);
        }
    }
};