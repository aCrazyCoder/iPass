#ifndef PASSWORDOFAPP_H
#define PASSWORDOFAPP_H

#include <QWidget>
#include <QMessageBox>
#include <QDesktopWidget>
#include "global/global.h"

namespace Ui {
class passwordOfApp;
}

class passwordOfApp : public QWidget
{
    Q_OBJECT

public:
    explicit passwordOfApp(QWidget *parent = 0);
    ~passwordOfApp();

private:
    Ui::passwordOfApp *ui;

public slots:
    void monitorPassword();

signals:
    void mainPasswordFinished();
    void mainPasswordNotFinished();

};

#endif // PASSWORDOFAPP_H
