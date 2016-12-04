/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */

#ifndef CENTERDIALOG_H
#define CENTERDIALOG_H

#include <QtGui>
#include "graph.h"

class CenterDialog : public QDialog
{
    Q_OBJECT

public:
    CenterDialog(QWidget *parent, Graph &g);

signals:
    void findCenter();

private slots:
    void addFindClicked();

private:
    QLabel *labelOne;
    QLabel *labelTwo;
    QLabel *labelThree;
    QTextBrowser *textBrowserOne;
    QTextBrowser *textBrowserTwo;
    QLineEdit *lineEdit;
    QPushButton *addButton;
    QPushButton *closeButton;

    Graph graph;
};

#endif // CENTERDIALOG_H
