#include "editautomato.h"

editautomato::editautomato (QWidget* parent, Qt::WFlags f) : QWidget (parent, f) {
    this->estado_origem_selecionado = "";
    this->estado_destino_selecionado = "";
    this->movendoAlguem = false;
    this->criandoLink = false;
    this->x = 1;
    this->y = 1;
    this->w = 0;
    this->h = 0;
    this->setMouseTracking (true);
}

void editautomato::desenhaSeta (QPainter& pintor, QPoint pontaSeta, QPoint referencia) {
    // Na verdade, so desenha o triangulo... hehehehehe
    segmreta r1 (referencia, pontaSeta);
    QPoint triangulo[3], base;
    if (r1.isValid()) {
        triangulo[0] = r1.bottomRight();
        base = r1.deslocamento (-COMPRIMENTO_SETA);
        triangulo[1] = QPoint ((int) rint(base.x() - LARGURA_SETA * r1.sinang() / 2), (int) rint(base.y() + LARGURA_SETA * r1.cosang() / 2));
        triangulo[2] = QPoint ((int) rint(base.x() + LARGURA_SETA * r1.sinang() / 2), (int) rint(base.y() - LARGURA_SETA * r1.cosang() / 2));
        pintor.drawPolygon (triangulo, 3);
    }
}

