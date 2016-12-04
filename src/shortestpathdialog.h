/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef SHORTESTPATHDIALOG_H
#define SHORTESTPATHDIALOG_H

#include <QtGui>

#include "mainwindow.h"
#include "graph.h"

class ShortestPathDialog : public QDialog
{
    Q_OBJECT

public:
    ShortestPathDialog(QWidget *parent, QSet<QString> &vertices, QSet<QString> &pS, QSet<QString> &pWS, Graph &g);

signals:
    void findPath();

private slots:
    void addFindClicked();

private:
    QLabel *labelOne;
    QTextBrowser *textBrowser;
    QComboBox *comboOne;
    QComboBox *comboTwo;
    QPushButton *addButton;
    QPushButton *closeButton;

    QSet<QString> *pS;
    QSet<QString> *pWS;

    Graph graph;
};

#endif // SHORTESTPATHDIALOG_H
