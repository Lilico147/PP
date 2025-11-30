#include "Peao.h"
#include "Tabuleiro.h"

std::vector<Movimento> Peao::calcularMovimentosPossiveis(
    const Tabuleiro& tabuleiro,
    int maxLinhas, int maxColunas,
    bool temBuraco[8][8]
) const {

    std::vector<Movimento> movimentos;

    if (m_cor != Cor::BRANCA) {
        return movimentos;
    }

    int dir = 1; // Direção para cima

    // Movimentos para Frente
    int novaLinha = m_posicao.linha + dir;
    int novaCol = m_posicao.col;

    // Pulo de 1 casa
    if (novaLinha < maxLinhas && !temBuraco[novaLinha][novaCol]) {
        if (!tabuleiro.getPecaEm(Posicao(novaLinha, novaCol))) {
            Posicao posPara = Posicao(novaLinha, novaCol);
            Movimento mov = Movimento{ m_posicao, posPara };
            movimentos.push_back(mov);

            // Pulo de 2 casas (só se estiver na linha 1 e o pulo de 1 casa for válido)
            if (m_posicao.linha == 1) {
                int novaLinha2 = m_posicao.linha + (dir * 2);
                if (novaLinha2 < maxLinhas && !temBuraco[novaLinha2][novaCol]) {
                    if (!tabuleiro.getPecaEm(Posicao(novaLinha2, novaCol))) {
                        Posicao posPara2 = Posicao(novaLinha2, novaCol);
                        Movimento mov2 = Movimento{ m_posicao, posPara2 };
                        movimentos.push_back(mov2);
                    }
                }
            }
        }
    }

    // Movimentos de Captura (Diagonais) ---
    int dCol[2] = { -1, 1 }; // Diagonal esquerda e direita
    for (int i = 0; i < 2; ++i) {
        novaLinha = m_posicao.linha + dir;
        novaCol = m_posicao.col + dCol[i];

        if (novaLinha < maxLinhas && novaCol >= 0 && novaCol < maxColunas) {
            if (!temBuraco[novaLinha][novaCol]) {
                Peca* pecaNoDestino = tabuleiro.getPecaEm(Posicao(novaLinha, novaCol));
                // Válido se houver peça inimiga
                if (pecaNoDestino && pecaNoDestino->getCor() == Cor::PRETA) {
                    Posicao posPara = Posicao(novaLinha, novaCol);
                    Movimento mov = Movimento{ m_posicao, posPara };
                    movimentos.push_back(mov);
                }
            }
        }
    }

    return movimentos;
}