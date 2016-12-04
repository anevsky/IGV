/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */

#ifndef DISTANCEDIALOG_H
#define DISTANCEDIALOG_H

#include <QtGui>
#include "graph.h"

class DistanceDialog : public QDialog
{
    Q_OBJECT

public:
    DistanceDialog(QWidget *parent, QSet<QString> &vertices, Graph &g);

private slots:
    void addFindClicked();

private:
    QLabel *labelOne;
    QLabel *labelTwo;
    QLabel *labelThree;
    QComboBox *comboOne;
    QComboBox *comboTwo;
    QLineEdit *lineEdit;
    QPushButton *addButton;
    QPushButton *closeButton;

    Graph graph;
};

#endif // DISTANCEDIALOG_H
