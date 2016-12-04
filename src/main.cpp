/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include <QApplication>
#include <QtGui>
#include <QtCore>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QSplashScreen *splash = new QSplashScreen;
    //splash->setPixmap(QPixmap(":/images/logo.png"));
    //splash->show();

    //Qt::Alignment topRight = Qt::AlignRight | Qt::AlignLeft;

    //splash->showMessage(QObject::tr("Loading..."), topRight, Qt::white);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);

    a.setStyleSheet("QPushButton[text=\"OK\"] { color: green; } ");
    a.setStyleSheet("QPushButton[text=\"Cancel\"] { color: red; } ");
    a.setStyleSheet("QPushButton:hover { color: darkMagenta; } ");

    MainWindow w;

    w.setWindowIcon(QIcon(":/images/icon.png"));

    w.show();

    //splash->finish(&w);
    //delete splash;

    return a.exec();
}
