#include "changepasswordofapp.h"
#include "ui_changepasswordofapp.h"

changePasswordOfApp::changePasswordOfApp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::changePasswordOfApp)
{
    isShown = false;

    ui->setupUi(this);
    ui->pushButton->setIcon(QIcon(":/image/see.png"));
    ui->pushButton_2->setIcon(QIcon(":/image/see.png"));
    ui->pushButton_3->setIcon(QIcon(":/image/see.png"));

    ui->lineEdit->setEchoMode(QLineEdit::Password);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);

    setWindowIcon(QIcon(":/image/Password.ico"));
    setWindowTitle(QString::fromLocal8Bit("更改密码"));
    setWindowFlags(Qt::Window);
    setWindowModality(Qt::WindowModal);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);

    connect(ui->lineEdit, &QLineEdit::editingFinished, [=](){if(mainPasswordOfApp != ui->lineEdit->text())
            {
                if(!ui->lineEdit->text().isEmpty())
                    QMessageBox::warning(this, "iPass", QString::fromLocal8Bit("输入原密码错误，请重新输入！"));
                ui->lineEdit->setText("");
                ui->lineEdit->setFocus();
            }
    });
    connect(ui->buttonBox, &QDialogButtonBox::accepted, [=](){if((ui->lineEdit_2->text() == ui->lineEdit_3->text()) && (mainPasswordOfApp == ui->lineEdit->text()))
        {
            mainPasswordOfApp = ui->lineEdit_2->text();
            QMessageBox::information(this, "iPass", QString::fromLocal8Bit("iPass主密码更改成功!"));
            close();
        }
        else
            QMessageBox::warning(this, "iPass", QString::fromLocal8Bit("两次新密码输入不同，请重新确认！"));
    });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &changePasswordOfApp::close);
    connect(ui->pushButton, &QPushButton::clicked, [=](){
        static int i = 0;
        if(i%2 == 0)
            ui->lineEdit->setEchoMode(QLineEdit::Normal);
        else
            ui->lineEdit->setEchoMode(QLineEdit::Password);
        i++;
    });
    connect(ui->pushButton_2, &QPushButton::clicked, [=](){
        static int i = 0;
        if(i%2 == 0)
            ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
        else
            ui->lineEdit_2->setEchoMode(QLineEdit::Password);
        i++;
    });
    connect(ui->pushButton_3, &QPushButton::clicked, [=](){
        static int i = 0;
        if(i%2 == 0)
            ui->lineEdit_3->setEchoMode(QLineEdit::Normal);
        else
            ui->lineEdit_3->setEchoMode(QLineEdit::Password);
        i++;
    });
}

changePasswordOfApp::~changePasswordOfApp()
{
    delete ui;
}

void changePasswordOfApp::closeEvent(QCloseEvent *event)
{
    event->accept();
    isShown = false;
    automaticLockApp = temp_automaticLockApp;
    timer->start(1000 * timeOfLockInterval);
}
