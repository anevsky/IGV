/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */

#ifndef ADJACENTVERTICESDIALOG_H
#define ADJACENTVERTICESDIALOG_H

#include <QtGui>
#include "graph.h"

class AdjacentVerticesDialog : public QDialog
{
    Q_OBJECT

public:
    AdjacentVerticesDialog(QWidget *parent, QSet<QString> &vertices, Graph &g);

signals:

private slots:
    void addFindClicked();

private:
    QLabel *labelOne;
    QLabel *labelTwo;
    QComboBox *comboOne;
    QTextBrowser *textBrowser;
    QPushButton *addButton;
    QPushButton *closeButton;

    Graph graph;
};

#endif // ADJACENTVERTICESDIALOG_H
