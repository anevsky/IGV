/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */

#include <QtGui>

#include "addedgedialog.h"

AddEdgeDialog::AddEdgeDialog(QWidget *parent, QSet<QString> &vertices)
            : QDialog(parent)
{        
    addButton = new QPushButton(tr("&Add"));
    addButton->setDefault(true);

    closeButton = new QPushButton(tr("&Close Window"));

    QPixmap plusPix(":/images/Plus.png");
    QPixmap goOutPix(":/images/GoOut.png");

    addButton->setIcon(plusPix);
    closeButton->setIcon(goOutPix);

    comboOne = new QComboBox;
    comboTwo = new QComboBox;

    connect(addButton, SIGNAL(clicked()),
            this, SLOT(addClicked()));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(close()));    

    label = new QLabel(tr("Vertex &1:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    lineEdit->setFocus();

    label2 = new QLabel(tr("Vertex &2:"));
    lineEdit2 = new QLineEdit;
    label2->setBuddy(lineEdit2);

    label3 = new QLabel(tr("&Cost:"));
    lineEdit3 = new QLineEdit;
    label3->setBuddy(lineEdit3);
    lineEdit3->setText("1");

    connect(comboOne, SIGNAL(activated(QString)), this, SLOT(setTextOne()));
    connect(comboTwo, SIGNAL(activated(QString)), this, SLOT(setTextTwo()));    

    QList<QString> list = vertices.toList();
    qSort(list);

    comboOne->addItems(list);
    comboTwo->addItems(list);

    setTextOne();
    setTextTwo();

    QVBoxLayout *topLeftLayout = new QVBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(comboOne);
    topLeftLayout->addWidget(lineEdit);

    topLeftLayout->addWidget(label2);
    topLeftLayout->addWidget(comboTwo);
    topLeftLayout->addWidget(lineEdit2);

    topLeftLayout->addWidget(label3);
    topLeftLayout->addWidget(lineEdit3);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addStretch();
    rightLayout->addWidget(addButton);
    rightLayout->addWidget(closeButton);      

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Add Edge"));
    setFixedHeight(sizeHint().height());
}

void AddEdgeDialog::addClicked()
{
    QString text = lineEdit->text();
    QString text2 = lineEdit2->text();
    QString text3 = lineEdit3->text();
    emit addEdge(text, text2, text3);    
    lineEdit3->setText("1");
    comboOne->setFocus();
}

void AddEdgeDialog::setTextOne()
{
    lineEdit->setText(comboOne->currentText());
}

void AddEdgeDialog::setTextTwo()
{
    lineEdit2->setText(comboTwo->currentText());
}
