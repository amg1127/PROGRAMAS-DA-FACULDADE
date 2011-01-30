#ifndef MENSAGENS_TELA_H
#define MENSAGENS_TELA_H

class mensagens_tela;

#include "common.h"

class mensagens_tela : public QWidget {
    Q_OBJECT;
    private:
        bool comotava;
        QTextEdit *linhas_tela[MAX_ENTRADAS];
        int indicador_linha, ultimo_top;
        QListBox *lstUsuarios;
        QScrollView *qsvMensagens;
        QLabel *lblUsuarios;
        int llu;
        inline void prox_linha() { this->indicador_linha = (this->indicador_linha + 1) % MAX_ENTRADAS; }
        inline void ante_linha() { this->indicador_linha = (this->indicador_linha + MAX_ENTRADAS - 1) % MAX_ENTRADAS; }
        inline QTextEdit *linha_atual() { return (this->linhas_tela[this->indicador_linha]); }
        void PoeMensagem (QString, QFont, QColor);
        void rola_se_tava (void);
        QString lista_usuarios_Nome[MAX_USUARIOS];
        QFont lista_usuarios_Estilo[MAX_USUARIOS];
        QColor lista_usuarios_Cor[MAX_USUARIOS];
        int tam_lista_usuarios;
    protected slots:
        void resizeEvent (QResizeEvent *);
        void qsvMensagensValueChanged (int);
    public:
        void MensagemEntraNaConversa (QString, QFont, QColor);
        void MensagemEntraNaConversa (int);
        void MensagemSaiDaConversa (QString, QFont, QColor);
        void MensagemSaiDaConversa (int);
        void EscreveMensagem (QString, QFont, QColor, QString);
        void EscreveMensagem (int, QString);
        bool EntraNaConversa (QString, QFont, QColor);
        bool SaiDaConversa (QString);
        bool SaiDaConversa (int, QString&, QFont&, QColor&);
        int EstaNaConversa (QString);
        int QuantosConversam (void);
        bool UsuarioNaConversa (int, QString&, QFont&, QColor&);
        void defLarguraListaUsuarios (int);
        mensagens_tela (QWidget * = NULL);
        ~mensagens_tela ();
};

#endif // MENSAGENS_TELA_H
