#include "automato.h"

automato::automato () {
     this->m = new QMutex (QMutex::Recursive);
}

automato::~automato () {
    delete (this->m);
}

/* Verificar se a palavra passada eh reconhecida pelo automato
Eh essa parte (relativamente minuscula em relacao ao programa como um todo) que o Palazzo (ou Pallazo?) quer...

Nao sei se existe um nome oficial para esse algoritmo que descreverei, mas estava sonhando com ele ha alguns minutos atras:
1. No grafo, cada vertice representa uma cidade e cada arco representa uma estrada com uma praca da Ecosul.
2. Existe um Santana quadrado com uma palavra no porta-malas. Do porta-malas, so pode sair um simbolo por vez e na ordem em que foi colocado.
3. Para ir de uma cidade a outra, a Ecosul exige um simbolo como pagamento. Algumas vezes, a Ecosul faz promocao e permite a passagem do Santana sem
precisar tirar algum simbolo do porta-malas.
4. O Santana tem a capacidade de duplicacao
5. Se o Santana passar 2x pela mesma cidade, contendo a mesma palavra no porta-malas, entao ha um loop de palavras vazias. MATAR!
6. Os moradores das cidades finais sao hospitaleiros apenas com o Santana que vier de porta-malas vazio. Caso houver itens no porta-malas, fugir da cidade. Caso contrario, a palavra foi reconhecida.
7. Os moradores das cidades nao-finais nao gostam de ninguem. Se houver itens no porta-malas e algum caminho de fuga, ir embora. Senao, MATAR.
8. Se nao houver mais Santanas nas estradas, a palavra nao foi reconhecida pelo automato.

E uma coisa meio louca, mas eu vou implementar, porque pode dar certo!!! :)         */

/* Atualizado: DEU CERTO! HAHAHAHAHAHAHAHAHAHA!
 Como eh so essa parte que o Palazzo quer e ela ja esta bem comentada, ja cumpri a exigencia... 
 */

// palavra = a palavra que tem no porta-malas
// cidade = cidade onde o carro esta agora
// cidades = vetor que armazena o percurso do Santana
// porta_malas = vetor que armazena os simbolos que haviam no porta-malas em cada cidade que o Santana visitou

bool automato::reconhece (QString palavra, QString cidade, QLinkedList<QString>& porta_malas, QLinkedList<QString>& cidades) {
    // Verificar se a cidade eh hospitaleira
    if (palavra == "" && this->isEstadoFinal (cidade)) {
        return (true);
    }
    // Verificar se o carro esta chegando 2x na mesma cidade.
    QLinkedList<QString>::iterator i_ci, i_pm;
    QVector<QString>::iterator i;
    QVector<QString> dests;
    QChar simb;
    QString paux;
    for (i_ci = cidades.begin(), i_pm = porta_malas.begin(); i_ci != cidades.end(); i_ci++, i_pm++) {
        if ((*i_pm) == palavra) {
            if ((*i_ci) == cidade) {
                return (false);
            }
        } else {
            break;
        }
    }
    // Verificar se e possivel viajar de graca em alguma estrada
    dests = this->getFuncTrans (cidade, QChar(0));
    porta_malas.prepend (palavra);
    cidades.prepend (cidade);
    for (i = dests.begin(); i != dests.end(); i++) {
        if (this->reconhece (palavra, (*i), porta_malas, cidades)) {
            return (true);
        }
    }
    // Ver se ha algo no porta-malas
    if (palavra != "") {
        // Verificar se e possivel viajar com o que tem no porta-malas
        simb = palavra[0];
        paux = palavra.right (palavra.length() - 1);
        dests = this->getFuncTrans (cidade, simb);
        for (i = dests.begin(); i != dests.end(); i++) {
            if (this->reconhece (paux, (*i), porta_malas, cidades)) {
                return (true);
            }
        }
    }
    porta_malas.removeFirst();
    cidades.removeFirst();
    return (false);
}

bool automato::reconhece (QString palavra) {
    bool res;
    QString inicial;
    QLinkedList<QString> l1, l2;
    if (this->validaAutomato ()) {
        this->m->lock();
        inicial = this->getEstadoInicial();
        res = this->reconhece (palavra, inicial, l1, l2);
        this->m->unlock();
        return (res);
    } else {
        return (false);
    }
}

