#pragma once
#include "EstadoJogo.h" 
#include <vector>
#include <iostream>
#include <algorithm> 

 // Encontra a sequencia de xeque-mate usando busca recursiva.

class Solver {
public:

    // Vetor que armazena a sequência de movimentos da solução encontrada.
    std::vector<Movimento> solucao;

    // Flag para parar a busca recursiva assim que uma solução for achada.
    bool encontrouSolucao;

    Solver() : encontrouSolucao(false) {}

    void encontrarMate(EstadoJogo& estadoInicial) {
        int profundidadeMax = (estadoInicial.m_jogadasParaMate * 2) - 1;

        solucao.clear();
        encontrouSolucao = false;

        // Inicia a busca recursiva
        dfsSolver(estadoInicial, profundidadeMax);

        // Se a busca teve sucesso, inverte o vetor da solução
        if (encontrouSolucao) {
            std::cout << "SOLUCAO ENCONTRADA:" << std::endl;
            std::reverse(solucao.begin(), solucao.end());
        }
        else {
            std::cout << "Nenhuma solucao encontrada." << std::endl;
        }
    }

private:

    bool dfsSolver(EstadoJogo& estado, int profundidade) {

        // Se o jogador esta em mate...
        if (estado.isCheckMate()) {
            if (profundidade == 0) {
                encontrouSolucao = true; // Sinaliza para todas as outras buscas pararem
                return true; // Encontrou o caminho.
            }
            return false;
        }

        // Fim da Busca sem mate.
        if (profundidade == 0) {
            return false;
        }

        // Gera todos os movimentos legais para o jogador atual.
        std::vector<Movimento> movimentos = estado.gerarMovimentosLegais();
        std::sort(movimentos.begin(), movimentos.end());

        // Se não há movimentos legais e não é xeque.
        if (movimentos.empty()) {
            return false;
        }

        for (const auto& mov : movimentos) {

            // Aplica o movimento ao tabuleiro e guarda a peça capturada 
            std::unique_ptr<Peca> capturada = estado.fazerMovimento(mov);

            if (dfsSolver(estado, profundidade - 1)) {
                // achou o mate.
                solucao.push_back(mov); // Salva este movimento na solução
                return true; 
            }

            // Se a chamada recursiva falhou, desfaz o movimento para restaurar o tabuleiro e tentar o próximo movimento.
            estado.desfazerMovimento(mov, std::move(capturada));

            // Se outra busca já achou a solução, paramos esta busca.
            if (encontrouSolucao) return true;
        }

        return false;
    }
};