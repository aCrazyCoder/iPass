#include "addnewaccount.h"
#include "ui_addnewaccount.h"

addNewAccount::addNewAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addNewAccount)
{
    isShown = false;
    viewCount = 0;

    ui->setupUi(this);

    ui->view->setIcon(QIcon(":/image/view.png"));
    QPixmap pix(":/image/account.png");
    ui->pic->setPixmap(pix.scaled(ui->pic->size(), Qt::KeepAspectRatio));
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setMaxLength(14);
    ui->lineEdit_3->setToolTip(QString::fromLocal8Bit("最多支持14位密码"));
    ui->textEdit->setPlaceholderText(QString::fromLocal8Bit("记录备注信息，例如：密保问题，预留手机号等"));

    setTabOrder(ui->lineEdit, ui->lineEdit_2);
    setTabOrder(ui->lineEdit_2, ui->lineEdit_3);
    setTabOrder(ui->lineEdit_3, ui->view);
    setTabOrder(ui->view, ui->textEdit);

    setWindowIcon(QIcon(":/image/Password.ico"));
    setWindowTitle(QString::fromLocal8Bit("添加账号密码"));
    setWindowFlags(Qt::Window | windowFlags()&~Qt::WindowMinMaxButtonsHint);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &addNewAccount::saveAccountToList);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &addNewAccount::close);
    connect(ui->view, &QPushButton::clicked, [=](){
                                                   if(viewCount%2 == 0)
                                                   ui->lineEdit_3->setEchoMode(QLineEdit::Normal);
                                                   else
                                                   ui->lineEdit_3->setEchoMode(QLineEdit::Password);
                                                   viewCount++;});
}

addNewAccount::~addNewAccount()
{
    delete ui;
}

void addNewAccount::closeEvent(QCloseEvent *event)
{
    event->accept();
    isShown = false;
}

void addNewAccount::checkStrengthOfPassword(QString password)
{
    int temp = 0;
    for(int i = 0; i<password.length(); i++)
    {
        if(password.at(i).isNumber())
            temp += 1;
        else if(password.at(i).isLower())
            temp += 2;
        else if(password.at(i).isUpper())
            temp += 3;
        else
            temp += 4;
    }
    if(temp <= 15)
        strength = QString::fromLocal8Bit("较弱");
    else if(temp > 15 && temp <25)
        strength = QString::fromLocal8Bit("一般");
    else if((temp >= 25) | (password.length() >= 10))
        strength = QString::fromLocal8Bit("较强");

}

void addNewAccount::saveAccountToList()
{
    if(ui->lineEdit->text().isEmpty() | ui->lineEdit_2->text().isEmpty() | ui->lineEdit_3->text().isEmpty())
        QMessageBox::warning(this, QString::fromLocal8Bit("保存信息"), QString::fromLocal8Bit("输入信息不完善，保存失败"));
    else
    {
        if(isEdit)
        {

            title_list.replace(indexOfList, ui->lineEdit->text());
            account_list.replace(indexOfList, ui->lineEdit_2->text());
            password_list.replace(indexOfList, ui->lineEdit_3->text());
            note_list.replace(indexOfList, ui->textEdit->toPlainText());

            checkStrengthOfPassword(ui->lineEdit_3->text());
            strength_list.replace(indexOfList, strength);

            ui->lineEdit->setText("");
            ui->lineEdit_2->setText("");
            ui->lineEdit_3->setText("");
            ui->textEdit->setText("");

            close();

            viewCount = 0;

            emit editAccountFinished();

            isEdit = false;
        }
        else
        {
            title_list.append(ui->lineEdit->text());
            account_list.append(ui->lineEdit_2->text());
            password_list.append(ui->lineEdit_3->text());
            note_list.append(ui->textEdit->toPlainText());

            checkStrengthOfPassword(ui->lineEdit_3->text());
            strength_list.append(strength);

            view_list.append(0);

            totalNumberOfAccount++;

            ui->lineEdit->setText("");
            ui->lineEdit_2->setText("");
            ui->lineEdit_3->setText("");
            ui->textEdit->setText("");

            close();

            viewCount = 0;

            emit saveAccountToListFinished();
        }
    }
}

void addNewAccount::changeAccount()
{
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);

    ui->lineEdit->setText(title_list.at(indexOfList));
    ui->lineEdit_2->setText(account_list.at(indexOfList));
    ui->lineEdit_3->setText(password_list.at(indexOfList));
    ui->textEdit->setText(note_list.at(indexOfList));

    isEdit = true;
}
