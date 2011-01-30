#include "common.h"

int main (void) {
    TAdapHuff h;
    std::string teste;
    int i, l;
    std::cout << "Digite a sequencia de caracteres para compactar: ";
    std::cin >> teste;
    l = teste.length();
    for (i=0; i<l; i++)
        std::cout << h.CodificaCaractere (teste[i]);
    std::cout << std::endl;
    return (0);
}