// Adiciona um simbolo ao automato. Retorna 'true' se tudo deu certo.
bool automato::addSimb (QChar simbolo) {
    this->m->lock();
    if (this->simbolos.contains (simbolo)) {
        qDebug ("[automato::addSimb] Simbolo '%c' ja existe.", simbolo.toAscii());
        this->m->unlock();
        return (false);
    } else {
        this->simbolos.append (simbolo);
        this->revalidar = true;
        this->m->unlock();
        return (true);
    }
}

// Obtem a lista de simbolos ja adicionados
QVector<QChar> automato::getSimb (void) {
    this->m->lock();
    QVector<QChar> ret = QVector<QChar> (this->simbolos);
    this->m->unlock();
    return (ret);
}

// Muda um simbolo (por exemplo, o simbolo que era 'a' passa a ser 'b'). Retorna 'true' se tudo deu certo.
// Parametros (em ordem): nome antigo, nome novo
bool automato::mvSimb (QChar oldSimb, QChar newSimb) {
    this->m->lock();
    int pesq = this->simbolos.indexOf (newSimb);
    if (pesq >= 0) {
        qDebug ("[automato::mvSimb] Simbolo '%c' ja existe.", newSimb.toAscii());
        this->m->unlock();
        return (false);
    }
    pesq = this->simbolos.indexOf (oldSimb);
    if (pesq < 0) {
        qDebug ("[automato::mvSimb] Simbolo '%c' nao existe.", oldSimb.toAscii());
        this->m->unlock();
        return (false);
    }
    this->simbolos.replace (pesq, newSimb);
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        QVector<arco>::iterator j = i->arcos.begin();
        while (j != i->arcos.end()) {
            if (j->simb == oldSimb) {
                j->simb = newSimb;
            }
            j++;
        }
        i++;
    }
    this->revalidar = true;
    this->m->unlock();
    return (true);
}

// Remove um simbolo do automato. Retorna 'true' se tudo deu certo.
bool automato::rmSimb (QChar oldSimb) {
    this->m->lock();
    int pesq = this->simbolos.indexOf (oldSimb);
    if (pesq < 0) {
        qDebug ("[automato::rmSimb] Simbolo '%c' nao existe.", oldSimb.toAscii());
        this->m->unlock();
        return (false);
    }
    this->simbolos.remove (pesq);
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        QVector<arco>::iterator j = i->arcos.begin();
        while (j != i->arcos.end()) {
            if (j->simb == oldSimb) {
                j = i->arcos.erase(j);
            } else {
                j++;
            }
        }
        i++;
    }
    this->revalidar = true;
    this->m->unlock();
    return (true);
}

// Adiciona um estado ao automato. Retorna 'true' se tudo deu certo.
bool automato::addEstado (QString estado) {
    this->m->lock();
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->nome == estado) {
            qDebug ("[automato::addEstado] Estado '%s' ja existe.", estado.toAscii().constData());
            this->m->unlock();
            return (false);
        }
        i++;
    }
    vertice nv;
    nv.nome = estado;
    this->vertices.append (nv);
    this->ajustaVertice (estado);
    this->revalidar = true;
    this->m->unlock();
    return (true);
}

// Obtem a lista de estados ja adicionados
QVector<QString> automato::getEstado (void) {
    this->m->lock();
    QVector<QString> ret;
    ret.reserve (this->vertices.count ());
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        ret.append (i->nome);
        i++;
    }
    this->m->unlock();
    return (ret);
}

// Muda o nome de um estado. Retorna 'true' se tudo deu certo.
// Parametros (em ordem): nome antigo, nome novo
bool automato::mvEstado (QString oldEstado, QString newEstado) {
    this->m->lock();
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->nome == newEstado) {
            qDebug ("[automato::mvEstado] Estado '%s' ja existe.", newEstado.toAscii().constData());
            this->m->unlock();
            return (false);
        }
        i++;
    }
    i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->nome == oldEstado) {
            i->nome = newEstado;
            QVector<vertice>::iterator i2 = this->vertices.begin();
            while (i2 != this->vertices.end()) {
                QVector<arco>::iterator i3 = i2->arcos.begin();
                while (i3 != i2->arcos.end()) {
                    if (i3->destino == oldEstado) {
                        i3->destino = newEstado;
                    }
                    i3++;
                }
                i2++;
            }
            this->revalidar = true;
            this->m->unlock();
            return (true);
        }
        i++;
    }
    qDebug ("[automato::mvEstado] Estado '%s' nao existe.", oldEstado.toAscii().constData());
    this->m->unlock();
    return (false);
}

