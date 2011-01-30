#include "common.h"

#define TESTE_EOF { if (entrada.eof()) { std::cerr << "Erro: EOF inesperado!" << std::endl; return (1); } }
#define NAORECONHECIDO { std::cerr << "Erro: formato de arquivo desconhecido!" << std::endl; return (1); }

int main (int argc, char **argv) {
    int tam_arq, num_desloca, l;
    char dado, mops;
    std::string difer;
    dado = 0;
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <infile> <outfile>" << std::endl;
        return (1);
    }
    std::fstream entrada (argv[1], std::ios_base::in | std::ios_base::binary);
    if (entrada.fail()) {
        std::cerr << "Nao pude abrir o arquivo de entrada!" << std::endl;
        return (1);
    }
    std::cout << "Etapa 1: processando o cabecalho do arquivo..." << std::endl;
    TAdapHuff algoritmo;
    tam_arq = (unsigned int) (entrada.get() & 0xff); \
    TESTE_EOF;
    tam_arq = (tam_arq << 8) | (unsigned int) (entrada.get() & 0xff);
    TESTE_EOF;
    tam_arq = (tam_arq << 8) | (unsigned int) (entrada.get() & 0xff);
    TESTE_EOF;
    tam_arq = (tam_arq << 8) | (unsigned int) (entrada.get() & 0xff);
    TESTE_EOF;
    if (tam_arq <= 0) NAORECONHECIDO;
    std::cout << "Etapa 2: descompactando o arquivo..." << std::endl;
    std::fstream saida (argv[2], std::ios_base::out | std::ios_base::binary);
    if (saida.fail()) {
        std::cerr << " ** Erro: nao pude abrir o arquivo de saida! **" << std::endl;
        return (1);
    }
    difer = "";
    while (tam_arq > 0) {
        dado = entrada.get();
        TESTE_EOF;
        for (num_desloca=7; num_desloca>=0; num_desloca--)
            if (dado & (1 << num_desloca))
                difer += "1";
            else
                difer += "0";
        mops = algoritmo.DecodificaCaractere (difer);
        while (mops != 0 && tam_arq > 0) {
            saida << mops;
            tam_arq--;
            mops = algoritmo.DecodificaCaractere (difer);
        }
        l = difer.length ();
        if (l > 4096) NAORECONHECIDO;
    }
    entrada.close();
    saida.close();
    std::cout << "Concluido." << std::endl;
    return (0);
}
