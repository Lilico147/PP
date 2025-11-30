 Este trabalho consiste na implementação de um solver para o jogo de xadrez, simplificado, em C++
 (compilador msvc), com interface gráfica, e com a biblioteca SFML, para SO Windows. Além
 disso, uma documentação será necessária.

 Detalhamento:
 Baseado no jogo Pocket Chess e seguindo as regras internacionais do xadrez (podem ser
 consultadas aqui: https://pt.wikipedia.org/wiki/Leis_do_xadrez. O programa consiste em dado uma
 entrada válida representando:
 • Uma configuração de tabuleiro de xadrez, com tamanho retangular (ou quadrado);
 • Posicionamento de buracos no tabuleiro, que são células do tabuleiro que não podem ser
 usadas ou atravessadas.
 • Posicionamento das peças bancas e pretas, onde o jogador com as peças pretas possui
 apenas o Rei;
 • Número de jogadas para um cheque mate do jogador com peças brancas, onde o jogador
 com as peças brancas realiza o primeiro movimento.
 Seu programa deve calcular uma das possíveis sequência de jogadas que resultam em um cheque
 mate. Estes cálculos devem ser realizados independentemente da interface gráfica, isto é: a
 implementação de uma nova interface (gráfica ou textual) deve ser possível sem a necessidade de
 alterar o código que realiza os cálculos.
 A entrada do problema e saída com a sequência de jogadas, deve ser por arquivo (explicado e
 padronizado abaixo). A interface gráfica deve possibilitar listagem e a seleção de um arquivo de
 entrada e mostrar a sequência de jogadas. Caso não exista um arquivo de saída, realizar os cálculos,
 gerá-lo para em seguida mostrá-lo.
 Use o jogo  Pocket Chess para gerar exemplos de entrada e conferir vossos programas.
 
Persistência:
 Considere a notação algébrica longa, pode ser apendida aqui:
 https://en.wikipedia.org/wiki/Algebraic_notation_(chess), além de outras fontes. Ignore as notações
 referente a roque, cheque, cheque-mate, etc. Só precisamos descrever o tabuleiro inicial, no arquivo
 de entrada e as jogadas realizadas no arquivo de saida.
 O arquivo de entrada (extensão pcin) consistem em:
 • Na primeira linha o número de linhas (max=8), seguido do número de colunas (max=8) do
 tabuleiro e o número de jogadas, do jogador com as peças brancas, para um cheque mate;
 • Na segunda linha, uma sequência de i's j's, indicando os buracos do tabuleiro, ou uma linha
 em branco quando não há buracos;
 • Nas próximas, o posicionamento das peças, seguindo a notação algébrica longa, onde a
 primeira peça é o rei preto.
A saída consiste em arquivo (extensão pcout), contendo em cada linha uma jogada na sequência
 brancas, pretas, brancas,... , pretas, brancas (vide exmplos visto em sala de aula).
 
 Observaçõs:
 • Seu programa, obrigatoriamente, deve prever a inclusão de novos tipos de peças, tanto para
 os arquivos quanto pra a interface gráfica, sem a necessidade de rescrever o código já
 existente. É permitido apenas a recompilação do código, após a extensão e alteração da
 função principal do programa (int main()) e diretivas de pré-processamento.
 • Crie um documento em pdf, explicando como a extensão do código, para inclusão de novas
 peças, deve ser realizado