// Remove um estado. Retorna 'true' se tudo deu certo.
bool automato::rmEstado (QString oldEstado) {
    this->m->lock();
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->nome == oldEstado) {
            QVector<vertice>::iterator j = this->vertices.begin();
            while (j != this->vertices.end()) {
                QVector<arco>::iterator k = j->arcos.begin();
                while (k != j->arcos.end()) {
                    if (k->destino == oldEstado) {
                        k = j->arcos.erase(k);
                    } else {
                        k++;
                    }
                }
                j++;
            }
            this->vertices.erase (i);
            this->revalidar = true;
            this->m->unlock();
            return (true);
        }
        i++;
    }
    qDebug ("[automato::rmEstado] Estado '%s' nao existe.", oldEstado.toAscii().constData());
    this->m->unlock();
    return (false);
}

// Marca um estado (previamente adicionado) como inicial ou nao. Retorna 'true' se tudo deu certo.
bool automato::setEstadoInicial (QString oldEstado, bool flag) {
    this->m->lock();
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->nome == oldEstado) {
            i->inicial = flag;
            if (flag) {
                QVector<vertice>::iterator j = this->vertices.begin();
                while (j != this->vertices.end()) {
                    if (j != i) {
                        j->inicial = false;
                    }
                    j++;
                }
                this->setPosEstado (i->nome, i->pos);
            }
            this->revalidar = true;
            this->m->unlock();
            return (true);
        }
        i++;
    }
    qDebug ("[automato::setEstadoInicial] Estado '%s' nao existe.", oldEstado.toAscii().constData());
    this->m->unlock();
    return (false);
}

// Pergunta se o estado especificado foi marcado como inicial.
bool automato::isEstadoInicial (QString oldEstado) {
    this->m->lock();
    bool ret;
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->nome == oldEstado) {
            ret = i->inicial;
            this->m->unlock();
            return (ret);
        }
        i++;
    }
    qDebug ("[automato::isEstadoInicial] Estado '%s' nao existe.", oldEstado.toAscii().constData());
    this->m->unlock();
    return (false);
}

// Obtem o estado que foi marcado como inicial, se houver
QString automato::getEstadoInicial (void) {
    QString res;
    this->m->lock();
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->inicial) {
            res = QString(i->nome);
            this->m->unlock();
            return (res);
        }
        i++;
    }
    this->m->unlock();
    return (QString());
}

// Marca um estado (previamente adicionado) como final ou nao. Retorna 'true' se tudo deu certo.
bool automato::setEstadoFinal (QString oldEstado, bool flag) {
    this->m->lock();
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->nome == oldEstado) {
            i->final = flag;
            this->revalidar = true;
            this->m->unlock();
            return (true);
        }
        i++;
    }
    qDebug ("[automato::setEstadoFinal] Estado '%s' nao existe.", oldEstado.toAscii().constData());
    this->m->unlock();
    return (false);
}

// Pergunta se o estado especificado foi marcado como final
bool automato::isEstadoFinal (QString oldEstado) {
    bool ret;
    this->m->lock();
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->nome == oldEstado) {
            ret = i->final;
            this->m->unlock();
            return (ret);
        }
        i++;
    }
    qDebug ("[automato::isEstadoFinal] Estado '%s' nao existe.", oldEstado.toAscii().constData());
    this->m->unlock();
    return (false);
}

// Obtem a lista dos estados que foram marcados como final
QVector<QString> automato::getEstadoFinal (void) {
    this->m->lock();
    QVector<QString> ret;
    ret.reserve (this->vertices.count ());
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->final) {
            ret.append (i->nome);
        }
        i++;
    }
    this->m->unlock();
    return (ret);
}

