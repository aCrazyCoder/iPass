#include "stacklayout.h"
#include "ui_stacklayout.h"

stackLayout::stackLayout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stackLayout)
{
    ui->setupUi(this);

    ui->frame->setStyleSheet("#frame{background-color:rgb(255,255,255);border:1px solid blue;border-radius: 35px;}");

    QPixmap pix1(":/image/edit.png");
    ui->edit_Btn->setIcon(pix1.scaled(ui->edit_Btn->size(), Qt::KeepAspectRatio));
    ui->edit_Btn->setStyleSheet("QPushButton:pressed{background-color:rgb(179,234,249);}"
                                "QPushButton{background-color: rgb(240, 240, 240);border: none;}");

    QPixmap pix2(":/image/Trashcan.png");
    ui->delete_Btn->setIcon(pix2.scaled(ui->delete_Btn->size(), Qt::KeepAspectRatio));
    ui->delete_Btn->setStyleSheet("QPushButton:pressed{background-color:rgb(179,234,249);}"
                                  "QPushButton{background-color: rgb(240, 240, 240);border: none;}");

    QPixmap pix3(":/image/copy.png");
    ui->copy_account->setIcon(pix3.scaled(ui->copy_account->size(), Qt::KeepAspectRatio));
    ui->copy_account->setStyleSheet("QPushButton:pressed{background-color:rgb(179,234,249);}"
                                    "QPushButton{background-color: rgb(255, 255, 255);border: none;}");
    ui->copy_password->setIcon(pix3.scaled(ui->copy_account->size(), Qt::KeepAspectRatio));
    ui->copy_password->setStyleSheet("QPushButton:pressed{background-color:rgb(179,234,249);}"
                                     "QPushButton{background-color: rgb(255, 255, 255);border: none;}");

    QPixmap pix4(":/image/see_new.png");
    ui->view_password->setIcon(pix4.scaled(ui->view_password->size(), Qt::KeepAspectRatio));
    ui->view_password->setStyleSheet("QPushButton:pressed{background-color:rgb(179,234,249);}"
                                     "QPushButton{background-color: rgb(255, 255, 255);border: none;}");

    connect(ui->copy_account, &QPushButton::clicked, [=](){QClipboard *clipboard = QApplication::clipboard();
                                                           clipboard->setText(ui->account_frame->text());
                                                           if(clearClipboard == true)
                                                               QTimer::singleShot(1000 * clearTime, clipboard, [=](){clipboard->setText("");});});
    connect(ui->copy_password, &QPushButton::clicked,[=](){QClipboard *clipboard = QApplication::clipboard();
                                                           clipboard->setText(ui->password_frame->text());
                                                           if(clearClipboard == true)
                                                               QTimer::singleShot(1000 * clearTime, clipboard, [=](){clipboard->setText("");});});
    connect(ui->view_password, &QPushButton::clicked,[=](){
                                                           if(view_list.at(indexOfList)%2 == 0)
                                                           {
                                                               ui->password_frame->setPixmap(QPixmap());
                                                               ui->password_frame->setText(password_list.at(indexOfList));
                                                           }
                                                           else
                                                           {
                                                               QPixmap pix5(":/image/heidian.png");
                                                               ui->password_frame->text().clear();
                                                               ui->password_frame->setPixmap(pix5);
                                                           }
                                                           int i = view_list.at(indexOfList) + 1;
                                                           view_list.replace(indexOfList, i);});
    connect(ui->edit_Btn, &QPushButton::clicked, [=](){emit editInformationOfAccount();});
    connect(ui->delete_Btn, &QPushButton::clicked, [=](){isDelete = true; emit deleteAccount();});

}

stackLayout::~stackLayout()
{
    delete ui;
}

void stackLayout::updateInformation(int index)
{
    indexOfList = index;

    ui->account_Lab->setText(title_list.at(index));
    ui->account_frame->setText(account_list.at(index));
    ui->password_frame->setText(password_list.at(index));
    ui->strength_frame->setText(strength_list.at(index));
    ui->textEdit->setText(note_list.at(index));

    if(isMaskPassword == true)
    {
        QPixmap pix5(":/image/heidian.png");
        ui->password_frame->text().clear();
        ui->password_frame->setPixmap(pix5);
    }

    view_list.replace(index, 0);
}
