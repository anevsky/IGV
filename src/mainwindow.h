/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMenu;
class QToolBox;
class QWidget;
class QPushButton;
class QGroupBox;
class QRadioButton;
class QFrame;
class QTextBrowser;
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

class GraphWidget;
class Graph;
class AddVertexDialog;
class AddEdgeDialog;
class AdjacentVerticesDialog;
class CenterDialog;
class DistanceDialog;
class EdgesListDialog;
class NearestVertexDialog;
class RadiusDialog;
class ShortestPathDialog;
class VertexDegreeDialog;
class VertexEccentricityDialog;
class VerticesListDialog;
class GraphInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    GraphWidget *graphWidget;
    Graph *graph;

protected:

private slots:
    void newGraph();
    void loadGraph();
    bool saveGraph();
    bool saveGraphInfo();

    void addVertexButtonClicked();
    void addEdgeButtonClicked();

    void verticesListButtonClicked();
    void edgesListButtonClicked();

    void freezeEdgeButtonClicked();
    void pullEdgeButtonClicked();
    void strainEdgeButtonClicked();
    void editEdgeButtonToggled();

    void calculateButtonClicked();
    void shortestPathButtonClicked();
    void distanceButtonClicked();
    void nearestVertexButtonClicked();
    void radiusButtonClicked();
    void vertexDegreeButtonClicked();
    void adjacentVerticesButtonClicked();
    void vertexEccentricityButtonClicked();
    void centerButtonClicked();

    void addVertex(QString str);
    void addEdge(QString str, QString str2, QString str3);
    void findPath();
    void findCenter();
    void editGraph();
    void randomGraph();
    void graphInfo();

    void about();

private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void createDockWindows();

    bool okToContinue();
    bool loadGraphFile(const QString &fileName);
    bool saveGraphFile(const QString &fileName);
    bool saveGraphInfoFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    QString strippedName(const QString &fullFileName);
    QString curFile;    

    AddVertexDialog *addVertexDialog;
    AddEdgeDialog *addEdgeDialog;

    AdjacentVerticesDialog *adjacentVerticesDialog;
    CenterDialog *centerDialog;
    DistanceDialog *distanceDialog;
    EdgesListDialog *edgesListDialog;
    NearestVertexDialog *nearestVertexDialog;
    RadiusDialog *radiusDialog;
    ShortestPathDialog *shortestPathDialog;
    VertexDegreeDialog *vertexDegreeDialog;
    VertexEccentricityDialog *vertexEccentricityDialog;
    VerticesListDialog *verticesListDialog;
    GraphInfo *graphInfoDialog;

    QToolBox *toolBoxLeft;

    QWidget *editGraphPage;
    QWidget *calculateGraphPage;
    QWidget *rightGraphPage;

    QGroupBox *vertexBox;
    QPushButton *addVertexButton;
    QPushButton *verticesListButton;

    QGroupBox *edgeBox;
    QPushButton *addEdgeButton;
    QPushButton *edgesListButton;

    QGroupBox *settingEdgeBox;
    QRadioButton *freezeEdgeButton;
    QRadioButton *pullEdgeButton;
    QRadioButton *strainEdgeButton;
    QPushButton *editEdgeButton;

    QFrame *lineOneEditGraphPage;

    QPushButton *calculateButton;
    QPushButton *shortestPathButton;
    QPushButton *distanceButton;
    QPushButton *nearestVertexButton;
    QPushButton *radiusButton;
    QPushButton *vertexDegreeButton;
    QPushButton *adjacentVerticesButton;
    QPushButton *vertexEccentricityButton;
    QPushButton *centerButton;

    QToolBox *toolBoxRight;    
    QGroupBox *rightGroupBox;
    QLabel *rightLabel1;
    QLineEdit *rightLineEdit1;
    QLabel *rightLabel2;
    QLineEdit *rightLineEdit2;
    QLabel *rightLabel3;
    QLineEdit *rightLineEdit3;
    QLabel *rightLabel4;
    QLineEdit *rightLineEdit4;
    QLabel *rightLabel5;
    QLineEdit *rightLineEdit5;
    QLabel *rightLabel6;
    QLineEdit *rightLineEdit6;
    
    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *newGraphAction;
    QAction *loadGraphAction;
    QAction *saveGraphAction;
    QAction *saveGraphInfoAction;
    QAction *graphInfoAction;
    QAction *randomGraphAction;
    QAction *aboutAction;
    QAction *quitAction;
};

#endif
