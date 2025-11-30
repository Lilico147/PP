#pragma once
#include "Core.h"

class Tabuleiro; 

class Cavalo : public Peca {
public:
    Cavalo(Cor cor, Posicao pos) : Peca(cor, pos, 'C') {} // Símbolo 'C'

    virtual std::vector<Movimento> calcularMovimentosPossiveis(
        const Tabuleiro& tabuleiro,
        int maxLinhas, int maxColunas,
        bool temBuraco[8][8]
    ) const override;

};