#include "userguide.h"
#include "ui_userguide.h"

userGuide::userGuide(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userGuide)
{
    isShown = false;

    ui->setupUi(this);

    setWindowIcon(QIcon(":/image/Password.ico"));
    setWindowTitle(QString::fromLocal8Bit("使用说明"));
    setWindowFlags(Qt::Window);
    setWindowModality(Qt::WindowModal);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);
}

userGuide::~userGuide()
{
    delete ui;
}

void userGuide::closeEvent(QCloseEvent *event)
{
    event->accept();
    isShown = false;
}
