class THuff {
    private:
        int *vetPrioridades;
        char *vetCaracteres;
        std::string *vetCodificacoes;
        int tamvetores;
        bool recalculaCodificacao;
        void redimensionaVetores (int);
        void calculaCodificacao (void);
        void sortCellInterval (TCell *, int, int);
    public:
        void adicionaCaractere (char, int);
        std::string obtemCodificacao (char);
        int obtemPrioridade (char);
        char obtemCaractere (std::string);
        void removeCaractere (char);
        THuff ();
        ~THuff ();
};
