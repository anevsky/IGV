/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef VERTICESLISTDIALOG_H
#define VERTICESLISTDIALOG_H

#include <QtGui>

class VerticesListDialog : public QDialog
{
    Q_OBJECT

public:
    VerticesListDialog(QWidget *parent, QSet<QString> &vertices);

signals:

private slots:

private:
    QLabel *labelOne;
    QTextBrowser *textBrowser;
    QPushButton *addButton;
    QPushButton *closeButton;
};

#endif // VERTICESLISTDIALOG_H
