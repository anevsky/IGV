/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include <QtGui>

#include "shortestpathdialog.h"

ShortestPathDialog::ShortestPathDialog(QWidget *parent, QSet<QString> &vertices, QSet<QString> &pSf, QSet<QString> &pWSf, Graph &g)
            : QDialog(parent)
{
    graph = g;

    pS = &pSf;
    pWS = &pWSf;

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
    textBrowser = new QTextBrowser;

    labelOne->setText(tr("The Path Might Look Like:"));

    QLabel *labelTwo = new QLabel;
    QLabel *labelThree = new QLabel;

    labelTwo->setText(tr("Start:"));
    labelThree->setText(tr("End:"));

    QHBoxLayout *leftLLayout = new QHBoxLayout;
    leftLLayout->addWidget(labelTwo);
    leftLLayout->addWidget(labelThree);

    QVBoxLayout *leftVLayout = new QVBoxLayout;
    leftVLayout->addLayout(leftLLayout);
    leftVLayout->addLayout(leftLayout);
    leftVLayout->addWidget(labelOne);
    leftVLayout->addWidget(textBrowser);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(addButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftVLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("The Shortest Path"));
    setFixedHeight(sizeHint().height());
}

void ShortestPathDialog::addFindClicked()
{
    QString v1 = comboOne->currentText();
    QString v2 = comboTwo->currentText();

    std::string str;

    if (v1 != v2) {
        str = graph.GetShortestPath(v1.toStdString(), v2.toStdString());
    } else {
        return;
    }

    if (!str.size()) {
        return;
    }

    QString qs = str.c_str();

    textBrowser->setText(qs);

    QStringList list;
    list = qs.split("--");

    if (!list.empty()) {
        pS->insert(list.takeFirst());
        pS->insert(list.takeLast());
        QList<QString>::iterator iter = list.begin();
        while (iter != list.end()) {
            pWS->insert(*iter);
            ++iter;
        }
        emit findPath();
    }    
}
