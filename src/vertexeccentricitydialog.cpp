/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include <QtGui>

#include "vertexeccentricitydialog.h"

VertexEccentricityDialog::VertexEccentricityDialog(QWidget *parent, QSet<QString> &vertices, Graph &g)
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
    textBrowser = new QTextBrowser;

    labelOne->setText(tr("Vertex:"));
    labelTwo->setText(tr("Eccentricity:"));

    QHBoxLayout *leftLLayout = new QHBoxLayout;
    leftLLayout->addWidget(labelOne);

    QVBoxLayout *leftVLayout = new QVBoxLayout;
    leftVLayout->addLayout(leftLLayout);
    leftVLayout->addLayout(leftLayout);
    leftVLayout->addWidget(labelTwo);
    leftVLayout->addWidget(textBrowser);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(addButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftVLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Vertex Eccentricity"));
    setFixedHeight(sizeHint().height());
}

void VertexEccentricityDialog::addFindClicked()
{
    QString v1 = comboOne->currentText();

    std::map<std::string, double> map = graph.GetVertexEccentricity(v1.toStdString());

    QString qs, n;
    std::string str;

    for (std::map<std::string, double>::iterator iter = map.begin(); iter != map.end(); ++iter) {
        qs += tr("from '");
        str = iter->first;
        qs += str.c_str();
        qs += tr("': ");
        n.setNum(iter->second);
        qs += n;
        qs += ";";
        qs.append("\n");
    }

    textBrowser->setText(qs);
}
