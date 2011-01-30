#include "common.h"

#define TESTE_EOF { if (entrada.eof()) { std::cerr << "Erro: EOF inesperado!" << std::endl; return (1); } }
#define NAORECONHECIDO { std::cerr << "Erro: formato de arquivo desconhecido!" << std::endl; return (1); }
#define LEINT(stream,dado) { \
    dado = (unsigned int) (stream.get() & 0xff); \
    TESTE_EOF; \
    dado = (dado << 8) | (unsigned int) (stream.get() & 0xff); \
    TESTE_EOF; \
    dado = (dado << 8) | (unsigned int) (stream.get() & 0xff); \
    TESTE_EOF; \
    dado = (dado << 8) | (unsigned int) (stream.get() & 0xff); \
    TESTE_EOF; }

int main (int argc, char **argv) {
    int num_desloca, tam_arq, probabs, i, l, quants;
    char dado, leit, mops;
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
    THuff algoritmo;
    quants = (unsigned int) (entrada.get());
    TESTE_EOF;
    tam_arq = 0;
    for (i=0; i<quants; i++) {
        leit = entrada.get();
        TESTE_EOF;
        LEINT (entrada, probabs);
        if (probabs <= 0) NAORECONHECIDO;
        tam_arq += probabs;
        if (tam_arq <= 0) NAORECONHECIDO;
        algoritmo.adicionaCaractere (leit, probabs);
    }
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
        l = difer.length ();
        if (l > 4096) NAORECONHECIDO;
        for (i=1; i<l && tam_arq > 0; i++) {
            mops = algoritmo.obtemCaractere (difer.substr (0, i));
            if (mops != 0) {
                saida << mops;
                tam_arq--;
                difer = difer.substr (i, l - i);
                l -= i;
                i = 0;
            }
        }
    }
    entrada.close();
    saida.close();
    std::cout << "Concluido." << std::endl;
    return (0);
}
