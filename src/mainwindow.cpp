/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include <QtGui>
#include <QFile>
#include <QString>
#include <QTextStream>

#include <iostream>
#include <fstream>
#include <string>

#include "mainwindow.h"

#include "graph.h"
#include "file.h"
#include "graphwidget.h"
#include "addvertexdialog.h"
#include "addedgedialog.h"
#include "adjacentverticesdialog.h"
#include "centerdialog.h"
#include "distancedialog.h"
#include "edgeslistdialog.h"
#include "nearestvertexdialog.h"
#include "radiusdialog.h"
#include "shortestpathdialog.h"
#include "vertexdegreedialog.h"
#include "vertexeccentricitydialog.h"
#include "verticeslistdialog.h"
#include "graphinfo.h"

MainWindow::MainWindow()
{
    graphWidget = new GraphWidget;
    setCentralWidget(graphWidget);

    graph = new Graph;

    addVertexDialog = 0;
    addEdgeDialog = 0;

    adjacentVerticesDialog = 0;
    centerDialog = 0;
    distanceDialog = 0;
    edgesListDialog = 0;
    nearestVertexDialog = 0;
    radiusDialog = 0;
    shortestPathDialog = 0;
    vertexDegreeDialog = 0;
    vertexEccentricityDialog = 0;
    verticesListDialog = 0;
    graphInfoDialog = 0;

    createActions();
    createMenus();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("IGV - Intelligent Graph Visualizer"));

    setUnifiedTitleAndToolBarOnMac(true);

    connect(addVertexButton, SIGNAL(clicked()), this, SLOT(addVertexButtonClicked()));
    connect(addEdgeButton, SIGNAL(clicked()), this, SLOT(addEdgeButtonClicked()));

    connect(verticesListButton, SIGNAL(clicked()), this, SLOT(verticesListButtonClicked()));
    connect(edgesListButton, SIGNAL(clicked()), this, SLOT(edgesListButtonClicked()));

    connect(freezeEdgeButton, SIGNAL(clicked()), this, SLOT(freezeEdgeButtonClicked()));
    connect(pullEdgeButton, SIGNAL(clicked()), this, SLOT(pullEdgeButtonClicked()));
    connect(strainEdgeButton, SIGNAL(clicked()), this, SLOT(strainEdgeButtonClicked()));
    connect(editEdgeButton, SIGNAL(toggled(bool)), this, SLOT(editEdgeButtonToggled()));

    connect(calculateButton, SIGNAL(clicked()), this, SLOT(calculateButtonClicked()));
    connect(shortestPathButton, SIGNAL(clicked()), this, SLOT(shortestPathButtonClicked()));
    connect(distanceButton, SIGNAL(clicked()), this, SLOT(distanceButtonClicked()));
    connect(nearestVertexButton, SIGNAL(clicked()), this, SLOT(nearestVertexButtonClicked()));
    connect(radiusButton, SIGNAL(clicked()), this, SLOT(radiusButtonClicked()));
    connect(vertexDegreeButton, SIGNAL(clicked()), this, SLOT(vertexDegreeButtonClicked()));
    connect(adjacentVerticesButton, SIGNAL(clicked()), this, SLOT(adjacentVerticesButtonClicked()));
    connect(vertexEccentricityButton, SIGNAL(clicked()), this, SLOT(vertexEccentricityButtonClicked()));
    connect(centerButton, SIGNAL(clicked()), this, SLOT(centerButtonClicked()));

    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

}

void MainWindow::newGraph()
{
    if (okToContinue()) {
        setCurrentFile("");
    }

    addVertexDialog = 0;
    addEdgeDialog = 0;

    adjacentVerticesDialog = 0;
    centerDialog = 0;
    distanceDialog = 0;
    edgesListDialog = 0;
    nearestVertexDialog = 0;
    radiusDialog = 0;
    shortestPathDialog = 0;
    vertexDegreeDialog = 0;
    vertexEccentricityDialog = 0;
    verticesListDialog = 0;

    setCurrentFile("");

    graphWidget->scene->clear();

    delete graph;

    graphWidget->verticesSet.clear();
    graphWidget->verticesNames.clear();
    graphWidget->vertexMap.clear();
    graphWidget->edgesSet.clear();
    graphWidget->edgeMap.clear();
    graphWidget->centerSet.clear();
    graphWidget->pathSet.clear();
    graphWidget->pathWaySet.clear();

    graph = new Graph;

    int vsize = graphWidget->verticesSet.size();
    int esize = graphWidget->edgesSet.size();

    for (int i = 0; i != vsize; ++i) {
        graphWidget->nodes[i] = 0;
    }

    for (int i = 0; i != esize; ++i) {
        graphWidget->edges[i] = 0;
    }

    rightLineEdit2->clear();
    rightLineEdit3->clear();
    rightLineEdit4->clear();
    rightLineEdit5->clear();
    rightLineEdit6->clear();
    
    statusBar()->showMessage(tr(""));
}

