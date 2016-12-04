/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include <QtGui>

#include "edgeslistdialog.h"

EdgesListDialog::EdgesListDialog(QWidget *parent, QSet<QString> &edges)
    : QDialog(parent)
{
    addButton = new QPushButton(tr("&Ok"));
    addButton->setDefault(true);

    closeButton = new QPushButton(tr("&Close Window"));

    editButton = new QPushButton(tr("&Create A Graph According To A Given Configuration"));

    QPixmap plusPix(":/images/Ok.png");
    QPixmap goOutPix(":/images/GoOut.png");
    QPixmap wizardPix(":/images/Plus.png");

    addButton->setIcon(plusPix);
    closeButton->setIcon(goOutPix);
    editButton->setIcon(wizardPix);

    connect(addButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(editButton, SIGNAL(clicked()),
            this, SLOT(editGraphClicked()));

    labelOne = new QLabel;
    edgesList = new QListWidget;

    if (!edges.empty()) {

        QString strList;

        for (QSet<QString>::iterator iter = edges.begin(); iter != edges.end(); ++iter) {
            strList.append(*iter);
        }

        QStringList list;

        list = strList.split("\n");

        edgesList->addItems(list);
        edgesList->sortItems(Qt::AscendingOrder);
        delete edgesList->takeItem(0);

        for (int i = 0; i != edgesList->count(); ++i) {
            edgesList->item(i)->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
        }
    }

    labelOne->setText(tr("Edges List:"));

    QHBoxLayout *leftLLayout = new QHBoxLayout;
    leftLLayout->addWidget(labelOne);

    QVBoxLayout *leftVLayout = new QVBoxLayout;
    leftVLayout->addLayout(leftLLayout);
    leftVLayout->addWidget(edgesList);
    leftVLayout->addWidget(editButton);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(addButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftVLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Edges List"));
    setFixedHeight(sizeHint().height());
}

void EdgesListDialog::editGraphClicked()
{
    QList<QListWidgetItem*> items = edgesList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

    QFile fileToWrite("sysFiles/sysEditGraph.igv");

    fileToWrite.open(QIODevice::WriteOnly);
    QTextStream out(&fileToWrite);

    QStringList list;
    QListWidgetItem *item;
    for (QList<QListWidgetItem*>::iterator iter = items.begin(); iter != items.end(); ++iter) {
        item = *iter;
        if (item->text().toUtf8() != "") {
            list.append(item->text().toUtf8());
        }
    }

    for (QStringList::iterator iter = list.begin(); iter != list.end(); ++iter) {
        out << *iter << "\n";
    }

    fileToWrite.close();   

    emit editGraph();
    //emit close();
}
