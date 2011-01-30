#include "common.h"

int main (void) {
    THuff h;
    int i, tam, prob, j, soma;
    char ch;
    char *vetchar;
    double soma2;
    std::string s;
    std::cout << "Quantos caracteres? ";
    std::cin >> tam;
    if (tam <= 0)
        return (1);
    NEW_ARRAY_OF_OBJECTS(vetchar, char, tam);
    soma = 0;
    for (i=0; i<tam; i++) {
        std::cout << "Digite o caractere: ";
        std::cin >> ch;
        if (ch == 0) {
            i--;
            continue;
        }
        for (j=0; j<i; j++)
            if (vetchar[j] == ch)
                break;
        do {
            std::cout << "Digite a probabilidade do caractere '" << ch << "' aparecer (numero inteiro): ";
            std::cin >> prob;
        } while (prob <= 0);
        soma += prob;
        vetchar[i] = ch;
        h.adicionaCaractere (ch, prob);
        if (i != j)
            i--;
    }
    soma2 = 0;
    std::cout << std::endl << "Codificacao resultante:" << std::endl;
    for (i=0; i<tam; i++) {
        s = h.obtemCodificacao (vetchar[i]);
        std::cout << "    " << vetchar[i] << " = " << s << std::endl;
        soma2 += double (h.obtemPrioridade(vetchar[i]) * s.length()) / soma;
    }
    std::cout << "Comprimento medio da codificacao resultante: " << soma2 << std::endl;
    DEL_ARRAY_OF_OBJECTS (vetchar);
    return (0);
}
