#include <QApplication>
#include <QDebug>
#include"logindialog.h"
#include "mainwindow.h"
#include <QFile>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QFile style(":/style/LA.qss");
    if (style.open(QIODevice::ReadOnly))
    {
        QString styleSheet = QLatin1String(style.readAll());
        a.setStyleSheet(styleSheet);
        style.close();
    }
    MainWindow w;
    w.show();
    return QApplication::exec();
}
