/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef RADIUSDIALOG_H
#define RADIUSDIALOG_H

#include <QtGui>
#include "graph.h"

class RadiusDialog : public QDialog
{
    Q_OBJECT

public:
    RadiusDialog(QWidget *parent, Graph &g);

signals:

private slots:
    void addFindClicked();

private:
    QLabel *labelOne;
    QLineEdit *lineEdit;
    QPushButton *addButton;
    QPushButton *closeButton;

    Graph graph;
};

#endif // RADIUSDIALOG_H