void editautomato::paintEvent (QPaintEvent *ev) {
    int i, larg, ii, cont, maxw, maxh, actw, acth, raio1, raio2, raio3;
    QColor cor;
    QPainter pintor (this);
    QVector<QString> estados, setas;
    QVector<QChar> simbs;
    QString v, vv, txt;
    QFont f;
    QPoint pos, pos2, pos3, paux1, paux2;
    segmreta aux, aux2;
    bool fin, inic, fin2, inic2, prec_desloc;
    QWidget::paintEvent (ev);
    ev->accept();
    pintor.fillRect (this->rect(), COR_FUNDO);
    estados = this->aut.getEstado ();
    f.setPixelSize (TAM_FONTE);
    pintor.setFont (f);
    maxw = 0;
    maxh = 0;
    for (i = estados.count() - 1; i >= 0; i--) {
        raio1 = ESTADO_DIAMETRO / 2;
        raio2 = raio1 + ESTADO_DIAMETRO_OFFSET_FINAL / 2;
        raio3 = raio2 - ESTADO_DIAMETRO_OFFSET_FINAL;
        pintor.setBrush (Qt::NoBrush);
        v = estados[i];
        pos = this->aut.getPosEstado (v);
        inic = this->aut.isEstadoInicial(v);
        fin = this->aut.isEstadoFinal(v);
        if (inic && fin) {
            cor = COR_ESTADO_INICIAL_FINAL;
        } else if (inic) {
            cor = COR_ESTADO_INICIAL;
        } else if (fin) {
            cor = COR_ESTADO_FINAL;
        } else {
            cor = COR_NORMAL;
        }
        if (this->estado_origem_selecionado == v && (this->estado_destino_selecionado == "" || this->criandoLink)) {
            cor = COR_SELECIONADO;
        } else if (this->estado_destino_selecionado == v && this->criandoLink) {
            cor = COR_SELECIONADO;
        }
        larg = QFontMetrics(f).width (v);
        pintor.setPen (QPen (QBrush (cor), 2));
        // Desenhar o circulo
        if (fin) {
            pintor.drawEllipse (pos.x() - raio2, pos.y() - raio2, raio2 * 2, raio2 * 2);
            pintor.drawEllipse (pos.x() - raio3, pos.y() - raio3, raio3 * 2, raio3 * 2);
            actw = pos.x() + raio2;
            acth = pos.y() + raio2;
        } else {
            pintor.drawEllipse (pos.x() - raio1, pos.y() - raio1, raio1 * 2, raio1 * 2);
            actw = pos.x() + raio1;
            acth = pos.y() + raio1;
        }
        maxw = (actw > maxw) ? actw : maxw;
        maxh = (acth > maxh) ? acth : maxh;
        raio1 += DISTANCIA_ESTADO;
        raio2 += DISTANCIA_ESTADO;
        actw += DISTANCIA_ESTADO;
        acth += DISTANCIA_ESTADO;
        // Escrever dentro do circulo
        pintor.drawText (pos.x() - (larg >>1), pos.y() + (TAM_FONTE>>1), v);
        // Desenhar as setas. (Vamos revisar geometria analitica, neste momento)
        pintor.setBrush (COR_FUNDO);
        // Primeiro, a seta que indica aquele estado que eh o inicial...
        if (inic) {
            pintor.setPen (QPen (QBrush (cor), 2));
            if (fin) {
                pintor.drawLine (pos.x(), pos.y() - raio2, pos.x(), pos.y() - raio2 - 2 * COMPRIMENTO_SETA);
                this->desenhaSeta (pintor,
                                   QPoint (pos.x(), pos.y() - raio2),
                                   QPoint (pos.x(), pos.y() - raio2 - COMPRIMENTO_SETA));
            } else {
                pintor.drawLine (pos.x(), pos.y() - raio1, pos.x(), pos.y() - raio1 - 2 * COMPRIMENTO_SETA);
                this->desenhaSeta (pintor,
                                   QPoint (pos.x(), pos.y() - raio1),
                                   QPoint (pos.x(), pos.y() - raio1 - COMPRIMENTO_SETA));
            }
        }
        // Se um 'link' estiver sendo criado, desenhar uma seta do estado-origem ate o rato
        // "rato", que coisa brega!!!
        if (this->criandoLink && this->jaSaiuArea && this->estado_origem_selecionado == v && this->estado_destino_selecionado == "") {
            pos2 = QPoint (this->x, this->y);
            aux = segmreta (pos, pos2);
            aux2 = segmreta (aux.deslocamento ((fin) ? raio2 : raio1), pos2);
            if (aux2.isValid()) {
                pintor.drawLine (aux2.topLeft(), aux2.deslocamento (-COMPRIMENTO_SETA));
                this->desenhaSeta (pintor, aux2.bottomRight(), aux2.topLeft());
                maxw = (this->x > maxw) ? this->x : maxw;
                maxh = (this->y > maxh) ? this->y : maxh;
            }
        }
        // Agora, as setas que ligam um estado a outro
        for (ii = estados.count() - 1; ii >= 0; ii--) {
            vv = estados[ii];
            pos2 = this->aut.getPosEstado (vv);
            inic2 = this->aut.isEstadoInicial (vv);
            fin2 = this->aut.isEstadoFinal (vv);
            simbs = this->aut.getFuncTrans (v, vv);
            if (simbs.count() > 0 || (this->criandoLink && this->jaSaiuArea && this->estado_origem_selecionado == v && this->estado_destino_selecionado == vv)) {
                prec_desloc = (this->aut.getFuncTrans (vv, v).count() > 0 || (this->criandoLink && this->jaSaiuArea && this->estado_origem_selecionado == vv && this->estado_destino_selecionado == v));
                txt = "";
                for (cont = simbs.count() - 1; cont >= 0; cont--) {
                    if (simbs[cont].isNull()) {
                        txt = QString("3,") + txt;
                    } else {
                        txt = QString (simbs[cont]) + "," + txt;
                    }
                }
                txt = txt.left (txt.count() - 1);
                larg = QFontMetrics(f).width(txt);
                if (this->estado_origem_selecionado == v && this->estado_destino_selecionado == vv) {
                    cor = COR_SELECIONADO;
                } else {
                    cor = COR_NORMAL;
                }
                pintor.setPen (QPen (QBrush (cor), 2));
                if (v != vv) { // Isso porque o algoritmo para arcos com origem e extremidade no mesmo vertice eh outro.
                    // Desenhar uma linha que vai de um vertice a outro
                    aux = segmreta (pos, pos2);
                    if (prec_desloc) { // Se ha uma transicao no sentido contrario, essa seta deve ser deslocada para dar espaco para a outra.
                        paux1 = QPoint ((int) rint(aux.left() + ESTADO_DIAMETRO * aux.sinang() / 4), (int) rint(aux.top() - ESTADO_DIAMETRO * aux.cosang() / 4));
                        paux2 = QPoint ((int) rint(aux.right() + ESTADO_DIAMETRO * aux.sinang() / 4), (int) rint(aux.bottom() - ESTADO_DIAMETRO * aux.cosang() / 4));
                        aux = segmreta (paux1, paux2);
                    }
                    paux1 = aux.deslocamento ((fin) ? raio2 : raio1);
                    paux2 = aux.deslocamento (-COMPRIMENTO_SETA - ((fin2) ? raio2 : raio1));
                    if ((((fin) ? raio2 : raio1) + ((fin2) ? raio2 : raio1) + COMPRIMENTO_SETA + 4) < aux.diagoLength()) {
                        aux2 = segmreta (paux1, paux2);
                        pintor.drawLine (aux2.topLeft(), aux2.bottomRight());
                        // Vamos ao texto da linha. agora...
                        pos3 = aux2.deslocamento (aux2.diagoLength() / 2);
                        if (txt != "") {
                            pintor.drawRect (pos3.x() - (larg + 4) / 2, pos3.y() - (TAM_FONTE + 4) / 2, larg + 4, TAM_FONTE + 4);
                            pintor.drawText (pos3.x() - larg / 2, pos3.y() + TAM_FONTE / 2, txt);
                        }
                        // E agora, a ponta da seta.
                        this->desenhaSeta (pintor, aux.deslocamento (-((fin2) ? raio2 : raio1)), pos3);
                        // Pronto...
                    } else {
                        paux2 = aux.deslocamento (-((fin2) ? raio2 : raio1));
                        pintor.drawLine (paux1, paux2);
                    }
                } else {
                    // Bom... Eu calculei durante varios dias alguma maneira de fazer uma seta perfeitamente ovalada... Mas nao deu! :(
                    // GAMBIARRA!!!
                    // A ideia eh fazer meio circulo de cada lado do no e ligar as bordas com uma reta...
                    // Comecar desenhando a ponta da seta (no lado direito)
                    raio3 = (fin) ? raio2 : raio1;
                    this->desenhaSeta (pintor, QPoint (pos.x() + raio3, pos.y()), QPoint (pos.x() + raio3 + 20, pos.y()));
                    // No lado esquerdo do circulo, apenas um segmento de reta do tamanho do comprimento da seta
                    pintor.drawLine (pos.x() - raio3 - COMPRIMENTO_SETA, pos.y(), pos.x() - raio3, pos.y());
                    // Embaixo, desenhar uma linha, ligando os 2 semi-circulos (que eu ainda nao desenhei)
                    pintor.drawLine (pos.x() - raio3 - COMPRIMENTO_SETA, pos.y() + ALT_MEIO_CIRC + raio3 - raio1, pos.x() + raio3 + COMPRIMENTO_SETA, pos.y() + ALT_MEIO_CIRC + raio3 - raio1);
                    // Desenhar o semi-circulo da esquerda
                    pintor.drawArc (pos.x() - raio3 - COMPRIMENTO_SETA - LARG_MEIO_CIRC, pos.y(), 2 * LARG_MEIO_CIRC, ALT_MEIO_CIRC + raio3 - raio1 - 1, 1440, 2880);
                    // Desenhar o semi-circulo da direita
                    pintor.drawArc (pos.x() + raio3 + COMPRIMENTO_SETA - LARG_MEIO_CIRC, pos.y(), 2 * LARG_MEIO_CIRC, ALT_MEIO_CIRC + raio3 - raio1 - 1, 1440, -2880);
                    // Deixei o desenho do texto por ultimo...
                    if (txt != "") {
                        pintor.drawRect (pos.x() - (larg + 4) / 2, pos.y() + ALT_MEIO_CIRC + raio3 - raio1 - (TAM_FONTE + 4) / 2, larg + 4, TAM_FONTE + 4);
                        pintor.drawText (pos.x() - larg / 2, pos.y() + ALT_MEIO_CIRC + raio3 - raio1 + TAM_FONTE / 2, txt);
                    }
                    actw = pos.x() + raio3 + COMPRIMENTO_SETA + LARG_MEIO_CIRC;
                    acth = pos.y() + ALT_MEIO_CIRC + raio3 - raio1 + ((txt != "") ? ((TAM_FONTE + 4) / 2) : 0);
                    maxw = (actw > maxw) ? actw : maxw;
                    maxh = (acth > maxh) ? acth : maxh;
                }
            }
        }
    }
    maxw += ESPACO;
    maxh += ESPACO;
   if (this->w != maxw || this->h != maxh) {
        this->w = maxw;
        this->h = maxh;
        this->setFixedSize (this->w, this->h);
   }
}

