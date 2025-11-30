#pragma once
#include "Tabuleiro.h"
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm> 

class EstadoJogo {
public:
    Tabuleiro m_tabuleiro;
    Cor m_jogadorAtual;
    int m_maxLinhas;
    int m_maxColunas;
    bool m_buracos[8][8]; 
    int m_jogadasParaMate;

public:
    // Classe central, ela armazena o tabuleiro,sabe de quem é a vez e tem as regras principais.
    EstadoJogo()
        : m_jogadorAtual(Cor::BRANCA), m_maxLinhas(0), m_maxColunas(0), m_jogadasParaMate(0)
    {
        // Zera os buracos
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                m_buracos[i][j] = false;
            }
        }
    }

     // Movimento a ser executado. retorna um ponteiro para peça que foi capturada.

    std::unique_ptr<Peca> fazerMovimento(const Movimento& mov) {
        Peca* peca = m_tabuleiro.getPecaEm(mov.de);
        if (!peca) return nullptr;

        std::unique_ptr<Peca> capturada = m_tabuleiro.removerPecaEm(mov.para);
        peca->setPosicao(mov.para);
        m_jogadorAtual = (m_jogadorAtual == Cor::BRANCA ? Cor::PRETA : Cor::BRANCA);
        return capturada;
    }

    // Desfaz um movimento.
    void desfazerMovimento(const Movimento& mov, std::unique_ptr<Peca> pecaCapturada) {
        Peca* peca = m_tabuleiro.getPecaEm(mov.para);
        if (peca) {
            peca->setPosicao(mov.de);
        }
        if (pecaCapturada) {
            m_tabuleiro.adicionarPeca(std::move(pecaCapturada));
        }
        m_jogadorAtual = (m_jogadorAtual == Cor::BRANCA ? Cor::PRETA : Cor::BRANCA);
    }

      // Verifica se o rei esta em xeque.
    
    bool isCheck(Cor corReiAlvo) {
        Peca* rei = m_tabuleiro.getRei(corReiAlvo);
        if (!rei) return false; 

        Posicao posRei = rei->getPosicao();
        Cor atacante = (corReiAlvo == Cor::BRANCA ? Cor::PRETA : Cor::BRANCA);  // A cor do Rei que queremos verificar.

        for (const auto& pecaPtr : m_tabuleiro.getTodasAsPecas()) {
            if (pecaPtr->getCor() != atacante) // Se a peça não for atacante, pule.
                continue;

            auto movs = pecaPtr->calcularMovimentosPossiveis(
                m_tabuleiro,
                m_maxLinhas,
                m_maxColunas,
                m_buracos
            );

            // Verifica se algum desses movimentos dá na casa do Rei
            for (const auto& mov : movs) {
                if (mov.para.linha == posRei.linha && mov.para.col == posRei.col)
                    return true;
            }
        }
        return false;
    }

    // Gera uma lista de todos os movimentos legais para o jogador atual.
    std::vector<Movimento> gerarMovimentosLegais() {
        std::vector<Movimento> lista;
        Cor cor = m_jogadorAtual;

        for (const auto& pecaPtr : m_tabuleiro.getTodasAsPecas()) {
            if (pecaPtr->getCor() != cor) continue; // Se a peça não for do jogador atual, pule

            auto movs = pecaPtr->calcularMovimentosPossiveis(
                m_tabuleiro,
                m_maxLinhas,
                m_maxColunas,
                m_buracos
            );

            for (auto mov : movs) {
                std::unique_ptr<Peca> capt = fazerMovimento(mov);
                bool selfCheck = isCheck(cor);
                desfazerMovimento(mov, std::move(capt));

                if (!selfCheck) {
                    // para o Mate em 2
                    Cor oponente = (cor == Cor::BRANCA ? Cor::PRETA : Cor::BRANCA);
                    if (isCheck(oponente))
                        mov.score = 1000; // Marque com prioridade alta
                    else
                        mov.score = 0;

                    lista.push_back(mov);
                }
            }
        }
        return lista;
    }

    // Verifica se o jogador atual esta em xeque-mate.
    bool isCheckMate() { 
        // Condição 1: deve estar em xeque.
        if (!isCheck(m_jogadorAtual))
            return false;
        // Condição 2: não deve ter nenhum movimento legal para escapar.
        if (gerarMovimentosLegais().empty())
            return true;

        return false;
    }
};