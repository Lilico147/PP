#pragma once
#include "Core.h"

class Tabuleiro; 

class Peao : public Peca {
public:
    Peao(Cor cor, Posicao pos) : Peca(cor, pos, 'P') {} // Símbolo 'P'

    virtual std::vector<Movimento> calcularMovimentosPossiveis(
        const Tabuleiro& tabuleiro,
        int maxLinhas, int maxColunas,
        bool temBuraco[8][8]
    ) const override;

};