int editautomato::procuraMaisProximo (QPoint referencia, QVector<QString> listaDeVerificacao, QString& estado_origem, QString& estado_destino) {
    int i, i2, c, dist_men, dist_atual;
    bool primeiro, fin1, fin2, prec_desloc;
    QString item, item2;
    QPoint p, p2, paux1, paux2;
    segmreta aux1, aux2;
    QVector<QChar> simbs;
    c = listaDeVerificacao.count();
    primeiro = true;
    dist_men = 0;
    for (i = 0; i < c; i++) {
        item = listaDeVerificacao[i];
        p = this->aut.getPosEstado (item);
        fin1 = this->aut.isEstadoFinal (item);
        aux1 = segmreta(p, referencia);
        dist_atual = aux1.diagoLength();
        if (primeiro || dist_atual < dist_men) {
            primeiro = false;
            estado_origem = item;
            estado_destino = "";
            dist_men = dist_atual;
        }
        for (i2 = 0; i2 < c; i2++) {
            item2 = listaDeVerificacao[i2];
            simbs = this->aut.getFuncTrans (item, item2);
            if (simbs.count() == 0) {
                continue;
            }
            if (i != i2) {
                prec_desloc = (this->aut.getFuncTrans (item2, item).count() > 0);
                p2 = this->aut.getPosEstado (item2);
                fin2 = this->aut.isEstadoFinal (item2);
                aux1 = segmreta (p, segmreta (p, p2).deslocamento(-COMPRIMENTO_SETA));
                if (fin1 && (! fin2)) {
                    aux1 = segmreta (aux1.deslocamento(ESTADO_DIAMETRO_OFFSET_FINAL / 2), aux1.bottomRight());
                } else if ((! fin1) && fin2) {
                    aux1 = segmreta (aux1.topLeft(), aux1.deslocamento(-ESTADO_DIAMETRO_OFFSET_FINAL / 2));
                }
                if (prec_desloc) { // Se ha uma transicao no sentido contrario, essa seta foi deslocada para dar espaco para a outra.
                    paux1 = QPoint ((int) rint(aux1.left() + ESTADO_DIAMETRO * aux1.sinang() / 4), (int) rint(aux1.top() - ESTADO_DIAMETRO * aux1.cosang() / 4));
                    paux2 = QPoint ((int) rint(aux1.right() + ESTADO_DIAMETRO * aux1.sinang() / 4), (int) rint(aux1.bottom() - ESTADO_DIAMETRO * aux1.cosang() / 4));
                    aux1 = segmreta (paux1, paux2);
                }
                aux2 = segmreta (aux1.deslocamento (aux1.diagoLength() / 2), referencia);
                dist_atual = aux2.diagoLength();
                if (dist_atual < dist_men) {
                    estado_origem = item;
                    estado_destino = item2;
                    dist_men = dist_atual;
                }
            } else {
                aux2 = segmreta (QPoint(p.x(), p.y() + ALT_MEIO_CIRC + ((fin1) ? (ESTADO_DIAMETRO_OFFSET_FINAL / 2) : 0)), referencia);
                dist_atual = aux2.diagoLength();
                if (dist_atual < dist_men) {
                    estado_origem = item;
                    estado_destino = item;
                    dist_men = dist_atual;
                }
            }
        }
    }
    return (dist_men);
}

