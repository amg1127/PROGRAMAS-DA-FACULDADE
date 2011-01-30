#include "common.h"

int main (int argc, char **argv) {
    int num_desloca, i, l, tam_arq;
    char dado, leit;
    std::string difer;
    bool avisa0 = true, avisamenos1 = true;
    dado = 0;
    num_desloca = 0;
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <infile> <outfile>" << std::endl;
        return (1);
    }
    std::fstream entrada (argv[1], std::ios_base::in | std::ios_base::binary);
    if (entrada.fail()) {
        std::cerr << " ** Erro: nao pude abrir o arquivo de entrada! **" << std::endl;
        return (1);
    }
    std::cout << "Etapa 1: calculando o tamanho do arquivo de entrada..." << std::endl;
    tam_arq = 0;
    while (1) {
        leit = entrada.get();
        if (entrada.eof()) break;
        if (leit == 0) {
            if (avisa0) {
                std::cerr << " ** Aviso: o algoritmo nao foi preparado para codificar o caractere '\\0' presente no arquivo. Ele sera ignorado. **" << std::endl;
                avisa0 = false;
            }
        } else if (leit == -1) {
            if (avisamenos1) {
                std::cerr << " ** Aviso: o algoritmo nao foi preparado para codificar o caractere '\\xFF' presente no arquivo. Ele sera ignorado. **" << std::endl;
                avisamenos1 = false;
            }
        } else {
            tam_arq++;
            if (tam_arq < 0) {
                tam_arq--;
                std::cerr << " ** Aviso: o tamanho maximo do arquivo eh de 2GB. Ele sera truncado. **" << std::endl;
                break;
            }
        }
    }
    if (! tam_arq) {
        std::cerr << " ** Erro: o arquivo eh vazio... Nada a fazer. **" << std::endl;
        return (1);
    }
    std::cout << "Etapa 2: compactando o arquivo de entrada..." << std::endl;
    std::fstream saida (argv[2], std::ios_base::out | std::ios_base::binary);
    if (saida.fail()) {
        std::cerr << " ** Erro: nao pude abrir o arquivo de saida! **" << std::endl;
        return (1);
    }
    entrada.clear();
    entrada.seekg (0);
    saida << char ((tam_arq & 0xff000000) >> 24);
    saida << char ((tam_arq & 0xff0000) >> 16);
    saida << char ((tam_arq & 0xff00) >> 8);
    saida << char (tam_arq & 0xff);
    TAdapHuff algoritmo;
    while (tam_arq) {
        leit = entrada.get();
        if (leit != 0 && leit != -1) {
            difer = algoritmo.CodificaCaractere (leit);
            l = difer.length();
            for (i=0; i<l; i++) {
                dado <<= 1;
                if (difer[i] == '1')
                    dado++;
                num_desloca++;
                if (num_desloca == 8) {
                    saida.put(dado);
                    num_desloca = 0;
                    dado = 0;
                }
            }
            tam_arq--;
        }
    }
    while (num_desloca != 8 && num_desloca != 0) {
        dado <<= 1;
        num_desloca++;
    }
    saida.put(dado);
    entrada.close();
    saida.close();
    std::cout << "Concluido." << std::endl;
    return (0);
}
