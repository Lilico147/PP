#pragma once
#include "Core.h" 

class Tabuleiro;

class Rei : public Peca {
public:
    Rei(Cor cor, Posicao pos) : Peca(cor, pos, 'R') {} // Símbolo 'R'

    virtual std::vector<Movimento> calcularMovimentosPossiveis(
        const Tabuleiro& tabuleiro,
        int maxLinhas, int maxColunas,
        bool temBuraco[8][8]
    ) const override;

};