int editautomato::procuraMaisProximo (QPoint referencia, QVector<QString> listaDeVerificacao, QString& estado_origem) {
    int i, c, dist_men, dist_atual;
    bool primeiro;
    QString item;
    QPoint p;
    segmreta aux1;
    c = listaDeVerificacao.count();
    primeiro = true;
    dist_men = 0;
    for (i = 0; i < c; i++) {
        item = listaDeVerificacao[i];
        p = this->aut.getPosEstado (item);
        aux1 = segmreta(p, referencia);
        dist_atual = aux1.diagoLength();
        if (primeiro || dist_atual < dist_men) {
            primeiro = false;
            estado_origem = item;
            dist_men = dist_atual;
        }
    }
    return (dist_men);
}

void editautomato::mousePressEvent (QMouseEvent *ev) {
    QVector<QString> estados;
    QString orig_men, dest_men;
    int dist_men, c, raio;
    /*
         Momento para reflexao... Eu preciso monitorar os movimentos do mouse para...
         # ...movimentar um estado (nenhuma tecla pressionada)
         # ...criar um arco entre 2 estados (SHIFT + clique)
    */
    ev->accept();
    if (this->movendoAlguem || this->criandoLink) {
        return;
    }
    estados = this->aut.getEstado ();
    c = estados.count();
    if (c != 0) {
        dist_men = this->procuraMaisProximo (ev->pos(), estados, orig_men, dest_men);
        raio = ESTADO_DIAMETRO / 2 + DISTANCIA_ESTADO;
        if (dist_men > raio) {
            this->estado_origem_selecionado = "";
            this->estado_destino_selecionado = "";
        } else if (ev->modifiers() & Qt::ControlModifier) {
            if (dest_men == "") {
                this->estado_origem_selecionado = orig_men;
                this->estado_destino_selecionado = "";
                this->criandoLink = true;
                this->jaSaiuArea = false;
            }
        } else {
            this->estado_origem_selecionado = orig_men;
            this->estado_destino_selecionado = dest_men;
            this->movendoAlguem = (dest_men == "");
        }
        this->update ();
    }
}

