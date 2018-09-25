#include "passwordofapp.h"
#include "ui_passwordofapp.h"

passwordOfApp::passwordOfApp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::passwordOfApp)
{
    ui->setupUi(this);

    ui->lineEdit->setAlignment(Qt::AlignCenter);
    ui->lineEdit->setEchoMode(QLineEdit::Password);
    ui->lineEdit->setPlaceholderText(QString::fromLocal8Bit("数字、字母、特殊符号"));
    ui->lineEdit->setFocus();
    ui->lineEdit->setAlignment(Qt::AlignCenter);
    ui->lineEdit->setMaxLength(20);

    ui->lineEdit_2->setAlignment(Qt::AlignCenter);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_2->setPlaceholderText(QString::fromLocal8Bit("再次输入以确定"));
    ui->lineEdit_2->setAlignment(Qt::AlignCenter);
    ui->lineEdit_2->setMaxLength(20);

    ui->textEdit->setReadOnly(true);

    ui->textBrowser->setOpenExternalLinks(true);

    ui->pushButton->setStyleSheet("QPushButton{border: 1px solid rgb(64,203,239);border-radius:10px;}"
                                  "QPushButton#pushButton:pressed{background-color:rgb(255, 255, 255);border-style:inset;padding-left:3px;padding-top:6px;}");

    ui->pushButton_2->setStyleSheet("QPushButton{border: 1px solid rgb(64,203,239);border-radius:10px;}"
                                    "QPushButton#pushButton_2:pressed{background-color:rgb(255, 255, 255);border-style:inset;padding-left:3px;padding-top:6px;}");

    setTabOrder(ui->lineEdit, ui->lineEdit_2);

    setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    setWindowModality(Qt::WindowModal);

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(monitorPassword()));
    connect(ui->pushButton_2, &QPushButton::clicked, [=](){QMessageBox::warning(this, "iPass", QString::fromLocal8Bit("您必须设置主密码，否则您的账户存在安全隐患"));});

}

passwordOfApp::~passwordOfApp()
{
    delete ui;
}

void passwordOfApp::monitorPassword()
{
    if(ui->lineEdit->text().isEmpty() | ui->lineEdit_2->text().isEmpty())
        QMessageBox::warning(this, QString("iPass"), QString::fromLocal8Bit("iPass主密码不能为空，请重新输入"));
    else
    {
        if(ui->lineEdit->text() == ui->lineEdit_2->text())
        {
            mainPasswordOfApp = ui->lineEdit->text();

            /*
            *取主密码前八个字符转换为16进制后，组成一个64位的uint密钥
            */
            if(mainPasswordOfApp.length() >= 8)
            {
                for(int i = 0;i < 8; i++)
                    partOfMainPassword[i] = mainPasswordOfApp[i].toLatin1();
            }
            else
                for(int i = 0;i < mainPasswordOfApp.length(); i++)
                    partOfMainPassword[i] = mainPasswordOfApp[i].toLatin1();

            bool ok;
            enKey = partOfMainPassword.toHex().toULongLong(&ok,16);

            close();
            emit mainPasswordFinished();
        }
        else
        {
            QMessageBox::warning(this, QString("password"), QString::fromLocal8Bit("两次密码输入不一样，请重新输入"));
        }
    }
}
