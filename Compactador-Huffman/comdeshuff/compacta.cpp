#include "common.h"

#define ESCREVEINT(stream,dado) { \
    stream << char((unsigned(dado) & 0xff000000) >> 24); \
    stream << char((unsigned(dado) & 0x00ff0000) >> 16); \
    stream << char((unsigned(dado) & 0x0000ff00) >> 8 ); \
    stream << char((unsigned(dado) & 0x000000ff) >> 0 ); }

int main (int argc, char **argv) {
    int num_desloca, tam_arq, *probabs, i, l, quants;
    char dado, leit;
    bool avisa_null = true;
    std::string difer;
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
    // Ah... Ate poderia fazer em tudo em 2 passadas... Mas agora, estou com preguica. Vou fazer com 3 passadas.
    std::cout << "Etapa 1: localizando caracteres diferentes no arquivo..." << std::endl;
    difer = "";
    tam_arq = 0;
    while (1) {
        leit = entrada.get();
        if (entrada.eof()) break;
        if (leit == 0) {
            if (avisa_null) {
                avisa_null = false;
                std::cerr << " ** Aviso: o algoritmo nao foi preparado para codificar o caractere '\\0' presente no arquivo. Ele sera ignorado. **" << std::endl;
            }
        } else {
            if (difer.find (leit) == std::string::npos)
                difer += leit;
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
    quants = difer.length ();
    if (quants > 254) {
        std::cerr << " ** Erro: existem muitos caracteres diferentes no arquivo. **" << std::endl;
        return (1);
    }
    std::cout << "Etapa 2: contando caracteres presentes no arquivo para calcular probabilidades..." << std::endl;
    NEW_ARRAY_OF_OBJECTS (probabs, int, quants);
    for (i=0; i<quants;i++)
        probabs[i] = 0;
    entrada.clear();
    entrada.seekg (0);
    tam_arq = 0;
    while (1) {
        leit = entrada.get();
        if (entrada.eof()) break;
        tam_arq++;
        if (tam_arq < 0) {
            tam_arq--;
            break;
        }
        if (leit != 0)
            probabs[difer.find(leit)]++;
    }
    std::cout << "Etapa 3: compactando o arquivo de entrada..." << std::endl;
    std::fstream saida (argv[2], std::ios_base::out | std::ios_base::binary);
    if (saida.fail()) {
        std::cerr << " ** Erro: nao pude abrir o arquivo de saida! **" << std::endl;
        return (1);
    }
    THuff algoritmo;
    for (i=0;i<quants;i++) {
        algoritmo.adicionaCaractere (difer[i], probabs[i]);
    }
    saida << char(quants & 0xff);
    for (i=0; i<quants; i++) {
        saida << difer[i];
        ESCREVEINT (saida, probabs[i]);
    }
    entrada.clear();
    entrada.seekg (0);
    while (1) {
        leit = entrada.get();
        if (entrada.eof()) break;
        difer = algoritmo.obtemCodificacao (leit);
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
    }
    while (num_desloca != 8) {
        dado <<= 1;
        num_desloca++;
    }
    saida.put(dado);
    entrada.close();
    saida.close();
    std::cout << "Concluido." << std::endl;
    DEL_ARRAY_OF_OBJECTS (probabs);
    return (0);
}