void editautomato::mouseMoveEvent (QMouseEvent *ev) {
    QString orig_men, dest_men, orig_atual, dest_atual;
    QVector<QString> estados;
    QVector<QChar> simbs;
    QPoint p, p2;
    int dist_pts;
    segmreta aux;
    ev->accept();
    this->x = ev->x();
    this->y = ev->y();
    if (this->movendoAlguem) {
        this->aut.setPosEstado (this->estado_origem_selecionado, ev->pos());
        this->update ();
    } else if (this->criandoLink) {
        if (this->jaSaiuArea) {
            estados = this->aut.getEstado();
            dist_pts = this->procuraMaisProximo (ev->pos(), estados, dest_men) - ESTADO_DIAMETRO / 2 - DISTANCIA_ESTADO;
            if (this->aut.isEstadoFinal (dest_men)) {
                dist_pts -= ESTADO_DIAMETRO_OFFSET_FINAL / 2;
            }
            if (dist_pts < 0) {
                this->estado_destino_selecionado = dest_men;
            } else {
                this->estado_destino_selecionado = "";
            }
            this->update ();
        } else {
            p = this->aut.getPosEstado (this->estado_origem_selecionado);
            aux = segmreta (p, ev->pos());
            dist_pts = ESTADO_DIAMETRO / 2 + DISTANCIA_ESTADO;
            if (this->aut.isEstadoFinal (this->estado_origem_selecionado)) {
                dist_pts += ESTADO_DIAMETRO_OFFSET_FINAL / 2;
            }
            this->jaSaiuArea = (this->jaSaiuArea || aux.diagoLength() > dist_pts);
        }
    }
}

void editautomato::mouseReleaseEvent (QMouseEvent *ev) {
    ev->accept();
    if (this->movendoAlguem) {
        this->movendoAlguem = false;
    } else if (this->criandoLink) {
        if (this->estado_destino_selecionado != "") {
            QString cache1 = this->estado_origem_selecionado;
            QString cache2 = this->estado_destino_selecionado;
            this->mouseDoubleClickEvent (ev);
            this->estado_origem_selecionado = cache1;
            if (this->aut.getFuncTrans (cache1, cache2).count() != 0) {
                this->estado_destino_selecionado = cache2;
            } else {
                this->estado_destino_selecionado = "";
            }
        }
        this->criandoLink = false;
    }
    this->update ();
}

