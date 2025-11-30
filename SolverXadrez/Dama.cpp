#include "Dama.h"   
#include "Tabuleiro.h" 

// Implementação da lógica de movimento da Dama.

std::vector<Movimento> Dama::calcularMovimentosPossiveis(
    const Tabuleiro& tabuleiro,
    int maxLinhas, int maxColunas,
    bool temBuraco[8][8]
) const {

    std::vector<Movimento> movimentos; 

    // Define as direções de movimento
    int direcoes[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},   // Lógica da Torre 
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}  // Lógica do Bispo 
    };

    for (int i = 0; i < 8; ++i) {
        for (int k = 1; k < 8; ++k) {
            int novaLinha = m_posicao.linha + direcoes[i][0] * k;
            int novaCol = m_posicao.col + direcoes[i][1] * k;

            // Se saiu do tabuleiro, pare de deslizar.
            if (novaLinha < 0 || novaLinha >= maxLinhas || novaCol < 0 || novaCol >= maxColunas)
                break;

            // Se bateu em um buraco, pare.
            if (temBuraco[novaLinha][novaCol])
                break;

            // Verificação de Colisão
            Peca* pecaNoDestino = tabuleiro.getPecaEm(Posicao(novaLinha, novaCol));
            if (pecaNoDestino) {
                // Se for inimiga, captura
                if (pecaNoDestino->getCor() != m_cor) {
                    movimentos.push_back(Movimento{ m_posicao, Posicao(novaLinha, novaCol) });
                }
                break;
            }

            movimentos.push_back(Movimento{ m_posicao, Posicao(novaLinha, novaCol) });
        }
    }

    return movimentos; // Retorna a lista de todos os movimentos encontrados
}