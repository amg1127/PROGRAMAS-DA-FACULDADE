class TAdapHuff {
    private:
        TCell arv, *navega;
        int modo;
        void DeixaApenasValidos (std::string &);
    public:
        std::string CodificaCaractere (char);
        char DecodificaCaractere (std::string &);
        void Reinicia (void);
        TAdapHuff();
};
