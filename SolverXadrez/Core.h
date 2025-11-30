#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <cctype>    

class Tabuleiro;

// Cor da Peça
enum class Cor { BRANCA, PRETA, NENHUMA };

// Posição no tabuleiro (linha, coluna)
struct Posicao {
    int linha;
    int col;

    Posicao(int l = -1, int c = -1) : linha(l), col(c) {}

    bool operator==(const Posicao& o) const {
        return linha == o.linha && col == o.col;
    }

    // Converte a notação algébrica em coordenadas de array
    static Posicao fromNotacao(const std::string& notacao, int maxLinhas = 8) {
        if (notacao.length() < 2) return { -1, -1 };
        int col = notacao[0] - 'a';
        int linha = notacao[1] - '1';
        return { linha, col };
    }

    // escrever o arquivo de saída .pcout
    std::string toNotacao() const {
        if (linha == -1) return "??";
        std::string s = "";
        s += (char)('a' + col);
        s += (char)('1' + linha); 
        return s;
    }
}; 

// Define um Movimento
struct Movimento {
    Posicao de;
    Posicao para;
    int score = 0;

    bool operator<(const Movimento& outro) const { return score > outro.score; }
    std::string toNotacao() const { return de.toNotacao() + para.toNotacao(); }

}; 

// Arquitetura das Peças

class Peca {
protected:
    Cor m_cor;
    Posicao m_posicao;
    char m_simbolo;

public:
    Peca(Cor cor, Posicao pos, char simbolo) : m_cor(cor), m_posicao(pos), m_simbolo(simbolo) {}
    virtual ~Peca() = default;

    virtual std::vector<Movimento> calcularMovimentosPossiveis(
        const Tabuleiro& tabuleiro,
        int maxLinhas, int maxColunas,
        bool temBuraco[8][8]
    ) const = 0;

    void setPosicao(Posicao novaPos) { m_posicao = novaPos; }
    Cor getCor() const { return m_cor; }
    Posicao getPosicao() const { return m_posicao; }
    char getSimbolo() const { return m_simbolo; }
};

// fabrica de Peças
using ConstrutorPeca = std::function<std::unique_ptr<Peca>(Cor, Posicao)>;

class PecaNova {
private:
    std::map<char, ConstrutorPeca> m_construtores;
public:
    void registrarPeca(char simbolo, ConstrutorPeca construtor) { m_construtores[simbolo] = construtor; }

    std::unique_ptr<Peca> criarPeca(char simbolo, Cor cor, Posicao pos) {
        char simboloMaiusculo = std::toupper(simbolo);
        auto it = m_construtores.find(simboloMaiusculo);
        if (it != m_construtores.end()) {
            return it->second(cor, pos);
        }
        return nullptr;
    }
};