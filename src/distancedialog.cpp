/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */

#include <QtGui>

#include "distancedialog.h"

DistanceDialog::DistanceDialog(QWidget *parent, QSet<QString> &vertices, Graph &g)
            : QDialog(parent)
{
    graph = g;

    addButton = new QPushButton(tr("&Find"));
    addButton->setDefault(true);

    closeButton = new QPushButton(tr("&Close Window"));

    QPixmap plusPix(":/images/Ok.png");
    QPixmap goOutPix(":/images/GoOut.png");

    addButton->setIcon(plusPix);
    closeButton->setIcon(goOutPix);

    connect(addButton, SIGNAL(clicked()),
            this, SLOT(addFindClicked()));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(close()));

    comboOne = new QComboBox;
    comboTwo = new QComboBox;

    QList<QString> list = vertices.toList();
    qSort(list);

    comboOne->addItems(list);
    comboTwo->addItems(list);

    QHBoxLayout *leftLayout = new QHBoxLayout;
    leftLayout->addWidget(comboOne);
    leftLayout->addWidget(comboTwo);

    labelOne = new QLabel;
    labelTwo = new QLabel;
    labelThree = new QLabel;
    lineEdit = new QLineEdit;

    labelOne->setText(tr("From The Vertex:"));
    labelTwo->setText(tr("To The Vertex:"));
    labelThree->setText(tr("Distance:"));

    QHBoxLayout *leftLLayout = new QHBoxLayout;
    leftLLayout->addWidget(labelOne);
    leftLLayout->addWidget(labelTwo);

    QVBoxLayout *leftVLayout = new QVBoxLayout;
    leftVLayout->addLayout(leftLLayout);
    leftVLayout->addLayout(leftLayout);
    leftVLayout->addWidget(labelThree);
    leftVLayout->addWidget(lineEdit);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addStretch();
    rightLayout->addWidget(addButton);    
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftVLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("The Distance Between The Vertices"));
    setFixedHeight(sizeHint().height());
}

void DistanceDialog::addFindClicked()
{
    QString v1 = comboOne->currentText();
    QString v2 = comboTwo->currentText();

    double d = graph.GetDistanceFromVertexToVertex(v1.toStdString(), v2.toStdString());

    QString str;

    str.setNum(d);

    lineEdit->setText(str);
}