void editautomato::mouseDoubleClickEvent (QMouseEvent *ev) {
    QString resp, txt;
    QVector<QChar> simbs, novos, velhos;
    QChar item;
    bool ok;
    int cont;
    if (ev != NULL) {
        ev->accept();
    }
    if (this->estado_origem_selecionado != "") {
        if (this->estado_destino_selecionado != "") {
            // Oferecer uma opcao de alterar os simbolos do arco
            simbs = this->aut.getFuncTrans (this->estado_origem_selecionado, this->estado_destino_selecionado);
            txt = "";
            for (cont = simbs.count() - 1; cont >= 0; cont--) {
                if (simbs[cont].isNull()) {
                    txt = QString("3") + txt;
                } else {
                    txt = QString (simbs[cont]) + txt;
                }
            }
            velhos = this->aut.getSimb();
            do {
                resp = QInputDialog::getText ((QWidget*)0, QString(TITULO) + " - Alterar simbolos de transicao", "Entre com uma sequencia de simbolos associados a transicao selecionada.\n\nDigite '3' se quiser habilitar transicao vazia.\nEntre com quaisquer simbolos pertencentes ao automato.", QLineEdit::Normal, txt, &ok);
                if (ok) {
                    resp = resp.trimmed();
                    if (resp.isEmpty()) {
                        QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Alterar simbolos de transicao", "Erro:\n\nSequencia de simbolos invalida.");
                    } else {
                        novos.clear ();
                        for (cont = resp.length() - 1; cont >= 0; cont--) {
                            item = resp[cont];
                            if (novos.indexOf (item) >= 0 || (! (item == '3' || item.isLetter()))) {
                                QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Alterar simbolos de transicao", "Erro:\n\nSequencia de simbolos invalida.");
                                break;
                            }
                            if (velhos.indexOf (item) < 0 && item != '3') {
                                QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Alterar simbolos de transicao", "Erro:\n\nUm ou mais simbolos na sequencia nao pertencem ao alfabeto do automato.");
                                break;
                            }
                            novos.append (item);
                        }
                        if (cont < 0) {
                            ok = false;
                            for (cont = simbs.count() - 1; cont >= 0; cont--) {
                                this->aut.rmFuncTrans (this->estado_origem_selecionado, simbs[cont], this->estado_destino_selecionado);
                            }
                            for (cont = novos.count() - 1; cont >= 0; cont--) {
                                this->aut.addFuncTrans (this->estado_origem_selecionado, (novos[cont] == '3' ? 0 : novos[cont]), this->estado_destino_selecionado);
                            }
                            this->update ();
                        }
                    }
                }
            } while (ok);
        } else {
            // Oferecer uma opcao de alterar o nome do estado
            do {
                resp = QInputDialog::getText ((QWidget*)0, QString(TITULO) + " - Renomear estado", "Digite o novo nome do estado selecionado:", QLineEdit::Normal, this->estado_origem_selecionado, &ok);
                if (ok) {
                    resp = resp.trimmed();
                    ok = resp.isEmpty();
                    if (ok) {
                        QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Renomear estado", "Erro:\n\nNome de estado invalido.");
                    } else if (this->aut.mvEstado (this->estado_origem_selecionado, resp)) {
                        this->estado_origem_selecionado = resp;
                        this->update ();
                    } else {
                        QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Renomear estado", "Erro:\n\nErro desconhecido ao mudar o nome do estado.\nVerifique se o nome escolhido ja foi atribuido a algum outro estado.");
                        ok = true;
                    }
                }
            } while (ok);
        }
    }
}

void editautomato::processaDEL () {
    // Remover um objeto, que pode ser um estado (vertice) ou uma transicao (arco)
    if (this->movendoAlguem || this->criandoLink) {
        return;
    }
    if (this->estado_origem_selecionado != "") {
        if (this->estado_destino_selecionado != "") {
            if (QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Remover transicao", "Tem certeza que deseja remover a transicao selecionada?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
                this->aut.rmFuncTrans (this->estado_origem_selecionado, this->estado_destino_selecionado);
                this->estado_origem_selecionado = "";
                this->estado_destino_selecionado = "";
                this->update ();
            }
        } else {
            if (QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Remover estado", "Tem certeza que deseja remover o estado selecionado?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
                this->aut.rmEstado (this->estado_origem_selecionado);
                this->estado_origem_selecionado = "";
                this->update ();
            }
        }
    }
}

