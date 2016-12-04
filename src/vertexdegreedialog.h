/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef VERTEXDEGREEDIALOG_H
#define VERTEXDEGREEDIALOG_H

#include <QtGui>
#include "graph.h"

class VertexDegreeDialog : public QDialog
{
    Q_OBJECT

public:
    VertexDegreeDialog(QWidget *parent, QSet<QString> &vertices, Graph &g);

signals:

private slots:
    void addFindClicked();

private:
    QLabel *labelOne;
    QLabel *labelTwo;
    QComboBox *comboOne;
    QLineEdit *lineEdit;
    QPushButton *addButton;
    QPushButton *closeButton;

    Graph graph;
};

#endif // VERTEXDEGREEDIALOG_H
