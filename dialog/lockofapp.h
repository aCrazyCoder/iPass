#ifndef LOCKOFAPP_H
#define LOCKOFAPP_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QtTest/QTest>
#include <QSound>

#include "global/global.h"

namespace Ui {
class lockOfApp;
}

class lockOfApp : public QWidget
{
    Q_OBJECT

public:
    explicit lockOfApp(QWidget *parent = 0);
    ~lockOfApp();
    void DelayMsecSuspend(int msec);

private:
    Ui::lockOfApp *ui;

    /*
    *窗口抖动
    */
    void onShakeWindow();

    /*
    *区分编辑框和按钮的标志位
    */
    int flag;

public slots:
    void checkPassword();

signals:
    void correctPassword();
};

#endif // LOCKOFAPP_H