void MainWindow::loadGraph()
{
    if (okToContinue()) {
        if (graph) {
            newGraph();
        }

        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Load The Graph Configuration - IGV"), ".",
                                   tr("IGV Files (*.igv)"));
        if (!fileName.isEmpty()) {
            setCurrentFile(fileName);

            QApplication::setOverrideCursor(Qt::WaitCursor);

            graph->readFile(fileName);
            curFile = fileName;

            emit calculateButtonClicked();

            QFile file("sysFiles/sysShowVertices.igv");
            file.open(QIODevice::ReadOnly);

            QString str;
            str = file.readAll();
            file.close();

            QStringList listNodes = str.split(" ");

            for (QStringList::iterator iter = listNodes.begin(); iter != listNodes.end(); ++iter) {
                graphWidget->addNode(*iter);
             }

            QFile file2("sysFiles/sysShowEdges.igv");
            file2.open(QIODevice::ReadOnly);

            QString str2;
            str2 = file2.readAll();
            file2.close();

            QStringList listEdges = str2.split(" ");
            listEdges.toVector();
            QString s1, s2, s3;

            int i = 0;
            while (i != listEdges.size() - 1) {
                s1 = listEdges[i++];
                s2 = listEdges[i++];
                s3 = listEdges[i++];
                if (i < listEdges.size()) { graphWidget->addEdge(s1, s2, s3); }
            }

            QFile filePos(fileName);
            filePos.open(QIODevice::ReadOnly);

            QString strPos;
            strPos = filePos.readAll();

            filePos.close();

            QFile filep("sysFiles/Pos.igv");
            filep.open(QIODevice::WriteOnly);

            QTextStream outp(&filep);
            outp << strPos;

            filep.close();

            if (1) {
                File file;
                file.Read("sysFiles/Pos.igv");
                File::t_FileData fileData;
                file.GetData(fileData);

                for (File::t_FileData::iterator iter = fileData.begin(); iter != fileData.end(); ++iter) {
                    if (*iter == '@') {
                        freezeEdgeButtonClicked();
                        freezeEdgeButton->setChecked(true);
                        fileData.erase(fileData.begin(), iter + 1);
                        break;
                    }                    
                }

                file.Save(fileData, "sysFiles/Pos.igv");
            }

            if (1) {
                QFile file("sysFiles/Pos.igv");
                file.open(QIODevice::ReadOnly);

                QString str;
                str = file.readAll();

                file.close();

                QStringList listPos = str.split(" ");
                for (QStringList::iterator iter = listPos.begin(); iter < listPos.end() - 1; ++iter) {
                    Node *node = graphWidget->vertexMap[*iter];

                    ++iter;
                    QString s1 = *iter;

                    ++iter;
                    QString s2 = *iter;

                    node->setPos(s1.toDouble(), s2.toDouble());
                }
            }

            std::string s;
            QString qs;

            Node *node;
            std::set<std::string> set = graph->m_Center.vertices;

            if (!set.empty()) {
                s = *set.begin();
                qs += s.c_str();
                node = graphWidget->vertexMap[qs];
                graphWidget->centerNode = node;
            }

            qs.clear();

            QApplication::restoreOverrideCursor();
        }    
    }

    statusBar()->showMessage(tr(""));
}

bool MainWindow::saveGraph()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                               tr("Save The Graph Configuration - IGV"), ".",
                               tr("IGV Files (*.igv)"));
    if (fileName.isEmpty())
        return false;   

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);

    for (QSet<QString>::iterator iter = graphWidget->edgesSet.begin(); iter != graphWidget->edgesSet.end(); ++iter) {
        out << *iter;
    }

    out << "@";

    for (QSet<QString>::iterator iter = graphWidget->verticesNames.begin(); iter != graphWidget->verticesNames.end(); ++iter) {
        Node *node = graphWidget->vertexMap[*iter];
        out << *iter;
        out << " ";
        out << node->pos().x();
        out << " ";
        out << node->pos().y();
        out << " ";
    }

    file.close();

    statusBar()->showMessage(tr(""));

    return saveGraphFile(fileName);
}

bool MainWindow::saveGraphInfo()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                               tr("Save Information About The Graph - IGV"), ".",
                               tr("HTML Files (*.html)"));
    if (fileName.isEmpty())
        return false;

    if ((graphWidget->verticesNames.size() > 0) && (graphWidget->verticesNames.size() != graph->m_Vertices.size())) {
        emit calculateButtonClicked();
    }

    graph->SaveAsHtml(fileName.toStdString());

    statusBar()->showMessage(tr(""));

    return saveGraphInfoFile(fileName);
}

void MainWindow::graphInfo()
{
    statusBar()->showMessage(tr(""));

    if (!graphInfoDialog) {
            graphInfoDialog = new GraphInfo(this);
        }

    graphInfoDialog->setMinimumSize(650, 550);

        graphInfoDialog->show();
        graphInfoDialog->raise();
        graphInfoDialog->activateWindow();
}

void MainWindow::randomGraph()
{
    newGraph();

    int i(0);

    while (i < 8) {
        i = qrand() % 50;
    }

    while (i > 0) {
        graphWidget->addNode("");
        --i;
    }

    QSet<QString> vSet = graphWidget->verticesNames;
    QList<QString> list = vSet.toList();
    QString e1, e2, e3, ec;

    ec = "1";

    QList<QString> ranList;

    int v1, v2, v3;

    while(list.size() > 1) {
        v1 = qrand() % list.size();
        e1 = list.takeAt(v1);

        ranList.append(e1);

        v2 = qrand() % list.size();
        e2 = list.takeAt(v2);

        graphWidget->addEdge(e1, e2, ec);

        v3 = qrand() % ranList.size();

        e3 = ranList.at(v3);

        graphWidget->addEdge(e2, e3, ec);

        int v3 = qrand() % ranList.size();

        e3 = ranList.at(v3);

        graphWidget->addEdge(e2, e3, ec);
    }

    while (!list.empty()) {
        e1 = list.takeAt(0);
        e2 = ranList.at(0);
        graphWidget->addEdge(e1, e2, ec);
    }

    statusBar()->showMessage(tr(""));
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About IGV"),
            tr("<h2>Intelligent Graph Visualizer - «IGV»</h2>"
               "<p><img src=\":/images/logo.png\"  width=\"300\" height=\"185\"></p>"
               "<a href=\"http://alexnevsky.com/igv/\">IGV Web-Page</a>"
               "<p>IGV allows you to easily create and edit a graph, automatically places the graph on the plane (!!!), "
               "search for the minimal path (+coloring vertices), search center (+coloring), search vertices degree, "
               "search vertices eccentricity and others.</p>"

               "<p>This is just the first version of the program, do not judge strictly :)</p>"

               "If you have any questions, suggestions or you just want to leave your feedback about the program, "
               "you can always find my contacts on my website <a href = \"http://alexnevsky.com\">http://alexnevsky.com</a>.<br><br>"

               "I hope you find this program for yourself useful.<br><br>"

               "With best wishes,<br>"
               "Alex Nevsky<br>"

               "<br>Copyright (c) 2010 Alex A. Nevsky. GNU General Public License.<br>"));

   statusBar()->showMessage(tr(""));
}