// Obtem as coordenadas (x,y) do estado (faz sentido ao desenhar o automato na tela)
QPoint automato::getPosEstado (QString oldEstado) {
    this->m->lock();
    QPoint p;
    QVector<vertice>::iterator i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->nome == oldEstado) {
            p = QPoint (i->pos);
            this->m->unlock();
            return (p);
        }
        i++;
    }
    qDebug ("[automato::getPosEstado] Estado '%s' nao existe.", oldEstado.toAscii().constData());
    this->m->unlock();
    return (QPoint());
}

// Define as coordenadas (x,y) do estado (faz sentido ao desenhar o automato na tela)
bool automato::setPosEstado (QString oldEstado, QPoint pos) {
    this->m->lock();
    QVector<vertice>::iterator i;
    i = this->vertices.begin();
    while (i != this->vertices.end()) {
        if (i->nome == oldEstado) {
            i->pos = pos;
            this->ajustaVertice (oldEstado);
            this->m->unlock();
            return (true);
        }
        i++;
    }
    qDebug ("[automato::setPosEstado] Estado '%s' nao existe.", oldEstado.toAscii().constData());
    this->m->unlock();
    return (false);
}

// Adiciona um argumento para a funcao que recebe um par estado-simbolo e fornece um conjunto de estados. Retorna 'true' se tudo deu certo
// Parametros (em ordem): estado atual, simbolo, proximo estado
bool automato::addFuncTrans (QString oldEstado, QChar simb, QString newEstado) {
    this->m->lock();
    if (simb.isNull() || this->simbolos.contains (simb)) {
        for (QVector<vertice>::iterator i = this->vertices.begin(); i != this->vertices.end(); i++) {
            if (i->nome == oldEstado) {
                for (QVector<arco>::iterator j = i->arcos.begin(); j != i->arcos.end(); j++) {
                    if (j->simb == simb && j->destino == newEstado) {
                        qDebug ("[automato::addFuncTrans] Entrada ( ('%s','%c') -> '%s' ) ja existe.", oldEstado.toAscii().constData(), simb.toAscii(), newEstado.toAscii().constData());
                        this->m->unlock();
                        return (false);
                    }
                }
                arco nn;
                nn.simb = simb;
                nn.destino = newEstado;
                i->arcos.append (nn);
                this->revalidar = true;
                this->m->unlock();
                return (true);
            }
        }
    }
    qDebug ("[automato::addFuncTrans] Estado '%s' ou '%s' ou simbolo '%c' nao existem.", oldEstado.toAscii().constData(), newEstado.toAscii().constData(), simb.toAscii());
    this->m->unlock();
    return (false);
}

// Retorna o conjunto de proximos estados para o estado atual e simbolo de entrada dados
// Parametros (em ordem): estado atual, simbolo
QVector<QString> automato::getFuncTrans (QString oldEstado, QChar simb) {
    this->m->lock();
    QVector<QString> ret;
    for (QVector<vertice>::iterator i = this->vertices.begin(); i != this->vertices.end(); i++) {
        if (i->nome == oldEstado) {
            for (QVector<arco>::iterator j = i->arcos.begin(); j != i->arcos.end(); j++) {
                if (j->simb == simb) {
                    ret.append (j->destino);
                }
            }
        }
    }
    this->m->unlock();
    return (ret);
}

// Retorna o conjunto de simbolos para o estado atual e proximo estado dados
// Parametros (em ordem): estado atual, proximo estado
QVector<QChar> automato::getFuncTrans (QString oldEstado, QString newEstado) {
    this->m->lock();
    QVector<QChar> ret;
    for (QVector<vertice>::iterator i = this->vertices.begin(); i != this->vertices.end(); i++) {
        if (i->nome == oldEstado) {
            for (QVector<arco>::iterator j = i->arcos.begin(); j != i->arcos.end(); j++) {
                if (j->destino == newEstado) {
                    ret.append (j->simb);
                }
            }
        }
    }
    this->m->unlock();
    return (ret);
}

