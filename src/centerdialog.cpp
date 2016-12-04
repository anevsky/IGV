/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */

#include <QtGui>

#include "centerdialog.h"

CenterDialog::CenterDialog(QWidget *parent, Graph &g)
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

    labelOne = new QLabel;
    labelTwo = new QLabel;
    labelThree = new QLabel;
    textBrowserOne = new QTextBrowser;
    textBrowserTwo = new QTextBrowser;
    lineEdit = new QLineEdit;

    labelOne->setText(tr("Central Vertices:"));
    labelTwo->setText(tr("Central Edges:"));
    labelThree->setText(tr("The Maximum Distance Between The Centers:"));

    QHBoxLayout *leftLLayout = new QHBoxLayout;
    leftLLayout->addWidget(labelOne);
    leftLLayout->addWidget(labelTwo);

    QHBoxLayout *leftLayout = new QHBoxLayout;
    leftLayout->addWidget(textBrowserOne);
    leftLayout->addWidget(textBrowserTwo);

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
}

void CenterDialog::addFindClicked()
{
    typedef std::pair< std::pair<std::string, std::string>, double > t_Edge;

    std::set<std::string> set = graph.m_Center.vertices;
    std::list<t_Edge> list = graph.m_Center.edges;
    double d = graph.m_Center.maxDistanceBetweenCenter;

    std::string str;
    QString qs;

    for (std::set<std::string>::iterator iter = set.begin(); iter != set.end(); ++iter) {
        str = *iter;
        qs += str.c_str();
        qs += ";";
        qs.append("\n");
    }

    textBrowserOne->setText(qs);

    str.clear();
    qs.clear();

    std::pair< std::pair<std::string, std::string>, double > pair;

    std::string s1, s2;
    double sc;
    QString v, vc;

    for (std::list<t_Edge>::iterator iter = list.begin(); iter != list.end(); ++iter) {
        pair = *iter;
        s1 = pair.first.first;
        s2 = pair.first.second;
        sc = pair.second;

        v += s1.c_str();
        v += "--";
        v += s2.c_str();
        v += "; cost:";
        vc.setNum(sc);
        v += vc;
        v += "$;";
        v += "\n";
    }

    textBrowserTwo->setText(v);

    qs.clear();

    qs.setNum(d);
    lineEdit->setText(qs);

    emit findCenter();
}
