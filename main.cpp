#include "mainwindow.h"
#include <QApplication>
#include <QMutex>
#include "other/myeventfilter.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

    QFile file(currentDirOfApp + "/log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
    *程序当前工作目录
    */
    currentDirOfApp = a.applicationDirPath();

    /*
    *输出log日志
    */
    qInstallMessageHandler(myMessageOutput);
    qDebug()<<"This is a log file";

    /*
    *显示锁屏或者主界面
    */
    MainWindow w;
    if(!mainPasswordOfApp.isEmpty())
        w.lock_app();
    else
        w.show();

    /*
    *插入事件处理器
    */
    MyEventFilter m_filter;
    a.installEventFilter(&m_filter);

    return a.exec();
}

