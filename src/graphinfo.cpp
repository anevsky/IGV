/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include "graphinfo.h"

GraphInfo::GraphInfo(QWidget *parent)
            : QDialog(parent)
{
    closeButton = new QPushButton(tr("&Close Window"));

    QPixmap goOutPix(":/images/GoOut.png");

    closeButton->setIcon(goOutPix);

    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(close()));

    textBrowser = new QTextBrowser;

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(textBrowser);
    rightLayout->addWidget(closeButton);

    /*

    QFile file(":graph.html");
    file.open(QIODevice::ReadOnly);

    QString str;
    str = file.readAll();
    file.close();

    textBrowser->setHtml(str);

    */

    textBrowser->setHtml(tr(
"<h1>IGV - Intelligent Graph Visualizer</h1>"
"<p><img src=\":/images/logo.png\"></p>"
"<h1>Graph Theory</h1>"
"<p>In mathematics, a graph is an abstract representation of a set of objects where some pairs of the objects are connected by links. "
"The interconnected objects are represented by mathematical abstractions called vertices, and the links that connect some pairs of vertices "
"are called edges. Typically, a graph is depicted in diagrammatic form as a set of dots for the vertices, joined by lines or curves for the "
"edges. Graphs are one of the objects of study in discrete mathematics.</p>"
"<p>The edges may be directed (asymmetric) or undirected (symmetric). For example, if the vertices represent people at a party, and there is "
"an edge between two people if they shake hands, then this is an undirected graph, because if person A shook hands with person B, "
"then person B also shook hands with person A. On the other hand, if the vertices represent people at a party, and there is an edge "
"from person A to person B when person A knows of person B, then this graph is directed, because knowing of someone is not necessarily "
"a symmetric relation (that is, one person knowing of another person does not necessarily imply the reverse; for example, many fans may "
"know of a celebrity, but the celebrity is unlikely to know of all their fans). This latter type of graph is called a directed graph and "
"the edges are called directed edges or arcs; in contrast, a graph where the edges are not directed is called undirected.</p>"
"<p>Vertices are also called nodes or points, and edges are also called lines. Graphs are the basic subject studied by graph theory. "
"The word graph was first used in this sense by James Joseph Sylvester in 1878.</p>"
"<p>Please, read more here:</p>"
"<p>Graph (Wikipedia);</p>"
"<p>Graph Theory (Wikipedia);</p>"
"<p>Glossary Of Graph Theory (Wikipedia).</p>"
    ));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Graph Theory"));
    setFixedHeight(sizeHint().height());
}
