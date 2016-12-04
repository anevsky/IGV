/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include <QtGui>

#include "nearestvertexdialog.h"

NearestVertexDialog::NearestVertexDialog(QWidget *parent, QSet<QString> &vertices, Graph &g)
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

    QList<QString> list = vertices.toList();
    qSort(list);

    comboOne->addItems(list);

    QHBoxLayout *leftLayout = new QHBoxLayout;
    leftLayout->addWidget(comboOne);

    labelOne = new QLabel;
    labelTwo = new QLabel;
    labelThree = new QLabel;
    lineEdit = new QLineEdit;

    labelOne->setText(tr("Vertex:"));
    labelTwo->setText(tr("Nearest Vertex:"));
    labelThree->setText(tr("# Will Ve Found One Of The possible Nearest Vertices,\nif several."));

    QHBoxLayout *leftLLayout = new QHBoxLayout;
    leftLLayout->addWidget(labelOne);

    QVBoxLayout *leftVLayout = new QVBoxLayout;
    leftVLayout->addLayout(leftLLayout);
    leftVLayout->addLayout(leftLayout);
    leftVLayout->addWidget(labelTwo);
    leftVLayout->addWidget(lineEdit);
    leftVLayout->addWidget(labelThree);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(addButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftVLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Nearest Vertex"));
    setFixedHeight(sizeHint().height());
}

void NearestVertexDialog::addFindClicked()
{
    QString v1 = comboOne->currentText();

    std::string str = graph.GetNearestVertex(v1.toStdString());

    QString qs = str.c_str();

    lineEdit->setText(qs);
}
