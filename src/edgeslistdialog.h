/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef EDGESLISTDIALOG_H
#define EDGESLISTDIALOG_H

#include <QtGui>
#include "graph.h"

class EdgesListDialog : public QDialog
{
    Q_OBJECT

public:
    EdgesListDialog(QWidget *parent, QSet<QString> &edges);

signals:
    void editGraph();

private slots:
    void editGraphClicked();

private:
    QLabel *labelOne;
    QListWidget *edgesList;
    QPushButton *addButton;
    QPushButton *closeButton;
    QPushButton *editButton;

    Graph graph;
};

#endif // EDGESLISTDIALOG_H
