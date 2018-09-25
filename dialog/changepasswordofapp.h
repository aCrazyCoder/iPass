#ifndef CHANGEPASSWORDOFAPP_H
#define CHANGEPASSWORDOFAPP_H

#include <QWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include "global/global.h"

namespace Ui {
class changePasswordOfApp;
}

class changePasswordOfApp : public QWidget
{
    Q_OBJECT

public:
    explicit changePasswordOfApp(QWidget *parent = 0);
    ~changePasswordOfApp();

    bool isShown;
    void closeEvent(QCloseEvent *event)override;

    bool temp_automaticLockApp;

private:
    Ui::changePasswordOfApp *ui;
};

#endif // CHANGEPASSWORDOFAPP_H
