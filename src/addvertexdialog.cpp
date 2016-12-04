/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */

#include <QtGui>

#include "addvertexdialog.h"

AddVertexDialog::AddVertexDialog(QWidget *parent)
    : QDialog(parent)
{
    label = new QLabel(tr("Vertex Name:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    lineEdit->setFocus();

    QRegExp regExp("[A-Za-z0-9]{1,30}");
    lineEdit->setValidator(new QRegExpValidator(regExp, this));

    addButton = new QPushButton(tr("&Add"));
    addButton->setDefault(true);
    //addButton->setEnabled(false);

    closeButton = new QPushButton(tr("&Close Window"));

    QPixmap plusPix(":/images/Plus.png");
    QPixmap goOutPix(":/images/GoOut.png");

    addButton->setIcon(plusPix);
    closeButton->setIcon(goOutPix);

   // connect(lineEdit, SIGNAL(textChanged(const QString &)),
   //         this, SLOT(enableAddButton(const QString &)));
    connect(addButton, SIGNAL(clicked()),
            this, SLOT(addClicked()));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(close()));

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    label2 = new QLabel(tr("# Already there exists a vertex is added will not."));
    leftLayout->addWidget(label2);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(addButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Add Vertex"));
    setFixedHeight(sizeHint().height());
}

void AddVertexDialog::addClicked()
{
    QString text = lineEdit->text();
    emit addVertex(text);
    lineEdit->clear();
    lineEdit->setFocus();
}

void AddVertexDialog::enableAddButton(const QString &text)
{
    addButton->setEnabled(!text.isEmpty());
}
