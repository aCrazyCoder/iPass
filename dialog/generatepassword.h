#ifndef GENERATEPASSWORD_H
#define GENERATEPASSWORD_H

#include <QWidget>
#include <QCryptographicHash>
#include <QClipboard>
#include <QMessageBox>
#include <QTime>
#include <QCloseEvent>
#include "global/global.h"

namespace Ui {
class generatePassWord;
}

class generatePassWord : public QWidget
{
    Q_OBJECT

public:
    explicit generatePassWord(QWidget *parent = 0);
    ~generatePassWord();

    bool isShown;
    void closeEvent(QCloseEvent *event);

private:
    Ui::generatePassWord *ui;

    QString otherCharacter;

public slots:
    void generate_PassWord();
    void displayPassword(QString sha);
    void copyPassword();
};

#endif // GENERATEPASSWORD_H
