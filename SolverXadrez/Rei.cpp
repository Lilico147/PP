#include "Rei.h"
#include "Tabuleiro.h"

std::vector<Movimento> Rei::calcularMovimentosPossiveis(
    const Tabuleiro& tabuleiro,
    int maxLinhas, int maxColunas,
    bool temBuraco[8][8]
) const {

    std::vector<Movimento> movimentos;

    // 8 movimentos possíveis em torno do Rei
    int movimentosPossiveis[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, // Cima 
        { 0, -1},          { 0, 1}, // Lados 
        { 1, -1}, { 1, 0}, { 1, 1}  // Baixo 
    };

    for (int i = 0; i < 8; ++i) {
        int novaLinha = m_posicao.linha + movimentosPossiveis[i][0];
        int novaCol = m_posicao.col + movimentosPossiveis[i][1];

        // 1. Verifica se está dentro do tabuleiro
        if (novaLinha >= 0 && novaLinha < maxLinhas &&
            novaCol >= 0 && novaCol < maxColunas)
        {
            // 2. Verifica se a nova posição não é um buraco
            if (temBuraco[novaLinha][novaCol]) continue;

            // 3. Verifica peças
            Peca* pecaNoDestino = tabuleiro.getPecaEm(Posicao(novaLinha, novaCol));

            // Se é uma peça amiga, pula para próximo movimento
            if (pecaNoDestino) {
                if (pecaNoDestino->getCor() == m_cor)
                    continue; 
             
            }

            Posicao posPara = Posicao(novaLinha, novaCol);
            Movimento mov{ m_posicao, posPara };
            movimentos.push_back(mov); // adiciona apenas movimentos válidos


            // 4. Movimento válido (casa vazia ou captura)
            movimentos.push_back(Movimento{ m_posicao, Posicao(novaLinha, novaCol) });
        }
    }

    return movimentos;
}