// Remove um trio estado atual - simbolo - proximo estado. Retorna 'true' se tudo deu certo.
// Parametros (em ordem): estado atual, simbolo, proximo estado
bool automato::rmFuncTrans (QString oldEstado, QChar simb, QString newEstado) {
    this->m->lock();
    for (QVector<vertice>::iterator i = this->vertices.begin(); i != this->vertices.end(); i++) {
        if (i->nome == oldEstado) {
            for (QVector<arco>::iterator j = i->arcos.begin(); j != i->arcos.end(); j++) {
                if (j->simb == simb) {
                    if (j->destino == newEstado) {
                        i->arcos.erase (j);
                        this->revalidar = true;
                        this->m->unlock();
                        return (true);
                    }
                }
            }
        }
    }
    qDebug ("[automato::addFuncTrans] Entrada ( ('%s','%c') -> '%s' ) nao existe.", oldEstado.toAscii().constData(), simb.toAscii(), newEstado.toAscii().constData());
    this->m->unlock();
    return (false);
}

// Remove todos os trios estado atual - simbolo - proximo estado. Retorna 'true' se tudo deu certo.
// Parametros (em ordem): estado atual, proximo estado
bool automato::rmFuncTrans (QString oldEstado, QString newEstado) {
    this->m->lock();
    for (QVector<vertice>::iterator i = this->vertices.begin(); i != this->vertices.end(); i++) {
        if (i->nome == oldEstado) {
            for (QVector<arco>::iterator j = i->arcos.begin(); j != i->arcos.end(); j++) {
                if (j->destino == newEstado) {
                    i->arcos.erase (j);
                    this->revalidar = true;
                    this->m->unlock();
                    return (this->rmFuncTrans (oldEstado, newEstado));
                }
            }
        }
    }
    this->m->unlock();
    return (true);
}

// Le um automato de um arquivo
bool automato::readFromFile (QString file) {
    this->m->lock();
    QFile fd (file);
    int i, max, x, y, j, nnos;
    QString nome, s, s2, dest, simbs;
    QChar simb;
    bool inicial, final;
    QVector<vertice> vertices_temp;
    QVector<QChar> simbolos_temp;
    if (! fd.open (QIODevice::ReadOnly)) {
        qDebug ("[automato::readFromFile] Erro lendo '%s': %s", file.toAscii().constData(), fd.errorString().toAscii().constData());
        this->m->unlock();
        return (false);
    }
    fd.close ();
    QSettings entrada (file, QSettings::IniFormat);
    if (entrada.status () != QSettings::NoError) {
        qDebug ("[automato::readFromFile] Erro de leitura causado por 'QSettings'. Nao e possivel obter informacoes.");
        this->m->unlock();
        return (false);
    }
    vertices_temp = this->vertices;
    simbolos_temp = this->simbolos;
    this->vertices.clear ();
    this->simbolos.clear ();
    entrada.sync ();
    simbs = entrada.value ("main/alfabeto", QString()).toString();
    for (i = simbs.count() - 1; i >= 0; i--) {
        simb = simbs[i];
        if (! this->addSimb (simb)) {
            qDebug ("[automato::readFromFile] Erro de leitura: parametros invalidos no arquivo de entrada.");
            this->vertices = vertices_temp;
            this->simbolos = simbolos_temp;
            this->m->unlock();
            return (false);
        }
    }
    max = entrada.value("main/countEstados", 0).toInt();
    for (i = 0; i < max; i++) {
        s = QString ("estado_") + QString::number(i) + "/";
        nome = entrada.value (s + "nome", "").toString ();
        inicial = entrada.value (s + "inicial", false).toBool ();
        final = entrada.value (s + "final", false).toBool ();
        x = entrada.value (s + "x", -1).toInt ();
        y = entrada.value (s + "y", -1).toInt ();
        nnos = entrada.value (s + "countNos", -1).toInt ();
        if (nome != "" && x >= 0 && y >= 0 && nnos >= 0) {
            if (! (this->addEstado (nome) && this->setEstadoInicial (nome, inicial) && this->setEstadoFinal (nome, final) && this->setPosEstado (nome, QPoint (x, y)))) {
                qDebug ("[automato::readFromFile] Erro de leitura: parametros nao-validos no arquivo.");
            this->vertices = vertices_temp;
            this->simbolos = simbolos_temp;
                this->m->unlock();
                return (false);
            }
        } else {
            qDebug ("[automato::readFromFile] Erro de leitura: parametros nao-validos no arquivo de entrada.");
            this->vertices = vertices_temp;
            this->simbolos = simbolos_temp;
            this->m->unlock();
            return (false);
        }
    }
    for (i = 0; i < max; i++) {
        s = QString ("estado_") + QString::number(i) + "/";
        nome = entrada.value (s + "nome", "").toString ();
        if (nome != "") {
            nnos = entrada.value (s + "countNos", -1).toInt ();
            for (j = 0; j < nnos; j++) {
                s2 = s + "arco_" + QString::number (j);
                simb = entrada.value (s2 + "_simbolo", QString()).toString ()[0];
                dest = entrada.value (s2 + "_destino", QString()).toString ();
                if (dest != "" && (! simb.isNull())) {
                    if (! (this->addFuncTrans (nome, (simb == '3') ? 0 : simb, dest))) {
                        qDebug ("[automato::readFromFile] Erro de leitura: parametros invalidos no arquivo do automato.");
                        this->vertices = vertices_temp;
                        this->simbolos = simbolos_temp;
                        this->m->unlock();
                        return (false);
                    }
                } else {
                    qDebug ("[automato::readFromFile] Erro de leitura: parametros nao-validos no arquivo do automato.");
                    this->vertices = vertices_temp;
                    this->simbolos = simbolos_temp;
                    this->m->unlock();
                    return (false);
                }
            }
        } else {
            qDebug ("[automato::readFromFile] Erro de leitura: parametros nao-validos no arquivo do automato finito.");
            this->vertices = vertices_temp;
            this->simbolos = simbolos_temp;
            this->m->unlock();
            return (false);
        }
    }
    this->revalidar = true;
    this->m->unlock();
    return (true);
}