void editautomato::processaCTRL_DEL () {
    // Limpar o automato inteiro
    if (this->movendoAlguem || this->criandoLink) {
        return;
    }
    if (QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Limpar automato", "Tem certeza que deseja limpar o automato?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        this->aut.clear();
        this->estado_origem_selecionado = "";
        this->estado_destino_selecionado = "";
        this->update ();
    }
}

void editautomato::processaCTRL_N () {
    // Criar um novo estado no automato
    QString resp;
    bool ok;
    if (this->movendoAlguem || this->criandoLink) {
        return;
    }
    do {
        resp = QInputDialog::getText ((QWidget*)0, QString(TITULO) + " - Adicionar novo estado", "Digite o nome do novo estado:", QLineEdit::Normal, QString(), &ok);
        if (ok) {
            resp = resp.trimmed();
            ok = resp.isEmpty();
            if (ok) {
                QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Adicionar novo estado", "Erro:\n\nNome de estado invalido.");
            } else if (this->aut.addEstado (resp)) {
                this->aut.setPosEstado(resp, QPoint (this->x, this->y));
                this->estado_origem_selecionado = resp;
                this->update ();
            } else {
                QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Adicionar novo estado", "Erro:\n\nErro desconhecido ao adicionar o novo estado.\nVerifique se o nome escolhido ja foi atribuido a algum outro estado.");
                ok = true;
            }
        }
    } while (ok);
}

void editautomato::processaCTRL_I () {
    // Marcar um estado como inicial ou nao-inicial
    if (this->movendoAlguem || this->criandoLink) {
        return;
    }
    if (this->estado_origem_selecionado != "" && this->estado_destino_selecionado == "") {
        this->aut.setEstadoInicial (this->estado_origem_selecionado, ! this->aut.isEstadoInicial (this->estado_origem_selecionado));
        this->update ();
    }
}

void editautomato::processaCTRL_L () {
    // Sei que isso aqui nao eh diario, mas eu quero desabafar so um pouquinho, antes de continuar programando
    // QUE SAUDADES DO MEU CARROOOOOOOOO!!
    // Ok, vamos seguir... Criar uma transicao e sugerir ao usuario para cria-la usando CTRL+mouse...
    QString origem, destino, cache1, cache2;
    QVector<QString> estados;
    bool ok;
    if (this->movendoAlguem || this->criandoLink) {
        return;
    }
    estados = this->aut.getEstado();
    do {
        origem = QInputDialog::getText ((QWidget*)0, QString(TITULO) + " - Adicionar nova transicao", "Digite o nome do estado de entrada:\n\n(dica: para criar transicoes, segure a tecla CTRL e arraste o estado de origem ate o estado destino)", QLineEdit::Normal, origem, &ok);
        if (ok) {
            origem = origem.trimmed();
            ok = origem.isEmpty();
            if (ok) {
                QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Adicionar nova transicao", "Erro:\n\nNome de estado invalido.");
            } else if (estados.indexOf (origem) < 0) {
                QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Adicionar nova transicao", "Erro:\n\nO estado digitado nao foi localizado no automato.\nVerifique se o mesmo existe ou crie-o antes.");
                ok = true;
            }
        } else {
            return;
        }
    } while (ok);
    do {
        destino = QInputDialog::getText ((QWidget*)0, QString(TITULO) + " - Adicionar nova transicao", "Digite o nome do estado destino:", QLineEdit::Normal, destino, &ok);
        if (ok) {
            destino = destino.trimmed();
            ok = destino.isEmpty();
            if (ok) {
                QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Adicionar nova transicao", "Erro:\n\nNome de estado invalido.");
            } else if (estados.indexOf (destino) < 0) {
                QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Adicionar nova transicao", "Erro:\n\nO estado digitado nao foi localizado no automato.\nVerifique se o mesmo existe ou crie-o antes.");
                ok = true;
            }
        } else {
            return;
        }
    } while (ok);
    cache1 = this->estado_origem_selecionado;
    cache2 = this->estado_destino_selecionado;
    this->estado_origem_selecionado = origem;
    this->estado_destino_selecionado = destino;
    this->mouseDoubleClickEvent (NULL);
    if (this->aut.getFuncTrans (origem, destino).count() != 0) {
        this->estado_origem_selecionado = origem;
        this->estado_destino_selecionado = destino;
    } else {
        this->estado_origem_selecionado = cache1;
        this->estado_destino_selecionado = cache2;
    }
}