bool MainWindow::loadGraphFile(const QString &fileName)
{
    if (!graph->readFile(fileName)) {
        statusBar()->showMessage(tr("Upload Canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File Uploaded"), 2000);
    return true;
}

bool MainWindow::saveGraphFile(const QString &fileName)
{
    if (!graph->writeFile(fileName)) {
        statusBar()->showMessage(tr("Saving Fanceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("The File Is Saved"), 2000);
    return true;
}

bool MainWindow::saveGraphInfoFile(const QString &fileName)
{
    if (!graph->writeAllFile(fileName)) {
        statusBar()->showMessage(tr("Saving Fanceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("The File Is Saved"), 2000);
    return true;
}

void MainWindow::createActions()
{
    newGraphAction = new QAction(tr("&New"), this);
    newGraphAction->setIcon(QIcon(":/images/new.png"));
    newGraphAction->setShortcut(tr("Ctrl+N"));
    newGraphAction->setStatusTip(tr("Creare New Graph"));
    connect(newGraphAction, SIGNAL(triggered()), this, SLOT(newGraph()));

    randomGraphAction = new QAction(tr("&Random Graph"), this);
    randomGraphAction->setIcon(QIcon(":/images/Wizard.png"));
    randomGraphAction->setShortcut(tr("Ctrl+R"));
    randomGraphAction->setStatusTip(tr("Create Graph Of An Arbitrary Configuration"));
    connect(randomGraphAction, SIGNAL(triggered()), this, SLOT(randomGraph()));

    loadGraphAction = new QAction(tr("&Load..."), this);
    loadGraphAction->setIcon(QIcon(":/images/open.png"));
    loadGraphAction->setShortcut(tr("Ctrl+O"));
    loadGraphAction->setStatusTip(tr("Load The Graph Configuration From File"));
    connect(loadGraphAction, SIGNAL(triggered()), this, SLOT(loadGraph()));

    saveGraphAction = new QAction(tr("&Save..."), this);
    saveGraphAction->setIcon(QIcon(":/images/save.png"));
    saveGraphAction->setShortcut(tr("Ctrl+S"));
    saveGraphAction->setStatusTip(tr("Save Graph Configuration To Disk"));
    connect(saveGraphAction, SIGNAL(triggered()), this, SLOT(saveGraph()));

    saveGraphInfoAction = new QAction(tr("Save As &HTML..."), this);
    saveGraphInfoAction->setIcon(QIcon(":/images/ArrowDownRight.png"));
    saveGraphInfoAction->setShortcut(tr("Ctrl+H"));
    saveGraphInfoAction->setStatusTip(tr("Save Graph Information To Disk"));
    connect(saveGraphInfoAction, SIGNAL(triggered()), this, SLOT(saveGraphInfo()));

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcuts(QKeySequence::Quit);
    quitAction->setIcon(QIcon(":/images/Logout.png"));
    //quitAction->setStatusTip(tr("Quit The Program"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    graphInfoAction = new QAction(tr("&Graph Theory"), this);
    graphInfoAction->setIcon(QIcon(":/images/Nuke.png"));
    graphInfoAction->setShortcut(tr("Ctrl+G"));
    graphInfoAction->setStatusTip(tr("Help On Graph Theory"));
    connect(graphInfoAction, SIGNAL(triggered()), this, SLOT(graphInfo()));

    aboutAction = new QAction(tr("&About IGV"), this);
    aboutAction->setIcon(QIcon(":/images/Star.png"));
    aboutAction->setShortcut(tr("Ctrl+A"));
    aboutAction->setStatusTip(tr("Show Information About The Program"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Graph"));
    fileMenu->addAction(newGraphAction);
    fileMenu->addAction(randomGraphAction);
    fileMenu->addAction(loadGraphAction);
    fileMenu->addAction(saveGraphAction);
    fileMenu->addAction(saveGraphInfoAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(graphInfoAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Done!"));
}

void MainWindow::createDockWindows()
{
//left tool box begin
    QDockWidget *dock = new QDockWidget(tr("Quick Access"), this);

    //dock->setStyleSheet("QWidget { font-weight: ; color: ; background-color: gold; } ");

    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    toolBoxLeft = new QToolBox(dock);

    //toolBoxLeft->setMinimumSize(310,500);
    toolBoxLeft->setMinimumSize(270,500);

//editGraphPage
    editGraphPage = new QWidget();

    //editGraphPage->setStyleSheet("QWidget { font-weight: ; color: ; background-color: #B0C4DE; } ");
    //editGraphPage->setStyleSheet("QWidget { font-weight: ; color: ; background-color: normal; } ");

    QVBoxLayout *editGraphPageLayour = new QVBoxLayout;

    vertexBox = new QGroupBox(tr("Vertices"));
    QVBoxLayout *vertexBoxLayour = new QVBoxLayout;

    addVertexButton = new QPushButton(tr("       Add Vertex"));
    addVertexButton->setToolTip(tr("Add A Vertex In The Graph"));
    verticesListButton = new QPushButton(tr("       Vertices List"));
    verticesListButton->setToolTip(tr("See A List Of All Vertices"));

    vertexBoxLayour->addWidget(addVertexButton);
    vertexBoxLayour->addWidget(verticesListButton);

    QPixmap targetPix(":/images/Target.png");
    addVertexButton->setIcon(targetPix);
    addVertexButton->setIconSize(targetPix.size());

    verticesListButton->setIcon(targetPix);
    verticesListButton->setIconSize(targetPix.size());

    vertexBox->setLayout(vertexBoxLayour);

    editGraphPageLayour->addWidget(vertexBox);

    edgeBox = new QGroupBox(tr("Edges"));
    QVBoxLayout *edgeBoxLayour = new QVBoxLayout;

    addEdgeButton = new QPushButton(tr("       Add Edge"));
    addEdgeButton->setToolTip(tr("Add An Edge Between Existing Vertices"));

    addEdgeButton->setIcon(targetPix);
    addEdgeButton->setIconSize(targetPix.size());

    edgesListButton = new QPushButton(tr("       Edges List"));
    edgesListButton->setToolTip(tr("See A List Of All Edges And Editing"));

    edgeBoxLayour->addWidget(addEdgeButton);
    edgeBoxLayour->addWidget(edgesListButton);

    edgesListButton->setIcon(targetPix);
    edgesListButton->setIconSize(targetPix.size());

    edgeBox->setLayout(edgeBoxLayour);

    editGraphPageLayour->addWidget(edgeBox);

    lineOneEditGraphPage = new QFrame;
    editGraphPageLayour->addWidget(lineOneEditGraphPage);


    settingEdgeBox = new QGroupBox(tr("Customize The Behavior Of Edges"));
    QVBoxLayout *settingEdgeBoxLayour = new QVBoxLayout;

    freezeEdgeButton = new QRadioButton(tr("Freeze"));
    freezeEdgeButton->setToolTip(tr("Graph Does Not Move"));
    pullEdgeButton = new QRadioButton(tr("Stretch"));
    pullEdgeButton->setToolTip(tr("Graph Tend To Occupy The Whole Scene"));
    strainEdgeButton = new QRadioButton(tr("Maintain Tension"));
    strainEdgeButton->setToolTip(tr("Graph Seek To Group The Vertices"));
    strainEdgeButton->setChecked(true);
    editEdgeButton = new QPushButton(tr("Adding Edges Mode"));
    editEdgeButton->setToolTip(tr("Mode Of Adding An Edge Between Two Vertices"));
    editEdgeButton->setCheckable(true);
    editEdgeButton->setChecked(false);

    settingEdgeBoxLayour->addWidget(freezeEdgeButton);
    settingEdgeBoxLayour->addWidget(pullEdgeButton);
    settingEdgeBoxLayour->addWidget(strainEdgeButton);
    settingEdgeBoxLayour->addWidget(editEdgeButton);

    settingEdgeBox->setLayout(settingEdgeBoxLayour);

    editGraphPageLayour->addWidget(settingEdgeBox);

    editGraphPageLayour->addStretch();

    editGraphPage->setLayout(editGraphPageLayour);
//

//calculateGraphPage
    calculateGraphPage = new QWidget();

    QVBoxLayout *calculateGraphPageLayour = new QVBoxLayout;

    QPixmap burnPix(":/images/Burn.png");
    calculateButton = new QPushButton(tr("          Calculate"));
    calculateButton->setIcon(burnPix);
    calculateButton->setIconSize(burnPix.size());
    calculateButton->setToolTip(tr("Identify The Characteristics Of The Graph. It May Take Some Time..."));

    QPixmap directionDiagPix(":/images/DirectionDiag1.png");
    shortestPathButton = new QPushButton(tr("     Shortest Path"));
    shortestPathButton->setToolTip(tr("Find The Shortest Path Between Two Vertices"));
    shortestPathButton->setIcon(directionDiagPix);
    shortestPathButton->setIconSize(directionDiagPix.size());

    QPixmap directionHorzPix(":/images/DirectionHorz.png");
    distanceButton = new QPushButton(tr("  The Distance Between The Vertices"));
    distanceButton->setToolTip(tr("Find The Distance Between Two Vertices"));
    distanceButton->setIcon(directionHorzPix);
    distanceButton->setIconSize(directionHorzPix.size());

    QPixmap arrow3RightPix(":/images/Arrow3Right.png");

    adjacentVerticesButton = new QPushButton(tr("     Adjacent Vertices"));
    adjacentVerticesButton->setToolTip(tr("Find The Adjacent Vertices To Vertex"));
    adjacentVerticesButton->setIcon(arrow3RightPix);
    adjacentVerticesButton->setIconSize(arrow3RightPix.size());

    nearestVertexButton = new QPushButton(tr("   The Nearest Vertex"));
    nearestVertexButton->setToolTip(tr("Find The Nearest Vertex To Vertex"));
    nearestVertexButton->setIcon(arrow3RightPix);
    nearestVertexButton->setIconSize(arrow3RightPix.size());

    vertexDegreeButton = new QPushButton(tr("          Vertex Degree"));
    vertexDegreeButton->setToolTip(tr("Find The Degree Of The Vertices"));
    vertexDegreeButton->setIcon(arrow3RightPix);
    vertexDegreeButton->setIconSize(arrow3RightPix.size());

    vertexEccentricityButton = new QPushButton(tr("     Vertex Eccentricity"));
    vertexEccentricityButton->setToolTip(tr("Find A Rejected Vertex To All Other Vertices"));
    vertexEccentricityButton->setIcon(arrow3RightPix);
    vertexEccentricityButton->setIconSize(arrow3RightPix.size());

    radiusButton = new QPushButton(tr("            Graph Radius"));
    radiusButton->setToolTip(tr("Find The Radius Of The Graph"));
    radiusButton->setIcon(arrow3RightPix);
    radiusButton->setIconSize(arrow3RightPix.size());

    centerButton = new QPushButton(tr("            Graph Center"));
    centerButton->setToolTip(tr("Find The Center Of The Graph And The Maximum Distance Between The Centers"));
    centerButton->setIcon(arrow3RightPix);
    centerButton->setIconSize(arrow3RightPix.size());

    shortestPathButton->setEnabled(false);
    distanceButton->setEnabled(false);
    nearestVertexButton->setEnabled(false);
    radiusButton->setEnabled(false);
    vertexDegreeButton->setEnabled(false);
    adjacentVerticesButton->setEnabled(false);
    vertexEccentricityButton->setEnabled(false);
    centerButton->setEnabled(false);

    calculateGraphPageLayour->addWidget(calculateButton);
    calculateGraphPageLayour->addWidget(shortestPathButton);
    calculateGraphPageLayour->addWidget(distanceButton);
    calculateGraphPageLayour->addWidget(adjacentVerticesButton);
    calculateGraphPageLayour->addWidget(nearestVertexButton);
    calculateGraphPageLayour->addWidget(vertexDegreeButton);
    calculateGraphPageLayour->addWidget(vertexEccentricityButton);
    calculateGraphPageLayour->addWidget(radiusButton);
    calculateGraphPageLayour->addWidget(centerButton);

    calculateGraphPageLayour->addStretch();

    calculateGraphPage->setLayout(calculateGraphPageLayour);
//

    dock->setWidget(toolBoxLeft);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    QPixmap charNumberPix(":/images/CharNumber.png");
    QPixmap info2Pix(":/images/Info2.png");

    toolBoxLeft->addItem(editGraphPage, charNumberPix, tr("Vertices And Edges"));
    toolBoxLeft->addItem(calculateGraphPage, info2Pix, tr("&Graph Information"));
    toolBoxLeft->setCurrentIndex(0);

    //toolBoxLeft->setStyleSheet("QWidget { font-weight: ; color: ; background-color: lightBlue; }");
    //calculateGraphPage->setStyleSheet("QWidget { font-weight: ; color: ; background-color: normal; } ");

//left tool box end

//right tool box begin
    dock = new QDockWidget(tr("Graph Characteristics"), this);

    //dock->setStyleSheet("QDockWidget { font-weight: ; color: ; background-color: gold; } ");

    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    toolBoxRight = new QToolBox(dock);

    //toolBoxRight->setMinimumSize(250,500);
    toolBoxRight->setMinimumSize(220,500);

    rightGraphPage = new QWidget();

    QVBoxLayout *rightToolBoxLayour = new QVBoxLayout;

    rightGroupBox = new QGroupBox(tr("Graph"));

    QVBoxLayout *rightGroupBoxLayour = new QVBoxLayout;

    rightLabel1 = new QLabel(tr("- Orientation"));
    rightLineEdit1 = new QLineEdit(tr("Undirected"));
    rightLineEdit1->setReadOnly(true);
    rightLabel2 = new QLabel(tr("- Number Of Vertices"));
    rightLineEdit2 = new QLineEdit;
    rightLineEdit2->setReadOnly(true);
    rightLabel3 = new QLabel(tr("- Number Of Edges"));
    rightLineEdit3 = new QLineEdit;
    rightLineEdit3->setReadOnly(true);
    rightLabel4 = new QLabel(tr("- Radius"));
    rightLineEdit4 = new QLineEdit;
    rightLineEdit4->setReadOnly(true);
    rightLabel5 = new QLabel(tr("- Center"));
    rightLineEdit5 = new QLineEdit;
    rightLineEdit5->setReadOnly(true);
    rightLabel6 = new QLabel(tr("- The Maximum Distance\nBetween The Centers"));
    rightLineEdit6 = new QLineEdit;
    rightLineEdit6->setReadOnly(true);

    rightGroupBoxLayour->addWidget(rightLabel1);
    rightGroupBoxLayour->addWidget(rightLineEdit1);
    rightGroupBoxLayour->addWidget(rightLabel2);
    rightGroupBoxLayour->addWidget(rightLineEdit2);
    rightGroupBoxLayour->addWidget(rightLabel3);
    rightGroupBoxLayour->addWidget(rightLineEdit3);
    rightGroupBoxLayour->addWidget(rightLabel4);
    rightGroupBoxLayour->addWidget(rightLineEdit4);
    rightGroupBoxLayour->addWidget(rightLabel5);
    rightGroupBoxLayour->addWidget(rightLineEdit5);
    rightGroupBoxLayour->addWidget(rightLabel6);
    rightGroupBoxLayour->addWidget(rightLineEdit6);

    rightGroupBox->setLayout(rightGroupBoxLayour);

    rightToolBoxLayour->addWidget(rightGroupBox);
    rightToolBoxLayour->addStretch();

    rightGraphPage->setLayout(rightToolBoxLayour);

    dock->setWidget(toolBoxRight);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    QPixmap starEmptyPix(":/images/StarEmpty.png");

    toolBoxRight->addItem(rightGraphPage, starEmptyPix, tr("G - Graph"));

    //toolBoxRight->setStyleSheet("QWidget { font-weight: ; color: ; background-color: lightBlue; }");
    //rightGraphPage->setStyleSheet("QWidget { font-weight: ; color: ; background-color: normal; } ");

//right tool box end
}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("IGV"),
                        tr("Are You Sure?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return saveGraph();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                   .arg(tr("IGV")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::addVertexButtonClicked() {
    if (!addVertexDialog) {
        addVertexDialog = new AddVertexDialog(this);
        connect(addVertexDialog, SIGNAL(addVertex(QString)), this, SLOT(addVertex(QString)));
    }

    addEdgeDialog = 0;
    adjacentVerticesDialog = 0;
    centerDialog = 0;
    distanceDialog = 0;
    edgesListDialog = 0;
    nearestVertexDialog = 0;
    radiusDialog = 0;
    shortestPathDialog = 0;
    vertexDegreeDialog = 0;
    vertexEccentricityDialog = 0;
    verticesListDialog = 0;

    addVertexDialog->show();
    addVertexDialog->raise();
    addVertexDialog->activateWindow();
}

void MainWindow::addVertex(QString str) {
    graphWidget->addNode(str);

    QString s;
    s.setNum(graphWidget->verticesSet.size());
    rightLineEdit2->setText(s);
}

void MainWindow::verticesListButtonClicked() {
    if (!verticesListDialog) {
        verticesListDialog = new VerticesListDialog(this, graphWidget->verticesNames);
    }

    verticesListDialog->show();
    verticesListDialog->raise();
    verticesListDialog->activateWindow();
}

void MainWindow::addEdgeButtonClicked() {
    if (!addEdgeDialog) {
        addEdgeDialog = new AddEdgeDialog(this, graphWidget->verticesNames);
        connect(addEdgeDialog, SIGNAL(addEdge(QString, QString, QString)), this, SLOT(addEdge(QString, QString, QString)));
    }

    addVertexDialog = 0;
    adjacentVerticesDialog = 0;
    centerDialog = 0;
    distanceDialog = 0;
    edgesListDialog = 0;
    nearestVertexDialog = 0;
    radiusDialog = 0;
    shortestPathDialog = 0;
    vertexDegreeDialog = 0;
    vertexEccentricityDialog = 0;
    verticesListDialog = 0;

    addEdgeDialog->show();
    addEdgeDialog->raise();
    addEdgeDialog->activateWindow();
}

void MainWindow::addEdge(QString str, QString str2, QString str3) {
    graphWidget->addEdge(str, str2, str3);
    graphWidget->addEdge(str, str2, str3);

    QString s;
    s.setNum(graphWidget->edgesSet.size());
    rightLineEdit3->setText(s);

    rightLineEdit4->clear();
    rightLineEdit5->clear();
    rightLineEdit6->clear();
}

void MainWindow::edgesListButtonClicked() {
    if (!edgesListDialog) {
        edgesListDialog = new EdgesListDialog(this, graphWidget->edgesSet);
        connect(edgesListDialog, SIGNAL(editGraph()), this, SLOT(editGraph()));
    }

    edgesListDialog->show();
    edgesListDialog->raise();
    edgesListDialog->activateWindow();
}

void MainWindow::editGraph() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (graph) {
        newGraph();
    }

    QString fileName = "sysFiles/sysEditGraph.igv";

    graph->readFile(fileName);
    curFile = fileName;
    emit calculateButtonClicked();

    QFile file("sysFiles/sysShowVertices.igv");
    file.open(QIODevice::ReadOnly);

    QString str;
    str = file.readAll();
    file.close();

    QStringList listNodes = str.split(" ");

    for (QStringList::iterator iter = listNodes.begin(); iter != listNodes.end(); ++iter) {
        graphWidget->addNode(*iter);
     }

    QFile file2("sysFiles/sysShowEdges.igv");
    file2.open(QIODevice::ReadOnly);

    QString str2;
    str2 = file2.readAll();
    file2.close();

    QStringList listEdges = str2.split(" ");
    listEdges.toVector();
    QString s1, s2, s3;

    int i = 0;
    while (i != listEdges.size() - 1) {
        s1 = listEdges[i++];
        s2 = listEdges[i++];
        s3 = listEdges[i++];
        if (i < listEdges.size()) { graphWidget->addEdge(s1, s2, s3); }
    }

    QFile filePos(fileName);
    filePos.open(QIODevice::ReadOnly);

    QString strPos;
    strPos = filePos.readAll();

    filePos.close();

    QFile filep("sysFiles/Pos.igv");
    filep.open(QIODevice::WriteOnly);

    QTextStream outp(&filep);
    outp << strPos;

    filep.close();

    if (1) {
        File file;
        file.Read("sysFiles/Pos.igv");
        File::t_FileData fileData;
        file.GetData(fileData);

        for (File::t_FileData::iterator iter = fileData.begin(); iter != fileData.end(); ++iter) {
            if (*iter == '@') {
                freezeEdgeButtonClicked();
                freezeEdgeButton->setChecked(true);
                fileData.erase(fileData.begin(), iter + 1);
                break;
            }                    
        }

        file.Save(fileData, "sysFiles/Pos.igv");
    }

    if (1) {
        QFile file("sysFiles/Pos.igv");
        file.open(QIODevice::ReadOnly);

        QString str;
        str = file.readAll();

        file.close();

        QStringList listPos = str.split(" ");
        for (QStringList::iterator iter = listPos.begin(); iter < listPos.end() - 1; ++iter) {
            Node *node = graphWidget->vertexMap[*iter];

            ++iter;
            QString s1 = *iter;

            ++iter;
            QString s2 = *iter;

            node->setPos(s1.toDouble(), s2.toDouble());
        }
    }

    std::string s;
    QString qs;

    Node *node;
    std::set<std::string> set = graph->m_Center.vertices;

    if (!set.empty()) {
        s = *set.begin();
        qs += s.c_str();
        node = graphWidget->vertexMap[qs];
        graphWidget->centerNode = node;
    }

    qs.clear();

    QApplication::restoreOverrideCursor();
}

void MainWindow::calculateButtonClicked()
{    
    QApplication::setOverrideCursor(Qt::WaitCursor);

    addVertexDialog = 0;
    addEdgeDialog = 0;
    adjacentVerticesDialog = 0;
    centerDialog = 0;
    distanceDialog = 0;
    edgesListDialog = 0;
    nearestVertexDialog = 0;
    radiusDialog = 0;
    shortestPathDialog = 0;
    vertexDegreeDialog = 0;
    vertexEccentricityDialog = 0;
    verticesListDialog = 0;

    if (graph) {
        delete graph;
        graph = new Graph;
    }

    if (!graphWidget->vertexMap.isEmpty()) {
        QFile fileToWrite("sysFiles/sysNewGraph.igv");

        fileToWrite.open(QIODevice::WriteOnly);
        QTextStream out(&fileToWrite);

        QSetIterator<QString> iter(graphWidget->edgesSet);
        while (iter.hasNext()) {
            out << iter.next();
        }
        fileToWrite.close();

        QFile fileToWrite2("sysFiles/sysSetFromFile.igv");

        fileToWrite2.open(QIODevice::WriteOnly);
        QTextStream out2(&fileToWrite2);

        out2 << "sysFiles/sysNewGraph.igv";
        fileToWrite2.close();
    }

    if (!curFile.isEmpty() || !graphWidget->edgeMap.isEmpty()) {

        File fileToRead;
        fileToRead.Read("sysFiles/sysSetFromFile.igv");
        File::t_FileData fileData;
        fileToRead.GetData(fileData);

        graph->SetFromFile(fileData);

        std::string sysShowVerticesStr("sysFiles/sysShowVertices.igv");
        std::ofstream sysShowVerticesStream;
        sysShowVerticesStream.open(sysShowVerticesStr.c_str());
        graph->ShowVertices(sysShowVerticesStream);

        std::string sysShowEdgesStr("sysFiles/sysShowEdges.igv");
        std::ofstream sysShowEdgesStream;
        sysShowEdgesStream.open(sysShowEdgesStr.c_str());
        graph->ShowEdges(sysShowEdgesStream);        

        shortestPathButton->setEnabled(true);
        distanceButton->setEnabled(true);
        nearestVertexButton->setEnabled(true);
        radiusButton->setEnabled(true);
        vertexDegreeButton->setEnabled(true);
        adjacentVerticesButton->setEnabled(true);
        vertexEccentricityButton->setEnabled(true);
        centerButton->setEnabled(true);
    }

    QString s;

    s.setNum(graph->m_Vertices.size());
    rightLineEdit2->setText(s);

    s.setNum(graph->m_Edges.size() / 2);
    rightLineEdit3->setText(s);

    s.setNum(graph->GetRadius());
    rightLineEdit4->setText(s);

    std::set<std::string> set = graph->m_Center.vertices;

    std::string str;
    QString qs;

    Node *node;

    if (!set.empty()) {
        str = *set.begin();
        qs += str.c_str();
        node = graphWidget->vertexMap[qs];
        graphWidget->centerNode = node;
    }

    qs.clear();

    for (std::set<std::string>::iterator iter = set.begin(); iter != set.end(); ++iter) {
        str = *iter;
        qs += str.c_str();
        graphWidget->centerSet.insert(str.c_str());
        qs += ";";
        qs.append(" ");
    }

    rightLineEdit5->setText(qs);

    double d = graph->m_Center.maxDistanceBetweenCenter;
    s.setNum(d);
    rightLineEdit6->setText(s);

    QApplication::restoreOverrideCursor();
}

void MainWindow::shortestPathButtonClicked() {
    if (!shortestPathDialog) {
            shortestPathDialog = new ShortestPathDialog(this, graphWidget->verticesNames, graphWidget->pathSet, graphWidget->pathWaySet, *graph);
            connect(shortestPathDialog, SIGNAL(findPath()), this, SLOT(findPath()));
        }

        shortestPathDialog->show();
        shortestPathDialog->raise();
        shortestPathDialog->activateWindow();
}

void MainWindow::findPath() {
    QString fileName = "sysFiles/findPath.igv";

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);

    for (QSet<QString>::iterator iter = graphWidget->edgesSet.begin(); iter != graphWidget->edgesSet.end(); ++iter) {
        out << *iter;
    }

    out << "@";

    for (QSet<QString>::iterator iter = graphWidget->verticesNames.begin(); iter != graphWidget->verticesNames.end(); ++iter) {
        Node *node = graphWidget->vertexMap[*iter];
        out << *iter;
        out << " ";
        out << node->pos().x();
        out << " ";
        out << node->pos().y();
        out << " ";
    }

    file.close();

    int iNew = graphWidget->edgesSet.size();

    QSet<QString> cSet = graphWidget->centerSet;
    QSet<QString> pSet = graphWidget->pathSet;
    QSet<QString> pWaySet = graphWidget->pathWaySet;

    if (graph) {
        newGraph();
    }

    graphWidget->centerSet = cSet;
    graphWidget->pathSet = pSet;
    graphWidget->pathWaySet = pWaySet;

    if (!fileName.isEmpty()) {
        setCurrentFile(fileName);

        QApplication::setOverrideCursor(Qt::WaitCursor);

        graph->readFile(fileName);
        curFile = fileName;

        if (iNew < 50) {
            emit calculateButtonClicked();
        }

        QFile file("sysFiles/sysShowVertices.igv");
        file.open(QIODevice::ReadOnly);

        QString str;
        str = file.readAll();
        file.close();

        QStringList listNodes = str.split(" ");

        for (QStringList::iterator iter = listNodes.begin(); iter != listNodes.end(); ++iter) {
            if (*iter != "") {
                graphWidget->addNode(*iter);
            }
        }

        QFile file2("sysFiles/sysShowEdges.igv");
        file2.open(QIODevice::ReadOnly);

        QString str2;
        str2 = file2.readAll();
        file2.close();

        QStringList listEdges = str2.split(" ");
        listEdges.toVector();
        QString s1, s2, s3;

        int i = 0;
        while (i != listEdges.size() - 1) {
            s1 = listEdges[i++];
            s2 = listEdges[i++];
            s3 = listEdges[i++];
            if (i < listEdges.size()) { graphWidget->addEdge(s1, s2, s3); }
        }

        QFile filePos(fileName);
        filePos.open(QIODevice::ReadOnly);

        QString strPos;
        strPos = filePos.readAll();

        filePos.close();

        QFile filep("sysFiles/Pos.igv");
        filep.open(QIODevice::WriteOnly);

        QTextStream outp(&filep);
        outp << strPos;

        filep.close();

        if (1) {
            File file;
            file.Read("sysFiles/Pos.igv");
            File::t_FileData fileData;
            file.GetData(fileData);

            for (File::t_FileData::iterator iter = fileData.begin(); iter != fileData.end(); ++iter) {
                if (*iter == '@') {
                    freezeEdgeButtonClicked();
                    freezeEdgeButton->setChecked(true);
                    fileData.erase(fileData.begin(), iter + 1);
                    break;
                }
            }

            file.Save(fileData, "sysFiles/Pos.igv");
        }

        if (1) {
            QFile file("sysFiles/Pos.igv");
            file.open(QIODevice::ReadOnly);

            QString str;
            str = file.readAll();

            file.close();

            QStringList listPos = str.split(" ");
            for (QStringList::iterator iter = listPos.begin(); iter < listPos.end() - 1; ++iter) {
                Node *node = graphWidget->vertexMap[*iter];

                ++iter;
                QString s1 = *iter;

                ++iter;
                QString s2 = *iter;

                node->setPos(s1.toDouble(), s2.toDouble());
            }
        }

        std::string s;
        QString qs;

        Node *node;
        std::set<std::string> set = graph->m_Center.vertices;

        if (!set.empty()) {
            s = *set.begin();
            qs += s.c_str();
            node = graphWidget->vertexMap[qs];
            graphWidget->centerNode = node;
        }

        qs.clear();

        graphWidget->pathSet.clear();
        graphWidget->pathWaySet.clear();

        QApplication::restoreOverrideCursor();
    }
}

void MainWindow::distanceButtonClicked() {
    if (!distanceDialog) {
            distanceDialog = new DistanceDialog(this, graphWidget->verticesNames, *graph);
        }

        distanceDialog->show();
        distanceDialog->raise();
        distanceDialog->activateWindow();
}

void MainWindow::nearestVertexButtonClicked() {    
    if (!nearestVertexDialog) {
            nearestVertexDialog = new NearestVertexDialog(this, graphWidget->verticesNames, *graph);
        }

        nearestVertexDialog->show();
        nearestVertexDialog->raise();
        nearestVertexDialog->activateWindow();
}

void MainWindow::radiusButtonClicked() {
    if (!radiusDialog) {
            radiusDialog = new RadiusDialog(this, *graph);
        }

        radiusDialog->show();
        radiusDialog->raise();
        radiusDialog->activateWindow();
}

void MainWindow::vertexDegreeButtonClicked() {
    if (!vertexDegreeDialog) {
            vertexDegreeDialog = new VertexDegreeDialog(this, graphWidget->verticesNames, *graph);
        }

        vertexDegreeDialog->show();
        vertexDegreeDialog->raise();
        vertexDegreeDialog->activateWindow();
}

void MainWindow::adjacentVerticesButtonClicked() {
    if (!adjacentVerticesDialog) {
            adjacentVerticesDialog = new AdjacentVerticesDialog(this, graphWidget->verticesNames, *graph);
        }

        adjacentVerticesDialog->show();
        adjacentVerticesDialog->raise();
        adjacentVerticesDialog->activateWindow();
}

void MainWindow::vertexEccentricityButtonClicked() {
    if (!vertexEccentricityDialog) {
            vertexEccentricityDialog = new VertexEccentricityDialog(this, graphWidget->verticesNames, *graph);
        }

        vertexEccentricityDialog->show();
        vertexEccentricityDialog->raise();
        vertexEccentricityDialog->activateWindow();
}

void MainWindow::centerButtonClicked() {
    if (!centerDialog) {
            centerDialog = new CenterDialog(this, *graph);
            connect(centerDialog, SIGNAL(findCenter()), this, SLOT(findCenter()));
        }

        centerDialog->show();
        centerDialog->raise();
        centerDialog->activateWindow();
}

void MainWindow::findCenter() {
    QString fileName = "sysFiles/findCenter.igv";

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);

    for (QSet<QString>::iterator iter = graphWidget->edgesSet.begin(); iter != graphWidget->edgesSet.end(); ++iter) {
        out << *iter;
    }

    out << "@";

    for (QSet<QString>::iterator iter = graphWidget->verticesNames.begin(); iter != graphWidget->verticesNames.end(); ++iter) {
        Node *node = graphWidget->vertexMap[*iter];
        out << *iter;
        out << " ";
        out << node->pos().x();
        out << " ";
        out << node->pos().y();
        out << " ";
    }

    file.close();

    int iNew = graphWidget->edgesSet.size();

    QSet<QString> cSet = graphWidget->centerSet;

    if (graph) {
        newGraph();
    }

    graphWidget->centerSet = cSet;

    if (!fileName.isEmpty()) {
        setCurrentFile(fileName);

        QApplication::setOverrideCursor(Qt::WaitCursor);

        graph->readFile(fileName);
        curFile = fileName;

        if (iNew < 50) {
            emit calculateButtonClicked();
        }

        QFile file("sysFiles/sysShowVertices.igv");
        file.open(QIODevice::ReadOnly);

        QString str;
        str = file.readAll();
        file.close();

        QStringList listNodes = str.split(" ");

        for (QStringList::iterator iter = listNodes.begin(); iter != listNodes.end(); ++iter) {
            if (*iter != "") {
                graphWidget->addNode(*iter);
            }
        }

        QFile file2("sysFiles/sysShowEdges.igv");
        file2.open(QIODevice::ReadOnly);

        QString str2;
        str2 = file2.readAll();
        file2.close();

        QStringList listEdges = str2.split(" ");
        listEdges.toVector();
        QString s1, s2, s3;

        int i = 0;
        while (i != listEdges.size() - 1) {
            s1 = listEdges[i++];
            s2 = listEdges[i++];
            s3 = listEdges[i++];
            if (i < listEdges.size()) { graphWidget->addEdge(s1, s2, s3); }
        }

        QFile filePos(fileName);
        filePos.open(QIODevice::ReadOnly);

        QString strPos;
        strPos = filePos.readAll();

        filePos.close();

        QFile filep("sysFiles/Pos.igv");
        filep.open(QIODevice::WriteOnly);

        QTextStream outp(&filep);
        outp << strPos;

        filep.close();

        if (1) {
            File file;
            file.Read("sysFiles/Pos.igv");
            File::t_FileData fileData;
            file.GetData(fileData);

            for (File::t_FileData::iterator iter = fileData.begin(); iter != fileData.end(); ++iter) {
                if (*iter == '@') {
                    freezeEdgeButtonClicked();
                    freezeEdgeButton->setChecked(true);
                    fileData.erase(fileData.begin(), iter + 1);
                    break;
                }
            }

            file.Save(fileData, "sysFiles/Pos.igv");
        }

        if (1) {
            QFile file("sysFiles/Pos.igv");
            file.open(QIODevice::ReadOnly);

            QString str;
            str = file.readAll();

            file.close();

            QStringList listPos = str.split(" ");
            for (QStringList::iterator iter = listPos.begin(); iter < listPos.end() - 1; ++iter) {
                Node *node = graphWidget->vertexMap[*iter];

                ++iter;
                QString s1 = *iter;

                ++iter;
                QString s2 = *iter;

                node->setPos(s1.toDouble(), s2.toDouble());
            }
        }

        std::string s;
        QString qs;

        Node *node;
        std::set<std::string> set = graph->m_Center.vertices;

        if (!set.empty()) {
            s = *set.begin();
            qs += s.c_str();
            node = graphWidget->vertexMap[qs];
            graphWidget->centerNode = node;
        }

        qs.clear();

        //graphWidget->centerSet.clear();

        QApplication::restoreOverrideCursor();
    }
}

void MainWindow::freezeEdgeButtonClicked() {
    for (QSet<Node*>::iterator iter = graphWidget->verticesSet.begin(); iter != graphWidget->verticesSet.end(); ++iter) {
        Node *node = *iter;
        node->edgesSetting = 0;
    }
}

void MainWindow::pullEdgeButtonClicked() {
    for (QSet<Node*>::iterator iter = graphWidget->verticesSet.begin(); iter != graphWidget->verticesSet.end(); ++iter) {
        Node *node = *iter;
        node->edgesSetting = 2;
    }
}

void MainWindow::strainEdgeButtonClicked() {
    for (QSet<Node*>::iterator iter = graphWidget->verticesSet.begin(); iter != graphWidget->verticesSet.end(); ++iter) {
        Node *node = *iter;
        node->edgesSetting = 1;
    }
}

void MainWindow::editEdgeButtonToggled() {
    graphWidget->edgeEditing = editEdgeButton->isChecked();
}
