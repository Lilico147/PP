#include "Bispo.h"     
#include "Tabuleiro.h" 
#include <algorithm>   

//Implementação da lógica de movimento do Bispo.

std::vector<Movimento> Bispo::calcularMovimentosPossiveis(
    const Tabuleiro& tabuleiro,
    int maxLinhas, int maxColunas,
    bool temBuraco[8][8]
) const {

    std::vector<Movimento> movimentos; // A lista de movimentos a ser retornada

    // Define as direções de movimento diagonal 
    int direcoes[4][2] = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (int i = 0; i < 4; ++i) {
        for (int k = 1; k < 8; ++k) { 
            int novaLinha = m_posicao.linha + direcoes[i][0] * k;
            int novaCol = m_posicao.col + direcoes[i][1] * k;

            // Se o movimento sair do tabuleiro, pare de deslizar nesta direção
            if (novaLinha < 0 || novaLinha >= maxLinhas ||
                novaCol < 0 || novaCol >= maxColunas) {
                break;
            }

            // Se o movimento atingir um buraco, pare de deslizar
            if (temBuraco[novaLinha][novaCol]) {
                break;
            }

            // Verificação de Colisão
            Peca* pecaNoDestino = tabuleiro.getPecaEm(Posicao(novaLinha, novaCol));
            if (pecaNoDestino) {
                // Se a peça for inimiga, é um movimento válido 
                if (pecaNoDestino->getCor() != m_cor) {
                    Posicao posPara = Posicao(novaLinha, novaCol);
                    Movimento mov = Movimento{ m_posicao, posPara };
                    movimentos.push_back(mov);
                }
                // Se encontrar uma peça, deve parar de deslizar
                break;
            }

            // Se não for limite, nem buraco, nem peça, é uma casa vazia válida
            Posicao posPara = Posicao(novaLinha, novaCol);
            Movimento mov = Movimento{ m_posicao, posPara };
            movimentos.push_back(mov);
        }
    }

    return movimentos; // Retorna todos os movimentos encontrados
}