#ifndef SETTINGSOFAPP_H
#define SETTINGSOFAPP_H

#include <QWidget>
#include <QDialog>
#include <QHeaderView>
#include <QScrollBar>
#include <QCloseEvent>
#include <QDebug>

#include "global/global.h"
#include "dialog/changepasswordofapp.h"

namespace Ui {
class settingsOfApp;
}

class settingsOfApp : public QWidget
{
    Q_OBJECT

public:
    explicit settingsOfApp(QWidget *parent = 0);
    ~settingsOfApp();

    bool isShown;
    void closeEvent(QCloseEvent *event)override;

public slots:
    void insertRowOfTable(QString time, QString file, QString type, QString number);


private:
    Ui::settingsOfApp *ui;

    void saveSettingVariable();

signals:
    void settingFinished();
};

#endif // SETTINGSOFAPP_H
