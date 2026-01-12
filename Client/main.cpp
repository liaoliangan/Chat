#include <QApplication>
#include <QDebug>
#include"logindialog.h"
#include "mainwindow.h"
#include <QFile>
#include"global.h"
#include "HttpMgr.h"

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

    QString fileName = "config.ini";
    QString app_path = QCoreApplication::applicationDirPath(); //exe在build目录下，但ini下项目目录
    QDir project_path(app_path);
    project_path.cdUp();
    //QDir::separator是分隔符，windows下是\\,Unix/Linux下是/
    QString config_path = QDir::toNativeSeparators(project_path.absolutePath()  + QDir::separator() + fileName);
    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;

    COUT << gate_url_prefix.toStdString() << ENDL;

    MainWindow w;
    w.show();
    return QApplication::exec();
}