// Grava um automato em um arquivo
bool automato::writeToFile (QString file) {
    this->m->lock();
    QString s, s2, simbs;
    vertice* v;
    QFile fd (file);
    int i, j;
    // A ideia aqui e esvaziar o arquivo antes de gravar.
    if (! fd.open (QIODevice::Truncate | QIODevice::WriteOnly)) {
        qDebug ("[automato::writeToFile] Erro gravando '%s': %s", file.toAscii().constData(), fd.errorString().toAscii().constData());
        this->m->unlock();
        return (false);
    }
    fd.close ();
    QSettings saida (file, QSettings::IniFormat);
    i = this->vertices.count();
    saida.setValue ("main/countEstados", i);
    for (i--; i >= 0; i--) {
        v = &(this->vertices[i]);
        s = QString ("estado_") + QString::number(i) + "/";
        saida.setValue (s + "nome", v->nome);
        saida.setValue (s + "inicial", v->inicial);
        saida.setValue (s + "final", v->final);
        saida.setValue (s + "x", v->pos.x());
        saida.setValue (s + "y", v->pos.y());
        j = v->arcos.count();
        saida.setValue (s + "countNos", j);
        for (j--; j >= 0; j--) {
            s2 = s + "arco_" + QString::number(j);
            saida.setValue (s2 + "_simbolo", QString ((v->arcos[j].simb.isNull()) ? '3' : v->arcos[j].simb));
            saida.setValue (s2 + "_destino", v->arcos[j].destino);
        }
    }
    for (i = this->simbolos.count() - 1; i >= 0; i--) {
        simbs += this->simbolos[i];
    }
    saida.setValue ("main/alfabeto", simbs);
    saida.sync ();
    if (saida.status () != QSettings::NoError) {
        qDebug ("[automato::writeToFile] Erro de gravacao causado por 'QSettings'. Nao e possivel obter informacoes.");
        this->m->unlock();
        return (false);
    }
    this->m->unlock();
    return (true);
}

