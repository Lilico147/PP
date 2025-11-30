#pragma once
#include "Core.h"

class Tabuleiro; 

class Bispo : public Peca {
public:
    Bispo(Cor cor, Posicao pos) : Peca(cor, pos, 'B') {} // Símbolo 'B'

    virtual std::vector<Movimento> calcularMovimentosPossiveis(
        const Tabuleiro& tabuleiro,
        int maxLinhas, int maxColunas,
        bool temBuraco[8][8]
    ) const override;

};