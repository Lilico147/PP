#include "Torre.h"
#include "Tabuleiro.h"

std::vector<Movimento> Torre::calcularMovimentosPossiveis(
    const Tabuleiro& tabuleiro,
    int maxLinhas, int maxColunas,
    bool temBuraco[8][8]
) const {

    std::vector<Movimento> movimentos;
    // 4 direções da torre
    int direcoes[4][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    for (int i = 0; i < 4; ++i) {
        for (int k = 1; k < 8; ++k) { // deslizar
            int novaLinha = m_posicao.linha + direcoes[i][0] * k;
            int novaCol = m_posicao.col + direcoes[i][1] * k;

            // 1. Limites
            if (novaLinha < 0 || novaLinha >= maxLinhas || novaCol < 0 || novaCol >= maxColunas)
                break;

            // 2. Buracos
            if (temBuraco[novaLinha][novaCol])
                break;

            // 3. Peças
            Peca* pecaNoDestino = tabuleiro.getPecaEm(Posicao(novaLinha, novaCol));
            if (pecaNoDestino) {
                if (pecaNoDestino->getCor() != m_cor) {
                    movimentos.push_back(Movimento{ m_posicao, Posicao(novaLinha, novaCol) });
                }
                break; // para deslizar após qualquer peça
            }

            // 4. Casa vazia
            movimentos.push_back(Movimento{ m_posicao, Posicao(novaLinha, novaCol) });
        }
    }

    return movimentos;
}