void editautomato::processaCTRL_E () {
    QVector<QChar> velhos, novos;
    QChar item;
    int cont;
    QString txt, resp;
    bool ok;
    if (this->movendoAlguem || this->criandoLink) {
        return;
    }
    // Alterar o alfabeto utilizado pelo automato
    velhos = this->aut.getSimb();
    txt = "";
    for (cont = velhos.count() - 1; cont >= 0; cont--) {
        txt = QString (velhos[cont]) + txt;
    }
    do {
        resp = QInputDialog::getText ((QWidget*)0, QString(TITULO) + " - Editar alfabeto do automato", "Entre com uma sequencia de simbolos que formara o alfabeto de entrada do automato.", QLineEdit::Normal, txt, &ok);
        if (ok) {
            resp = resp.trimmed();
            if (resp.isEmpty()) {
                QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Editar alfabeto do automato", "Erro:\n\nSequencia de simbolos invalida.");
            } else {
                novos.clear ();
                for (cont = resp.length() - 1; cont >= 0; cont--) {
                    item = resp[cont];
                    if (novos.indexOf (item) >= 0 || (! item.isLetter())) {
                        QMessageBox::warning ((QWidget*)0, QString(TITULO) + " - Editar alfabeto do automato", "Erro:\n\nSequencia de simbolos invalida.");
                        break;
                    }
                    novos.append (item);
                }
                if (cont < 0) {
                    ok = false;
                    for (cont = velhos.count() - 1; cont >= 0; cont--) {
                        item = velhos[cont];
                        if (novos.indexOf (item) < 0) {
                            this->aut.rmSimb (item);
                        }
                    }
                    for (cont = novos.count() - 1; cont >= 0; cont--) {
                        item = novos[cont];
                        if (velhos.indexOf (item) < 0) {
                            this->aut.addSimb (item);
                        }
                    }
                    this->update ();
                }
            }
        }
    } while (ok);
}

void editautomato::processaCTRL_F () {
    // Marcar um estado como final ou nao-final
    if (this->movendoAlguem || this->criandoLink) {
        return;
    }
    if (this->estado_origem_selecionado != "" && this->estado_destino_selecionado == "") {
        this->aut.setEstadoFinal (this->estado_origem_selecionado, ! this->aut.isEstadoFinal (this->estado_origem_selecionado));
        this->update ();
    }
}

void editautomato::processaCTRL_S () {
    // O melhor dos comandos: salvar como...
    static QString arqname;
    QString newarqname;
    if (this->movendoAlguem || this->criandoLink) {
        return;
    }
    newarqname = QFileDialog::getSaveFileName ((QWidget*)0, QString (TITULO) + " - Salvar automato", arqname);
    if (newarqname != "") {
        arqname = newarqname;
        if (! this->aut.writeToFile (newarqname)) {
            QMessageBox::critical ((QWidget*)0, QString(TITULO) + " - Salvar automato", QString("Erro:\n\nNao foi possivel salvar o automato no arquivo '") + arqname + "'.");
        }
    }
}

void editautomato::processaCTRL_A () {
    // Abrir um automato previamente salvo
    static QString arqname;
    QString newarqname;
    if (this->movendoAlguem || this->criandoLink) {
        return;
    }
    newarqname = QFileDialog::getOpenFileName ((QWidget*)0, QString (TITULO) + " - Abrir automato", arqname);
    if (newarqname != "") {
        arqname = newarqname;
        this->estado_origem_selecionado = "";
        this->estado_destino_selecionado = "";
        if (! this->aut.readFromFile (newarqname)) {
            QMessageBox::critical ((QWidget*)0, QString(TITULO) + " - Abrir automato", QString("Erro:\n\nNao foi possivel carregar o automato do arquivo '") + arqname + "'.");
        }
        this->update();
    }
}
