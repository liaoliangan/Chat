#include <QApplication>
#include <QDebug>
#include"logindialog.h"
#include "mainwindow.h"
#include <QFile>
#include"global.h"
#include "HttpMgr.h"
#include <QtGlobal>
void myMessageHandler(QtMsgType type,
                      const QMessageLogContext &ctx,
                      const QString &msg)
{
    Q_UNUSED(ctx)   // 如需文件/行号，可再格式化
    switch (type) {
    case QtDebugMsg:
    case QtInfoMsg:
        fprintf(stdout, "%s\n", msg.toLocal8Bit().constData());
        fflush(stdout);
        break;
    case QtWarningMsg:
    case QtCriticalMsg:
    case QtFatalMsg:
        fprintf(stderr, "%s\n", msg.toLocal8Bit().constData());
        fflush(stderr);
        break;
    }
}

int main(int argc, char* argv[])
{
    qInstallMessageHandler(myMessageHandler); // 接管 Qt 日志
    QApplication a(argc, argv);
    QFile style(":/style/LA.qss");
    if (style.open(QIODevice::ReadOnly))
    {
        QString styleSheet = QLatin1String(style.readAll());
        a.setStyleSheet(styleSheet);
        COUT << "load style sheet" << ENDL;
        style.close();
    }

    QString fileName = "config.ini";
    QString app_path = QCoreApplication::applicationDirPath(); //exe在build目录下，但ini下项目目录
    QDir project_path(app_path);
    project_path.cdUp();
    //QDir::separator是分隔符，windows下是\\,Unix/Linux下是/
    QString config_path = QDir::toNativeSeparators(project_path.absolutePath() + QDir::separator() + fileName);
    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;

    COUT << gate_url_prefix.toStdString() << ENDL;

    MainWindow w;
    w.show();
    return QApplication::exec();
}
