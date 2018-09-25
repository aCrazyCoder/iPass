#include "lockofapp.h"
#include "ui_lockofapp.h"

lockOfApp::lockOfApp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lockOfApp)
{
    ui->setupUi(this);

    ui->lock_unlock->setPixmap(QPixmap(":/image/lock_closed.png"));

    ui->inputPassword->setPlaceholderText(QString::fromLocal8Bit("主密码"));
    ui->inputPassword->setEchoMode(QLineEdit::Password);
    ui->inputPassword->setAlignment(Qt::AlignCenter);
    ui->inputPassword->setFocus();

    setWindowIcon(QIcon(":/image/Password.ico"));
    setWindowTitle(QString::fromLocal8Bit("iPass"));
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);

    connect(ui->inputPassword, &QLineEdit::editingFinished, [=](){flag = 0; checkPassword();});
    connect(ui->unlock, &QPushButton::clicked, [=](){flag = 1; checkPassword();});

}

lockOfApp::~lockOfApp()
{
    delete ui;
}

void lockOfApp::DelayMsecSuspend(int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < _Timer )
    QApplication::processEvents(QEventLoop::AllEvents, 100);
}

void lockOfApp::onShakeWindow()
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "pos");
    pAnimation->setDuration(500);
    pAnimation->setLoopCount(2);
    pAnimation->setKeyValueAt(0, QPoint(pos().x(), pos().y()));
    pAnimation->setKeyValueAt(0.1, QPoint(pos().x() + 3, pos().y() + 3));
    pAnimation->setKeyValueAt(0.2, QPoint(pos().x() - 3, pos().y() + 3));
    pAnimation->setKeyValueAt(0.3, QPoint(pos().x() + 3, pos().y() - 3));
    pAnimation->setKeyValueAt(0.4, QPoint(pos().x() - 3, pos().y() - 3));
    pAnimation->setKeyValueAt(0.5, QPoint(pos().x() + 3, pos().y() + 3));
    pAnimation->setKeyValueAt(0.6, QPoint(pos().x() - 3, pos().y() + 3));
    pAnimation->setKeyValueAt(0.7, QPoint(pos().x() + 3, pos().y() - 3));
    pAnimation->setKeyValueAt(0.8, QPoint(pos().x() - 3, pos().y() - 3));
    pAnimation->setKeyValueAt(0.9, QPoint(pos().x() + 3, pos().y() + 3));
    pAnimation->setKeyValueAt(1, QPoint(pos().x() - 3, pos().y() -3));
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void lockOfApp::checkPassword()
{
    if(mainPasswordOfApp == ui->inputPassword->text())
    {
        //首先检查焦点是否仍处于输入框内
        if((ui->inputPassword->hasFocus()) | (flag == 1))
        {
            QSound::play(":/other/unlock.wav");
            ui->lock_unlock->clear();
            ui->lock_unlock->setPixmap(QPixmap(":/image/lock_open.png"));
            DelayMsecSuspend(500);
            close();
            ui->inputPassword->setText("");
            ui->lock_unlock->clear();
            ui->lock_unlock->setPixmap(QPixmap(":/image/lock_closed.png"));
            emit correctPassword();
            isLock = false;
        }
        else
            return;
    }
    else
        onShakeWindow();
}
