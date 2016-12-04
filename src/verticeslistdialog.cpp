/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include <QtGui>

#include "verticeslistdialog.h"

VerticesListDialog::VerticesListDialog(QWidget *parent, QSet<QString> &vertices)
            : QDialog(parent)
{
    addButton = new QPushButton(tr("&Ok"));
    addButton->setDefault(true);

    closeButton = new QPushButton(tr("&Close Window"));

    QPixmap plusPix(":/images/Ok.png");
    QPixmap goOutPix(":/images/GoOut.png");

    addButton->setIcon(plusPix);
    closeButton->setIcon(goOutPix);

    connect(addButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(close()));

    labelOne = new QLabel;
    textBrowser = new QTextBrowser;

    QList<QString> list = vertices.toList();
    qSort(list);

    QString str;

    for (QList<QString>::iterator iter = list.begin(); iter != list.end(); ++iter) {
        str.append(*iter);
        str.append(";");
        str.append("\n");
    }

    textBrowser->setText(str);

    labelOne->setText(tr("Vertices List:"));

    QHBoxLayout *leftLLayout = new QHBoxLayout;
    leftLLayout->addWidget(labelOne);

    QVBoxLayout *leftVLayout = new QVBoxLayout;
    leftVLayout->addLayout(leftLLayout);
    leftVLayout->addWidget(textBrowser);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(addButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftVLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Vertices List"));
    setFixedHeight(sizeHint().height());
}
