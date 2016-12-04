/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */

#ifndef ADDEDGEDIALOG_H
#define ADDEDGEDIALOG_H

#include <QtGui>

class AddEdgeDialog : public QDialog
{
    Q_OBJECT

public:
    AddEdgeDialog(QWidget *parent, QSet<QString> &vertices);

signals:
    void addEdge(const QString &v1, const QString &v2, const QString &cost);

private slots:
    void addClicked();
    void setTextOne();
    void setTextTwo();

private:
    QLabel *label;
    QLabel *label2;
    QLabel *label3;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit2;
    QLineEdit *lineEdit3;
    QPushButton *addButton;
    QPushButton *closeButton;
    QComboBox *comboOne;
    QComboBox *comboTwo;
};

#endif // ADDEDGEDIALOG_H
