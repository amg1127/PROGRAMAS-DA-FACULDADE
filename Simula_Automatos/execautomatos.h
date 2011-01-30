#ifndef __EXECAUTOMATOS_H
#define __EXECAUTOMATOS_H

#include "common.h"
#include "automato.h"

class execautomatos;
class threadexecutaautomato;

/*  Momentos de reflexao:

    Quantos passos preciso para o assistente?
        #1 Apresentacao
        #2 Carregar dados de entrada
        #3 Configurar como a saida deve ser exibida
        #4 Executar automato (em thread separada, para nao dar a sensacao de congelamento)
        #5 Exibir dados de saida
    O que eu preciso em todos os passos?
        # Um QPushButton para voltar
        # Um QPushButton para avanca
    O que eu preciso para o passo #1?
        # Um QLabel mostrando algum 'blablabla'...
    O que eu preciso para o passo #2?
        # Um QLabel mostrando para o usuario onde ele precisa digitar as palavras
        # Um QPushButton para o usuario poder carregar palavras de um arquivo
        # Um QTextEdit para o usuario inserir os dados de entrada
    O que eu preciso para o passo #3?
        # Um QLabel mostrando o texto 'Opcoes de visualizacao dos resultados'
        # Um QRadioButton para o item 'Apenas mostrar as palavras reconhecidas'
        # Um QRadioButton para o item 'Apenas mostrar as palavras nao-reconhecidas'
        # Um QRadioButton para o item 'Mostrar as palavras reconhecidas apos as nao-reconhecidas'
        # Um QRadioButton para o item 'Mostrar as palavras reconhecidas antes das nao-reconhecidas'
        # Um QRadioButton para o item 'Mostrar as palavras na ordem em que foram inseridas'
    O que eu preciso para o passo #4?
        # Um QProgressBar para mostrar ao usuario o andamento da execucao
        # Um QLabel com a palavra 'Progresso'
    O que eu preciso para o passo #5?
        # Um QLabel mostrando para o usuario onde estao os dados de saida
        # Um QPushButton para o usuario poder salvar o resultado em um arquivo
        # Um QTextEdit somente leitura para o usuario ver os dados de saida
*/

class execautomatos : public QDialog {
    Q_OBJECT;
    private:
        int passo;
        void resizeEvent (QResizeEvent *);
        void closeEvent (QCloseEvent *);
        QPushButton* btnVoltar;
        QPushButton* btnAvancar;
        QLabel* lblTelaInicial;
        QLabel* lblDadosEntrada;
        QPushButton* btnDadosEntrada;
        QTextEdit* txtDadosEntrada;
        QProgressBar* pbarProgresso;
        QLabel* lblProgresso;
        QLabel* lblDadosSaida;
        QPushButton* btnDadosSaida;
        QTextEdit* txtDadosSaida;
        QLabel* lblOpts;
        QRadioButton* radOpt0;
        QRadioButton* radOpt1;
        QRadioButton* radOpt2;
        QRadioButton* radOpt3;
        QRadioButton* radOpt4;
        QButtonGroup* bgrpOpts;
        automato* aut;
        int opcao;
        threadexecutaautomato* thexaut;
        bool querFechar;
        template <typename T> T maior (T, T);
        template <typename T> T maior (T, T, T);
        template <typename T> T maior (T, T, T, T);
        template <typename T> T maior (T, T, T, T, T);
    public:
        execautomatos (QWidget*, Qt::WFlags, automato&);
    private slots:
        void btnVoltar_clicked (bool);
        void btnAvancar_clicked (bool);
        void btnDadosEntrada_clicked (bool);
        void btnDadosSaida_clicked (bool);
        void threadexecutaautomato_finished ();
        void radOpt0_toggled (bool);
        void radOpt1_toggled (bool);
        void radOpt2_toggled (bool);
        void radOpt3_toggled (bool);
        void radOpt4_toggled (bool);
};

class threadexecutaautomato : public QThread {
    Q_OBJECT;
    private:
        void run ();
        bool continuar;
        static int contagem;
        automato* aut;
        int* opcao;
    public:
        void paraProcessamento ();
        QString txtSaida;
        QByteArray txtEntrada;
        threadexecutaautomato (QObject*, automato*, int&);
    signals:
        void setProgressValue (int);
};

template <typename T> T execautomatos::maior (T el1, T el2) {
    if (el1 > el2) {
        return (el1);
    } else {
        return (el2);
    }
}

template <typename T> T execautomatos::maior (T el1, T el2, T el3) {
    T el = this->maior (el1, el2);
    if (el > el3) {
        return (el);
    } else {
        return (el3);
    }
}

template <typename T> T execautomatos::maior (T el1, T el2, T el3, T el4) {
    return (this->maior (this->maior(el1, el2), this->maior (el3, el4)));
}

template <typename T> T execautomatos::maior (T el1, T el2, T el3, T el4, T el5) {
    return (this->maior (this->maior(el1, el2), this->maior (el3, el4, el5)));
}

#endif // __EXECAUTOMATOS_H
