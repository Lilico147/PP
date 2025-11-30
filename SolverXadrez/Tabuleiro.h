#pragma once 
#include "Core.h" 
#include <vector>
#include <memory>
 
 // Armazena o estado físico do jogo, todas as peças e buracos.

class Tabuleiro {
private:
    int m_linhas;
    int m_colunas;
    bool m_buracos[8][8]; 

    std::vector<std::unique_ptr<Peca>> m_pecas;

public:
    // Construtor: Apenas inicializa os buracos como falsos.
    Tabuleiro() : m_linhas(0), m_colunas(0) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                m_buracos[i][j] = false;
            }
        }
    }

    // Funções de Configuração
    void setDimensoes(int linhas, int colunas) {
        m_linhas = linhas;
        m_colunas = colunas;
    }

    void adicionarBuraco(Posicao pos) {
        if (pos.linha >= 0 && pos.linha < 8 && pos.col >= 0 && pos.col < 8) {
            m_buracos[pos.linha][pos.col] = true;
        }
    }

    // Adiciona uma peça ao vetor.
    void adicionarPeca(std::unique_ptr<Peca> peca) {
        if (peca) {
            // transfere a propriedade do ponteiro para dentro do vetor.
            m_pecas.push_back(std::move(peca));
        }
    }

    int getLinhas() const { return m_linhas; }
    int getColunas() const { return m_colunas; }

    // Verifica se uma posição específica é um buraco.
    bool isBuraco(Posicao pos) const {
        if (pos.linha >= 0 && pos.linha < 8 && pos.col >= 0 && pos.col < 8) {
            return m_buracos[pos.linha][pos.col];
        }
        return true; // Considera fora do tabuleiro como um buraco.
    }

    // Retorna um ponteiro bruto para a peça em uma posição, ou 'nullptr' se vazia.
    Peca* getPecaEm(Posicao pos) const {
        for (const auto& peca : m_pecas) {
            Posicao pPos = peca->getPosicao();
            if (pPos.linha == pos.linha && pPos.col == pos.col) {
                return peca.get(); // retorna o ponteiro bruto.
            }
        }
        return nullptr; // Casa vazia.
    }

    
    //Remove uma peça.
    std::unique_ptr<Peca> removerPecaEm(Posicao pos) {
        for (auto it = m_pecas.begin(); it != m_pecas.end(); ++it) {
            Posicao pPos = (*it)->getPosicao();
            if (pPos.linha == pos.linha && pPos.col == pos.col) {
                std::unique_ptr<Peca> pecaCapturada = std::move(*it); // Transfere a propriedade
                m_pecas.erase(it); // Remove o ponteiro vazio da lista
                return pecaCapturada;
            }
        }
        return nullptr; // Nenhuma peça para remover
    }

    // Retorna uma referência constante a vetores de peças.
    const std::vector<std::unique_ptr<Peca>>& getTodasAsPecas() const {
        return m_pecas;
    }

    // Encontra o Rei de uma cor específica (Branco ou Preto).
    
    Peca* getRei(Cor cor) const {
        for (const auto& peca : m_pecas) {
            // Procura o rei correto.
            if (peca->getCor() == cor && peca->getSimbolo() == 'R')
            {
                return peca.get();
            }
        }
        return nullptr; // Rei não encontrado
    }
};