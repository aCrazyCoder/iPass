#include "generatepassword.h"
#include "ui_generatepassword.h"

generatePassWord::generatePassWord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::generatePassWord)
{
    isShown = false;

    ui->setupUi(this);

    ui->lineEdit->setPlaceholderText(QString::fromLocal8Bit("汉字、字母、数字"));
    QPixmap pix(":/image/copy.png");
    ui->pushButton_2->setIcon(pix.scaled(ui->pushButton_2->size(), Qt::KeepAspectRatio));
    ui->pushButton_2->setStyleSheet("QPushButton:pressed {background-color:rgb(179,234,249);}"
                                    "QPushButton{background-color: rgb(255, 255, 255);border: none;}");
    ui->checkBox->setChecked(true);
    otherCharacter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_-=+~,.<>/?;:'[]{}\\|";


    setWindowIcon(QIcon(":/image/Password.ico"));
    setWindowTitle(QString::fromLocal8Bit("生成密码"));
    setWindowFlags(Qt::Window);
    setWindowModality(Qt::WindowModal);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(generate_PassWord()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(copyPassword()));
    connect(ui->checkBox, &QCheckBox::stateChanged, [=](int state){
                                                                       if(state == Qt::Checked)
                                                                           otherCharacter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_-=+~,.<>/?;:'[]{}\\|";
                                                                       else if(state == Qt::Unchecked)
                                                                           otherCharacter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                                                                       else
                                                                           return;
                                                                  });
}

generatePassWord::~generatePassWord()
{
    delete ui;
}

void generatePassWord::closeEvent(QCloseEvent *event)
{
    ui->lineEdit->setText("");
    ui->lineEdit->setPlaceholderText(QString::fromLocal8Bit("汉字、字母、数字"));
    ui->comboBox->setCurrentIndex(0);
    ui->checkBox->setChecked(true);
    otherCharacter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_-=+~,.<>/?;:'[]{}\\|";
    ui->lineEdit_2->setText("");

    event->accept();
    isShown = false;
}

void generatePassWord::generate_PassWord()
{
    if(ui->lineEdit->text().isEmpty())
        QMessageBox::warning(this, QString::fromLocal8Bit("输入警告"), QString::fromLocal8Bit("请输入字符串"));
    else
    {
        /*
        *step1：生成MD5
        */
        QByteArray byte_array;
        byte_array.append(ui->lineEdit->text());
        QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
        QString md5 = hash_byte_array.toHex();

        /*
        *step2：生成8个不等随机数
        */
        int temp[8];
        int i,j;
        QList<int> numbersList;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        for(i=0;i<8;i++)
        {
            numbersList.append(qrand()%32);
            bool flag=true;
            while(flag)
            {
                for(j=0;j<i;j++)
                {
                    if(numbersList[i]==numbersList[j])
                    {
                        break;
                    }
                }
                if(j<i)
                {
                    numbersList[i]=rand()%32;
                }
                if(j==i)
                {
                    flag=!flag;
                }
            }
        }
        for(i=0;i<8;i++)
        {
            temp[i] = numbersList[i];
        }

        /*
        *step3：根据随机数生成新的字符串
        */
        QString temp_str;
        for(int i = 0; i<8; i++)
            temp_str.append(md5[temp[i]]);

        /*
        *step4：对新的字符串进行SHA3-256加密
        */
        QByteArray byte_array_new;
        byte_array_new.append(temp_str);
        QByteArray hash_byte_array_new = QCryptographicHash::hash(byte_array_new, QCryptographicHash::Sha3_256);
        QString sha3_256 = hash_byte_array_new.toHex();

        /*
        *step5：根据用户需要显示密码
        */
        displayPassword(sha3_256);
    }

}

void generatePassWord::displayPassword(QString sha)
{
    /*
    *step1：确定用户需要密码位数
    */
    int numOfPassword;
    switch(ui->comboBox->currentIndex())
    {
        case 0:
        numOfPassword = 6;
        break;
        case 1:
        numOfPassword = 8;
        break;
        case 2:
        numOfPassword = 10;
        break;
        case 3:
        numOfPassword = 12;
        break;
        case 4:
        numOfPassword = 14;
        break;
        default:
        numOfPassword = 0;
        break;
    }

    /*
    *step2：生成不等随机数
    */
    int i,j;
    QList<int> numbersList;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(i=0;i<numOfPassword;i++)
    {
        numbersList.append(qrand()%64);
        bool flag=true;
        while(flag)
        {
            for(j=0;j<i;j++)
            {
                if(numbersList[i]==numbersList[j])
                {
                    break;
                }
            }
            if(j<i)
            {
                numbersList[i]=rand()%64;
            }
            if(j==i)
            {
                flag=!flag;
            }
        }
    }

    /*
    *step3：生成其它字符
    */
    QList<int> numbersList_other;
    for(i=0;i<14;i++)
    {
        numbersList_other.append(qrand()%otherCharacter.length());
        bool flag=true;
        while(flag)
        {
            for(j=0;j<i;j++)
            {
                if(numbersList_other[i]==numbersList_other[j])
                {
                    break;
                }
            }
            if(j<i)
            {
                numbersList_other[i]=rand()%otherCharacter.length();
            }
            if(j==i)
            {
                flag=!flag;
            }
        }
    }

    /*
    *step4：生成并显示对应密码
    */
    QString new_passwod;
    for(int i = 0; i<numbersList.length(); i++)
    {
        if(numbersList[i]%3 == 0)
            new_passwod.append(otherCharacter[numbersList_other[i]]);
        else
            new_passwod.append(sha[numbersList[i]]);
    }
    ui->lineEdit_2->setText(new_passwod);

}

void generatePassWord::copyPassword()
{
    QClipboard *board = QApplication::clipboard();
    board->setText(ui->lineEdit_2->text());
    if(clearClipboard == true)
        QTimer::singleShot(1000 * clearTime, board, [=](){board->setText("");});
}
