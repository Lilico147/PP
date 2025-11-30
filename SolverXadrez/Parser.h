#pragma once 
#include "EstadoJogo.h" 
#include "Core.h"      
#include <fstream>     
#include <sstream>     
#include <string>       

/**
 Lê arquivos .pcin e constrói o EstadoJogo inicial.
 Esta classe é a ligação entre os arquivos de entrada e o solver.
*/

class Parser {
public:
    EstadoJogo carregarArquivo(PecaNova& fabrica, const std::string& caminhoArquivo) {

        EstadoJogo estado; // Cria o estado de jogo vazio
        std::ifstream arquivo(caminhoArquivo);
        std::string linha;

        // verifica se o arquivo .pcin foi encontrado.
        if (!arquivo.is_open()) {
            std::cerr << "ERRO PARSER: Nao foi possivel abrir o arquivo: " << caminhoArquivo << std::endl;
            return estado; 
        }

        // Lê a primeira linha: Dimensões e numero de jogadas para o mate.
        int linhas, colunas, numJogadas;
        if (std::getline(arquivo, linha)) {
            std::stringstream ss(linha);
            ss >> linhas >> colunas >> numJogadas;

            estado.m_maxLinhas = linhas;
            estado.m_maxColunas = colunas;
            estado.m_tabuleiro.setDimensoes(linhas, colunas);
            estado.m_jogadasParaMate = numJogadas;
        }

        // Lê a segunda linha que são os buracos.
        if (std::getline(arquivo, linha) && !linha.empty()) {
            std::stringstream ss(linha);
            std::string notacaoBuraco;
            while (ss >> notacaoBuraco) {
                Posicao posBuraco = Posicao::fromNotacao(notacaoBuraco, estado.m_maxLinhas);

                estado.m_tabuleiro.adicionarBuraco(posBuraco);

                // Salva uma cópia no array do EstadoJogo para acesso rápido
                if (posBuraco.linha >= 0 && posBuraco.col >= 0) {
                    estado.m_buracos[posBuraco.linha][posBuraco.col] = true;
                }
            }
        }

        // Lê as linhas restantes que são as localizações das peças.
        estado.m_jogadorAtual = Cor::BRANCA; // Brancas sempre começam

        bool primeiraPecaLida = false;
        while (std::getline(arquivo, linha) && !linha.empty()) {
            char simbolo = linha[0]; 
            std::string notacaoPos = linha.substr(1); 

            Posicao pos = Posicao::fromNotacao(notacaoPos, estado.m_maxLinhas);

            // A primeira peça é o rei preto, o restante são brancas.
            Cor corDaPeca;
            if (!primeiraPecaLida) {
                corDaPeca = Cor::PRETA;
                primeiraPecaLida = true;
            }
            else {
                corDaPeca = Cor::BRANCA;
            }

            std::unique_ptr<Peca> novaPeca = fabrica.criarPeca(simbolo, corDaPeca, pos);

            // Adiciona a peça criada ao tabuleiro.
            if (novaPeca) {
                estado.m_tabuleiro.adicionarPeca(std::move(novaPeca));
            }
            else {
                // Erro se o .pcin pedir uma peça que não foi registrada 
                std::cerr << "ERRO: Peca desconhecida no .pcin: " << simbolo << std::endl;
            }
        }

        arquivo.close();
        return estado; // Retorna o estado do jogo para o solver.
    }
};