bool automato::validaAutomato (void) {
    // #1. ver se ha exatamente 1 estado inicial
    // #2. ver se ha ao menos 1 estado final
    // #3. ver se todas as coordenadas dos estados sao >= 0
    // #4. ver se nao ha estado com nomes repetidos
    // #5. ver se nao ha simbolos repetidos na lista de simbolos
    // #6. ver se os nos de cada vertice apontam para alguma coisa que exista
    // #7. ver se os nos de cada vertice tem simbolos que existem
    // #8. ver se todos os simbolos sao alfabeticos
    // Corrigindo: automatos que nao possuem estados finais sao validos. Teste #2 cai fora
    this->m->lock();
    int num_inicial = 0;
    int c, i, j, k;
    if (this->revalidar) {
        this->estavalido = true;
        for (c = this->simbolos.count() - 1; c >= 0 && this->estavalido; c--) {
            if (this->simbolos.count(this->simbolos[c]) > 1) {
                qDebug ("[automato::validaAutomato] simbolo repetido no alfabeto.");
                this->estavalido = false;
            } else if (! (this->simbolos[c].isLetter())) {
                qDebug ("[automato::validaAutomato] simbolo nao-alfabetico no alfabeto.");
                this->estavalido = false;
            }
        }
        for (i = this->vertices.count() - 1; i >= 0 && this->estavalido; i--) {
            num_inicial += (this->vertices[i].inicial) ? 1 : 0;
            if (this->vertices[i].pos.x() < 0 || this->vertices[i].pos.y() < 0) {
                qDebug ("[automato::validaAutomato] coordenadas negativas encontradas.");
                this->estavalido = false;
            } else {
                for (j = i - 1; j >= 0 && this->estavalido; j--) {
                    if (this->vertices[i].nome == this->vertices[j].nome) {
                        qDebug ("[automato::validaAutomato] estado repetido encontrado.");
                        this->estavalido = false;
                    }
                }
                for (j = this->vertices[i].arcos.count() - 1; j >= 0 && this->estavalido; j--) {
                    for (k = this->vertices.count() - 1; k >= 0; k--) {
                        if (this->vertices[k].nome == this->vertices[i].arcos[j].destino) {
                            break;
                        }
                    }
                    if (k < 0) {
                        qDebug ("[automato::validaAutomato] encontrou um arco que aponta para algum endereco desconhecido na memoria.");
                        this->estavalido = false;
                    } else if (! (this->simbolos.contains (this->vertices[i].arcos[j].simb) || this->vertices[i].arcos[j].simb.isNull())) {
                        qDebug ("[automato::validaAutomato] encontrou um arco identificado por um simbolo nao presente no alfabeto.");
                        this->estavalido = false;
                    }
                }
            }
        }
        if (num_inicial != 1) {
            qDebug ("[automato::validaAutomato] numero de estados iniciais deveria ser igual a 1, mas e igual a: %d.", num_inicial);
            this->estavalido = false;
        }
        this->revalidar = false;
    }
    this->m->unlock();
    return (this->estavalido);
}

void automato::ajustaVertice (QString vert) {
    int minx, miny;
    this->m->lock();
    QVector<vertice>::iterator i, j;
    QVector<arco>::iterator m;
    // Verificar se alguem esta perto demais da margem esquerda ou da margem superior
    for (i = this->vertices.begin(); i != this->vertices.end(); i++) {
        if (i->nome == vert) {
            break;
        }
    }
    Q_ASSERT (i != this->vertices.end());
    minx = ESPACO + ESTADO_DIAMETRO / 2;
    miny = minx;
    if (i->final) {
        minx += ESTADO_DIAMETRO_OFFSET_FINAL / 2;
        miny += ESTADO_DIAMETRO_OFFSET_FINAL / 2;
    }
    if (i->inicial) {
        miny += COMPRIMENTO_SETA * 2 + DISTANCIA_ESTADO;
    }
    for (m = i->arcos.begin(); m != i->arcos.end(); m++) {
        if (m->destino == i->nome) {
            minx += COMPRIMENTO_SETA + LARG_MEIO_CIRC + DISTANCIA_ESTADO;
            break;
        }
    }
    if (i->pos.x() < minx) {
        i->pos.setX (minx);
    }
    if (i->pos.y() < miny) {
        i->pos.setY (miny);
    }
    // Se 2 estados tiverem na mesma posicao, meus algoritmos falham...
    j = this->vertices.begin();
    while (j != this->vertices.end()) {
        if (j != i) {
            if (j->pos == i->pos) {
                i->pos.setX (i->pos.x() + 1);
                j = this->vertices.begin();
                continue;
            }
        }
        j++;
    }
    /*

    Bem que eu queria achar algum jeito de evitar que um estado fique em cima do outro...
    Mas acabei trancado em uma pergunta sem resposta.
    Como eu estou quase rodado em Logica Proposicional, vou deixar isso para outra hora.
    FUI!

    */
    this->m->unlock();
}

void automato::clear (void) {
    this->m->lock();
    this->vertices.clear();
    this->simbolos.clear();
    this->m->unlock();
}
