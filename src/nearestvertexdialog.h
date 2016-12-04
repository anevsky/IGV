/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef NEARESTVERTEXDIALOG_H
#define NEARESTVERTEXDIALOG_H

#include <QtGui>
#include "graph.h"

class NearestVertexDialog : public QDialog
{
    Q_OBJECT

public:
    NearestVertexDialog(QWidget *parent, QSet<QString> &vertices, Graph &g);

signals:

private slots:
    void addFindClicked();

private:
    QLabel *labelOne;
    QLabel *labelTwo;
    QLabel *labelThree;
    QComboBox *comboOne;
    QLineEdit *lineEdit;
    QPushButton *addButton;
    QPushButton *closeButton;

    Graph graph;
};

#endif // NEARESTVERTEXDIALOG_H
