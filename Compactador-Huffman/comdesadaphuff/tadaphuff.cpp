#include "common.h"
#define NYT -1
#define MAXID 51

TAdapHuff::TAdapHuff() {
    this->Reinicia();
}

void TAdapHuff::Reinicia (void) {
    this->modo = 0;
    this->arv.setId (MAXID);
    this->arv.setCharacter (NYT);
    this->arv.setWeight (0);
    this->arv.killSons();
    this->navega = this->arv.self();;
}

void TAdapHuff::DeixaApenasValidos (std::string &str) {
    int i, l;
    l = str.length ();
    for (i=0; i<l; i++) {
        if (str[i] != '0' && str[i] != '1') {
            str = str.substr (0, i) + str.substr (i+1, l-i-1);
            l--;
            i--;
        }
    }
}

char TAdapHuff::DecodificaCaractere (std::string &str) {
    std::string inicio;
    int l, i;
    char r;
    TCell *nodo;
    if (this->modo > 0) return (0);
    this->modo = -1;
    assert (this->navega != NULL);
    this->DeixaApenasValidos (str);
    l = str.length();
    r = 0;
    i = 0;
    while (this->navega->character() == 0) {
        if (i == l) {
            str = "";
            return (0);
        }
        this->navega = this->navega->son(str[i]);
        i++;
        assert (this->navega != NULL);
    }
    l -= i;
    str = str.substr (i, l);
    if (this->navega->character() == NYT) {
        if (l < 8) return (0);
        inicio = str.substr (0, 8);
        l -= 8;
        str = str.substr (8, l);
        for (i=0; i<8; i++)
            if (inicio[i] == '1')
                r |= 128 >> i;
        this->navega->makeSons ();
        this->navega->son0()->setId (this->navega->id() - 2);
        this->navega->son0()->setWeight (0);
        this->navega->son0()->setCharacter (NYT);
        this->navega->son1()->setId (this->navega->id() - 1);
        this->navega->son1()->setWeight (1);
        this->navega->son1()->setCharacter (r);
        this->navega->setCharacter (0);
    } else {
        r = this->navega->character ();
    }
    while (this->navega->parent() != NULL) {
        this->navega->setWeight (this->navega->weight() + 1);
        for (i = MAXID; this->navega->id() != i; i--) {
            nodo = this->arv.findSonById (i);
            assert (nodo != NULL);
            if (nodo->weight() < this->navega->weight() && this->navega->parent() != nodo) {
                this->navega->swap (nodo);
                i = nodo->id();
                nodo->setId (this->navega->id());
                this->navega->setId (i);
                this->navega = nodo;
                break;
            }
        }
        this->navega = this->navega->parent();
    }
    this->navega->setWeight (this->navega->weight() + 1);
    return (r);
}

std::string TAdapHuff::CodificaCaractere (char ch) {
    TCell *nodo, *nodo2;
    std::string retorno;
    int i;
    if (this->modo < 0 || ch == 0 || ch == NYT) return ("");
    this->modo = 1;
    nodo = this->arv.findSonByCharacter (ch);
    if (nodo == NULL) {
        nodo = this->arv.findSonByCharacter (NYT);
        assert (nodo != NULL);
        retorno = nodo->pathFromRootToMe ();
        for (i=7; i>=0; i--)
            retorno += (ch & (1 << i)) ? "1" : "0";
        nodo->makeSons ();
        nodo->son0()->setId (nodo->id() - 2);
        nodo->son0()->setWeight (0);
        nodo->son0()->setCharacter (NYT);
        nodo->son1()->setId (nodo->id() - 1);
        nodo->son1()->setWeight (1);
        nodo->son1()->setCharacter (ch);
        nodo->setCharacter (0);
    } else
        retorno = nodo->pathFromRootToMe ();
    while (nodo->parent() != NULL) {
        nodo->setWeight (nodo->weight() + 1);
        for (i = MAXID; nodo->id() != i; i--) {
            nodo2 = this->arv.findSonById (i);
            assert (nodo2 != NULL);
            if (nodo2->weight() < nodo->weight() && nodo->parent() != nodo2) {
                nodo2->swap (nodo);
                i = nodo2->id();
                nodo2->setId (nodo->id());
                nodo->setId (i);
                nodo = nodo2;
                break;
            }
        }
        nodo = nodo->parent();
    }
    nodo->setWeight (nodo->weight() + 1);
    return (retorno);
}
