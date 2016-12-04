/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef GRAPHINFO_H
#define GRAPHINFO_H

#include "QtGui"

class GraphInfo : public QDialog
{
    Q_OBJECT

public:
    GraphInfo(QWidget *parent = 0);

signals:

private slots:

private:
    QTextBrowser *textBrowser;
    QPushButton *closeButton;
};

#endif // GRAPHINFO_H
