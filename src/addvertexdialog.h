/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */

#ifndef ADDVERTEXDIALOG_H
#define ADDVERTEXDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class AddVertexDialog : public QDialog
{
    Q_OBJECT

public:
    AddVertexDialog(QWidget *parent = 0);

signals:
    void addVertex(const QString &str);

private slots:
    void addClicked();
    void enableAddButton(const QString &text);

private:
    QLabel *label;
    QLabel *label2;
    QLineEdit *lineEdit;
    QPushButton *addButton;
    QPushButton *closeButton;
};

#endif // ADDVERTEXDIALOG_H
