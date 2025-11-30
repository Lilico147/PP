#pragma once
#include "Core.h"

class Tabuleiro;

class Dama : public Peca {
public:
    Dama(Cor cor, Posicao pos) : Peca(cor, pos, 'D') {} // Símbolo 'D'

    virtual std::vector<Movimento> calcularMovimentosPossiveis(
        const Tabuleiro& tabuleiro,
        int maxLinhas, int maxColunas,
        bool temBuraco[8][8]
    ) const override;

};