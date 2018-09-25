#include "contact.h"
#include "ui_contact.h"

contact::contact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contact)
{
    isShown = false;

    ui->setupUi(this);

    ui->lineEdit_2->setAlignment(Qt::AlignCenter);
    ui->lineEdit_3->setAlignment(Qt::AlignCenter);
    ui->lineEdit_4->setAlignment(Qt::AlignCenter);

    setWindowIcon(QIcon(":/image/Password.ico"));
    setWindowTitle(QString::fromLocal8Bit("联系我们"));
    setWindowFlags(Qt::Window);
    setWindowModality(Qt::WindowModal);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);
}

contact::~contact()
{
    delete ui;
}

void contact::closeEvent(QCloseEvent *event)
{
    event->accept();
    isShown = false;
}
