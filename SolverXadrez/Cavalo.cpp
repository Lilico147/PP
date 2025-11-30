#include "Cavalo.h"
#include "Tabuleiro.h" 

 // Implementação da lógica de movimento específica do Cavalo.

std::vector<Movimento> Cavalo::calcularMovimentosPossiveis(
    const Tabuleiro& tabuleiro,
    int maxLinhas, int maxColunas,
    bool temBuraco[8][8]
) const {

    std::vector<Movimento> movimentos; 

    // Faz o L (KKKKK) em todas as direções possiveis
    int movimentosL[8][2] = {
        {-2, -1}, {-2, 1}, 
        {-1, -2}, {-1, 2}, 
        { 1, -2}, { 1, 2}, 
        { 2, -1}, { 2, 1}  
    };

    for (int i = 0; i < 8; ++i) {
        int novaLinha = m_posicao.linha + movimentosL[i][0];
        int novaCol = m_posicao.col + movimentosL[i][1];


        // Se o movimento sair do tabuleiro, ignore
        if (novaLinha < 0 || novaLinha >= maxLinhas ||
            novaCol < 0 || novaCol >= maxColunas) {
            continue;
        }

        // Se o movimento terminar em um buraco, ignore
        // (O Cavalo pode pular os buracos no caminho, mas não pode pousar neles)
        if (temBuraco[novaLinha][novaCol]) {
            continue;
        }

        Peca* pecaNoDestino = tabuleiro.getPecaEm(Posicao(novaLinha, novaCol));

        // Verifica se a casa de destino está vazia ou ocupada por um inimigo
        if (!pecaNoDestino || pecaNoDestino->getCor() != m_cor) {
            // Se a casa não tiver uma peça da mesma cor, o movimento é válido
            Posicao posPara = Posicao(novaLinha, novaCol);
            Movimento mov = Movimento{ m_posicao, posPara };
            movimentos.push_back(mov);
        }
    }

    return movimentos; // Retorna a lista de todos os movimentos encontrados
}