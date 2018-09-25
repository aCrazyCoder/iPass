#ifndef ADDNEWACCOUNT_H
#define ADDNEWACCOUNT_H

#include <QWidget>
#include <QMessageBox>
#include <QCloseEvent>

#include "global/global.h"

namespace Ui {
class addNewAccount;
}

class addNewAccount : public QWidget
{
    Q_OBJECT

public:
    explicit addNewAccount(QWidget *parent = 0);
    ~addNewAccount();

    bool isShown;
    void closeEvent(QCloseEvent *event)override;

private:
    Ui::addNewAccount *ui;

    void checkStrengthOfPassword(QString password);
    QString strength;

    /*
    *点击view次数
    */
    int viewCount;

public slots:
    void saveAccountToList();
    void changeAccount();

signals:
    void saveAccountToListFinished();
    void editAccountFinished();
};

#endif // ADDNEWACCOUNT_H
