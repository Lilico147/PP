#pragma once
#include "Core.h"

class Tabuleiro;

class Torre : public Peca {
public:
    Torre(Cor cor, Posicao pos) : Peca(cor, pos, 'T') {} // Símbolo 'T'

    virtual std::vector<Movimento> calcularMovimentosPossiveis(
        const Tabuleiro& tabuleiro,
        int maxLinhas, int maxColunas,
        bool temBuraco[8][8]
    ) const override;

};