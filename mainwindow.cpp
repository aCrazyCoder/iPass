#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /*
    *读取注册表保存的设置数据
    */
    readSettings();

    /*
    *初始化变量
    */
    mainpassword = new passwordOfApp(this);
    m_newAccount = new addNewAccount(this);
    m_stack = new QStackedWidget;
    m_list = new QListWidget;
    stack = new stackLayout;
    mlock = new lockOfApp;
    user = new userGuide(this);
    setting = new settingsOfApp(this);
    newpassword = new generatePassWord(this);
    m_contact = new contact(this);

    /*
    *初始化界面
    */
    initTool();
    initStackLayout();
    initInterface();
    openAppShow();

    /*
    *设置提醒信息
    */
    if(mainPasswordOfApp.isEmpty())
        QTimer::singleShot(2000, this, SLOT(remindMainPassword()));

    /*
    *设置鼠标跟踪，以检测应用闲置状态，每到一定时间就自动上锁
    */
    setMouseTracking(true);
    connect(timer, &QTimer::timeout, [=](){if((isLock == false) && (automaticLockApp == true) && (!mainPasswordOfApp.isEmpty())) lock_app();});
    timer->start(1000 * timeOfLockInterval);
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSyncData();
    saveData();
    writeSettings();
    event->accept();
}

void MainWindow::create_newAccount()
{
    m_newAccount->show();
    m_newAccount->isShown = true;
}

void MainWindow::import_file()
{
    if(enKey == 0)
        QMessageBox::warning(this, "iPass", QString::fromLocal8Bit("iPass尚未设置主密码，无法完成导入！"));
    else
    {
        QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("导入备份文件"), ".", tr("iPass Document(*.ips)"));
        QFile file;
        file.setFileName(path);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            SimpleCrypt decrypt(enKey);
            QStringList result;
            QTextStream in(&file);
            int lineNumber = 0;
            while(!in.atEnd())
            {
                QString line = decrypt.decryptToString(in.readLine());
                qDebug()<<line;

                lineNumber++;
                if(lineNumber == 1)
                {
                    if(line != "iPass")
                    {
                        QMessageBox::warning(this, "iPass", QString::fromLocal8Bit("密码匹配错误，该文件不是有效私有iPass备份文件，或该文件已被修改！"));
                        return;
                    }
                }
                else if(lineNumber == 2)
                {
                    bool ok;
                    totalNumberOfAccount = line.toInt(&ok);
                }
                else
                    result.append(line);
            }
            if((totalNumberOfAccount == 0) | (result.length() <= 5) | (result.at(0) != "title") | (result.at(totalNumberOfAccount+1) != "account") | (result.at(2*totalNumberOfAccount+2) != "password") | (result.at(3*totalNumberOfAccount+3) != "note") | (result.at(4*totalNumberOfAccount+4) != "strengh"))
                QMessageBox::warning(this, "iPass", QString::fromLocal8Bit("文件内容为空，或文件内容格式错误！"));
            else
            {
                title_list.clear();
                account_list.clear();
                password_list.clear();
                note_list.clear();
                strength_list.clear();
                view_list.clear();
                for(int i = 0; i < totalNumberOfAccount; i++)
                {
                    title_list.append(result.at(1+totalNumberOfAccount*0+i));
                    account_list.append(result.at(2+totalNumberOfAccount*1+i));
                    password_list.append(result.at(3+totalNumberOfAccount*2+i));
                    note_list.append(result.at(4+totalNumberOfAccount*3+i));
                    strength_list.append(result.at(5+totalNumberOfAccount*4+i));
                }
                showImportInformation();
            }
            emit importAndExportInformation(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"), path, QString("in"), QString::number(totalNumberOfAccount));

        }
    }
}

void MainWindow::export_file()
{
    if(mainPasswordOfApp.isEmpty())
        QMessageBox::information(this, "iPass", QString::fromLocal8Bit("您尚未设置iPass主密码，无法完成数据导出"));
    else
    {
        SimpleCrypt crypto(enKey);
        QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd").append("__backup__").append(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).section("/",-1,-1)).append(".ips");
        QDir folder;
        QFile file;
        QString fileUrl;

        bool pathCheck = true;
        if(!folder.exists(currentDirOfApp))
        {
            pathCheck = folder.mkpath(currentDirOfApp);
        }

        if(pathCheck)
        {
            fileUrl = currentDirOfApp + "/" + fileName;

            file.setFileName(fileUrl);
            file.open(QIODevice::ReadWrite);

            if(file.isOpen())
            {
                QTextStream out(&file);

                if(file.size() > 0)
                {
                    if(file.resize(0))
                    {
                        out << crypto.encryptToString(QString("iPass")) <<endl;
                        out << crypto.encryptToString(QString::number(totalNumberOfAccount)) << endl;
                        out << crypto.encryptToString(QString("title")) << endl;
                        for(int i = 0; i < title_list.length(); i++)
                        {
                            out << crypto.encryptToString(QString(title_list.at(i))) <<endl;
                        }
                        out << crypto.encryptToString(QString("account")) << endl;
                        for(int i = 0; i < account_list.length(); i++)
                        {
                            out << crypto.encryptToString(QString(account_list.at(i))) <<endl;
                        }
                        out << crypto.encryptToString(QString("password")) << endl;
                        for(int i = 0; i < password_list.length(); i++)
                        {
                            out << crypto.encryptToString(QString(password_list.at(i))) <<endl;
                        }
                        out << crypto.encryptToString(QString("note")) << endl;
                        for(int i = 0; i < note_list.length(); i++)
                        {
                            out << crypto.encryptToString(QString(note_list.at(i))) <<endl;
                        }
                        out << crypto.encryptToString(QString("strengh")) << endl;
                        for(int i = 0; i < strength_list.length(); i++)
                        {
                            out << crypto.encryptToString(QString(strength_list.at(i))) <<endl;
                        }

                        out.flush();
                    }
                }
                else
                {
                    out << crypto.encryptToString(QString("iPass")) <<endl;
                    out << crypto.encryptToString(QString::number(totalNumberOfAccount)) << endl;
                    out << crypto.encryptToString(QString("title")) << endl;
                    for(int i = 0; i < title_list.length(); i++)
                    {
                        out << crypto.encryptToString(QString(title_list.at(i))) <<endl;
                    }
                    out << crypto.encryptToString(QString("account")) << endl;
                    for(int i = 0; i < account_list.length(); i++)
                    {
                        out << crypto.encryptToString(QString(account_list.at(i))) <<endl;
                    }
                    out << crypto.encryptToString(QString("password")) << endl;
                    for(int i = 0; i < password_list.length(); i++)
                    {
                        out << crypto.encryptToString(QString(password_list.at(i))) <<endl;
                    }
                    out << crypto.encryptToString(QString("note")) << endl;
                    for(int i = 0; i < note_list.length(); i++)
                    {
                        out << crypto.encryptToString(QString(note_list.at(i))) <<endl;
                    }
                    out << crypto.encryptToString(QString("strengh")) << endl;
                    for(int i = 0; i < strength_list.length(); i++)
                    {
                        out << crypto.encryptToString(QString(strength_list.at(i))) <<endl;
                    }

                    out.flush();
                }
                emit importAndExportInformation(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"), fileUrl, QString("out"), QString::number(totalNumberOfAccount));
                lastSyncTime = QString::fromLocal8Bit("最近一次备份时间：").append(QDateTime::currentDateTime().toString("yyyy-MM-dd  hh:mm:ss"));
                lastSyncInformation->setText(lastSyncTime);
            }
            file.close();
        }
    QMessageBox::information(this, "iPass", QString::fromLocal8Bit("<font size='+1'>导出数据成功。</font>").append(QString::fromLocal8Bit("<br>文件路径：")).append(currentDirOfApp));
    }

}

void MainWindow::hide_password()
{
    if(isMaskPassword == true)
    {
        hidePassword->setIcon(QIcon(":/image/switch_off.png"));
        isMaskPassword = false;
    }
    else
    {
        hidePassword->setIcon(QIcon(":/image/switch_on.png"));
        isMaskPassword = true;
    }

    /*
    *及时刷新一下stack对应内容
    */
    if(totalNumberOfAccount > 0)
        emit updateStackInformation(indexOfList);
}

void MainWindow::lock_app()
{
    hideAll();
    mlock->show();
    isLock = true;
}

void MainWindow::generate_password()
{
    newpassword->isShown = true;
    newpassword->show();
}

void MainWindow::set_app()
{
    setting->isShown = true;
    setting->show();

    /*
    *及时刷新一下当前stack对应内容
    */
    connect(setting, &settingsOfApp::settingFinished, [=](){if(isMaskPassword == true)
                                                               hidePassword->setIcon(QIcon(":/image/switch_on.png"));
                                                            else
                                                               hidePassword->setIcon(QIcon(":/image/switch_off.png"));
                                                            if(totalNumberOfAccount > 0)
                                                                emit updateStackInformation(indexOfList);});
}

void MainWindow::open_userGuide()
{
    user->show();
    user->isShown = true;
}

void MainWindow::contact_us()
{
    m_contact->isShown = true;
    m_contact->show();
}

void MainWindow::open_aboutApp()
{
    QMessageBox::about(this,QString::fromLocal8Bit("关于软件"),QString::fromLocal8Bit("<h2>iPass ver1.0</h2>"
                                                                                  "<p>Copyright &copy; 2018 iPass Inc."
                                                                                  "<p>iPass是一个密码保护软件，提供各种密码备份以及生成密码功能，"
                                                                                  "PC端与Android端可跨平台备份，安全可靠。"));
}

void MainWindow::update_time()
{
    QDateTime CurrentTime = QDateTime::currentDateTime();
    QString Timestr = CurrentTime.toString("  yyyy-MM-dd  hh:mm:ss  "); // 设置显示的格式
    currentTimeLable->setText(Timestr);
}

void MainWindow::remindMainPassword()
{
    if(mainPasswordOfApp.isEmpty())
    {
        QMessageBox* box = new QMessageBox(QMessageBox::Warning, QString("iPass"), QString::fromLocal8Bit("您尚未设置iPass主密码，为保护您的账户安全，点击确定立即设置"));
        box->setStandardButtons(QMessageBox::Ok);
        box->setButtonText(QMessageBox::Ok, QString::fromLocal8Bit("确定"));
        connect(box, &QMessageBox::buttonClicked, [=](){hide();mainpassword->show();});
        box->setWindowIcon(QIcon(":/image/Password.ico"));
        box->exec();
    }
}

void MainWindow::insertNewAccountToStack()
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QIcon(":/image/account.png"));
    item->setText(title_list.at(totalNumberOfAccount-1));
    item->setFont(QFont(QString::fromLocal8Bit("微软雅黑"),14,5));
    item->setTextColor(QColor(100,139,178));
    item->setSizeHint(QSize(100,60));
    m_list->insertItem(totalNumberOfAccount-1, item);
    m_list->setCurrentRow(totalNumberOfAccount-1);

    stack->show();
    emit updateStackInformation(totalNumberOfAccount-1);
    m_stack->insertWidget(totalNumberOfAccount-1, stack);
}

void MainWindow::editAccountFinished()
{
    m_list->currentItem()->setText(title_list.at(indexOfList));
    emit updateStackInformation(indexOfList);
}

void MainWindow::showStackInformation(int index)
{
    /*
    *如果是删除信息的话，则不发射update信号
    *不加此判断条件，会出现神奇bug，索引值自动加1，导致后面索引全部错误
    */
    if(isDelete == false)
    {
        emit updateStackInformation(index);
        m_stack->setCurrentIndex(index);
    }

}

void MainWindow::changeInformationOfAccount()
{
    m_newAccount->show();
    emit editStackInformation();
}

void MainWindow::initInterface()
{
    setWindowTitle(QString::fromLocal8Bit("iPass"));
    setWindowIcon(QIcon(":/image/Password.ico"));
    setMinimumSize(800,600);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);

    currentTimeLable = new QLabel;
    currentTime = new QTimer(this);
    currentTime->start(1000);
    statusBar()->addWidget(currentTimeLable);
    QLabel *m_widget = new QLabel;
    m_widget->setMinimumWidth(300);
    statusBar()->addWidget(m_widget);
    lastSyncInformation = new QLabel(lastSyncTime);
    statusBar()->addWidget(lastSyncInformation);

    spilterLine = new QFrame();
    spilterLine->setFixedHeight(1);
    spilterLine->setMinimumSize(width(), 1);
    spilterLine->setStyleSheet("QFrame{""color:rgb(185,185,185);""}");
    spilterLine->setFrameShape(QFrame::HLine);
    spilterLine->setFrameShadow(QFrame::Plain);

    mainWidget = new QWidget(this);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_frame);
    mainLayout->addWidget(spilterLine);
    mainLayout->addLayout(stack_Layout, 5);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);
    createActions();
    createMenu();
    createConnections();
}

void MainWindow::initTool()
{
    m_frame = new QFrame;
    m_frame->setMinimumWidth(width());
    m_frame->setFixedHeight(40);

    lockApp_Btn = new QPushButton();
    lockApp_Btn->setStyleSheet("QPushButton{border:none;background-color:rgb(240,240,240);}"
                               "QPushButton:hover{background:rgb(179,234,249);}");
    lockApp_Btn->setFixedSize(30,30);
    lockApp_Btn->setIcon(QIcon(":/image/lock.png"));
    if(mainPasswordOfApp.isEmpty())
        lockApp_Btn->setEnabled(false);

    createNewAccount_Btn = new QPushButton();
    createNewAccount_Btn->setStyleSheet("QPushButton{border:none;background-color:rgb(240,240,240);}"
                                        "QPushButton:hover{background:rgb(179,234,249);}");
    createNewAccount_Btn->setFixedSize(30,30);
    createNewAccount_Btn->setIcon(QIcon(":/image/plus.png"));

    searchLeft = new QLabel;
    searchLeft->setStyleSheet("QLabel{"
                              "border-image:url(:image/input-left.png);"
                              "}");
    searchLeft->setFixedSize(17,30);
    searchRight = new QLabel;
    searchRight->setStyleSheet("QLabel{"
                               "border-image:url(:image/input-right.png);"
                               "}");
    searchRight->setFixedSize(17,30);

    m_search = new QLineEdit;
    m_search->setStyleSheet("border:none;");
    m_search->setFixedSize(150,30);
    m_search->setPlaceholderText(QString::fromLocal8Bit("搜索"));
    m_search->setFocus();

    search_Btn = new QPushButton();
    search_Btn->setStyleSheet("QPushButton{"
                               "border:none;background-color:white;"
                               "}");
    search_Btn->setFixedSize(22,30);
    search_Btn->setIcon(QIcon(":/image/Search.png"));
    search_Btn->setShortcut(QKeySequence("Enter"));

    expandFrame = new QFrame;
    expandFrame->setMinimumSize(330, 30);
    expandFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    frame_Layout = new QHBoxLayout;
    frame_Layout->addWidget(lockApp_Btn);
    frame_Layout->addSpacing(5);
    frame_Layout->addWidget(createNewAccount_Btn);
    frame_Layout->addWidget(expandFrame);
    frame_Layout->addWidget(searchLeft);
    frame_Layout->addWidget(m_search);
    frame_Layout->addWidget(search_Btn);
    frame_Layout->addWidget(searchRight);
    frame_Layout->setMargin(5);
    frame_Layout->setSpacing(0);

    m_frame->setLayout(frame_Layout);

}

void MainWindow::initStackLayout()
{

    m_list->setStyleSheet("QFrame{"
                          "border:none;"
                          "}");
    m_list->setIconSize(QSize(50,50));


    stackSpilterLine = new QFrame;
    stackSpilterLine->setFixedWidth(1);
    stackSpilterLine->setMinimumSize(1, m_list->height());
    stackSpilterLine->setStyleSheet("QFrame{""color:rgb(185,185,185);""}");
    stackSpilterLine->setFrameShape(QFrame::VLine);
    stackSpilterLine->setFrameShadow(QFrame::Plain);

    stack_Layout = new QHBoxLayout;
    stack_Layout->addWidget(m_list);
    stack_Layout->addWidget(stackSpilterLine);
    stack_Layout->addWidget(m_stack);
    stack_Layout->setStretchFactor(m_list, 2);
    stack_Layout->setStretchFactor(m_stack, 4);

}

void MainWindow::saveData()
{
    SimpleCrypt crypto(enKey);
    QString fileName = QString("temp").append(".ipss");
    QDir folder;
    QFile file;
    QString fileUrl;

    if(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).isEmpty())
    {
        fileUrl = currentDirOfApp + "/" + fileName;
    }
    else
    {
        if(!folder.exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)))
        {
            folder.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        }
        fileUrl = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + fileName;
    }

    file.setFileName(fileUrl);
    file.open(QIODevice::ReadWrite);

    if(file.isOpen())
    {
        QTextStream out(&file);

        if(file.size() > 0)
        {
            if(file.resize(0))
            {
                out << crypto.encryptToString(QString("iPass")) <<endl;
                out << crypto.encryptToString(QString::number(totalNumberOfAccount)) << endl;
                out << crypto.encryptToString(QString("title")) << endl;
                for(int i = 0; i < title_list.length(); i++)
                {
                    out << crypto.encryptToString(QString(title_list.at(i))) <<endl;
                }
                out << crypto.encryptToString(QString("account")) << endl;
                for(int i = 0; i < account_list.length(); i++)
                {
                    out << crypto.encryptToString(QString(account_list.at(i))) <<endl;
                }
                out << crypto.encryptToString(QString("password")) << endl;
                for(int i = 0; i < password_list.length(); i++)
                {
                    out << crypto.encryptToString(QString(password_list.at(i))) <<endl;
                }
                out << crypto.encryptToString(QString("note")) << endl;
                for(int i = 0; i < note_list.length(); i++)
                {
                    out << crypto.encryptToString(QString(note_list.at(i))) <<endl;
                }
                out << crypto.encryptToString(QString("strengh")) << endl;
                for(int i = 0; i < strength_list.length(); i++)
                {
                    out << crypto.encryptToString(QString(strength_list.at(i))) <<endl;
                }

                out.flush();
            }
        }
        else
        {
            out << crypto.encryptToString(QString("iPass")) <<endl;
            out << crypto.encryptToString(QString::number(totalNumberOfAccount)) << endl;
            out << crypto.encryptToString(QString("title")) << endl;
            for(int i = 0; i < title_list.length(); i++)
            {
                out << crypto.encryptToString(QString(title_list.at(i))) <<endl;
            }
            out << crypto.encryptToString(QString("account")) << endl;
            for(int i = 0; i < account_list.length(); i++)
            {
                out << crypto.encryptToString(QString(account_list.at(i))) <<endl;
            }
            out << crypto.encryptToString(QString("password")) << endl;
            for(int i = 0; i < password_list.length(); i++)
            {
                out << crypto.encryptToString(QString(password_list.at(i))) <<endl;
            }
            out << crypto.encryptToString(QString("note")) << endl;
            for(int i = 0; i < note_list.length(); i++)
            {
                out << crypto.encryptToString(QString(note_list.at(i))) <<endl;
            }
            out << crypto.encryptToString(QString("strengh")) << endl;
            for(int i = 0; i < strength_list.length(); i++)
            {
                out << crypto.encryptToString(QString(strength_list.at(i))) <<endl;
            }

            out.flush();
        }
    }
    file.close();
}

void MainWindow::saveSyncData()
{
    QString fileName = QString("syncInfo").append(".txt");
    QDir folder;
    QFile file;
    QString fileUrl;

    if(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).isEmpty())
    {
        fileUrl = currentDirOfApp + "/" + fileName;
    }
    else
    {
        if(!folder.exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)))
        {
            folder.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        }
        fileUrl = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + fileName;
    }

    file.setFileName(fileUrl);
    file.open(QIODevice::ReadWrite);
    if(file.isOpen())
    {
        QTextStream out(&file);
        if(file.size() > 0)
        {
            if(file.resize(0))
            {
                for(int i = 0; i < syncInfoOfFile.length(); i++)
                {
                    out << syncInfoOfTime.at(i) << endl;
                    out << syncInfoOfFile.at(i) << endl;
                    out << syncInfoOfType.at(i) << endl;
                    out << syncInfoOfNumber.at(i) << endl;
                }
                out.flush();
            }
        }
        else
        {
            for(int i = 0; i < syncInfoOfFile.length(); i++)
            {
                out << syncInfoOfTime.at(i) << endl;
                out << syncInfoOfFile.at(i) << endl;
                out << syncInfoOfType.at(i) << endl;
                out << syncInfoOfNumber.at(i) << endl;
            }
            out.flush();
        }
    }
    file.close();

}

void MainWindow::openAppShow()
{
    QDir folder;
    if(!folder.exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)))
        return;
    else
    {
        /*
        *读取账号信息
        */
        QString fileUrl = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/temp.ipss";
        QFile file;
        file.setFileName(fileUrl);
        file.open(QIODevice::ReadOnly);

        if(file.isOpen())
        {
            SimpleCrypt decrypt(enKey);
            QStringList result;
            QTextStream in(&file);
            int lineNumber = 0;
            while(!in.atEnd())
            {
                QString line = decrypt.decryptToString(in.readLine());

                lineNumber++;
                if(lineNumber == 1)
                {
                    if(line != "iPass")
                    {
                        QMessageBox::warning(this, "iPass", QString::fromLocal8Bit("数据文件损坏，请联系iPass"));
                        return;
                    }
                }
                else if(lineNumber == 2)
                {
                    bool ok;
                    totalNumberOfAccount = line.toInt(&ok);
                    if(totalNumberOfAccount == 0)
                        return;
                }
                else
                    result.append(line);
            }
            if((result.length() < 5) | (result.at(0) != "title") | (result.at(totalNumberOfAccount+1) != "account") | (result.at(2*totalNumberOfAccount+2) != "password") | (result.at(3*totalNumberOfAccount+3) != "note") | (result.at(4*totalNumberOfAccount+4) != "strengh"))
                QMessageBox::warning(this, "iPass", QString::fromLocal8Bit("数据文件损坏，请联系iPass"));
            else
            {
                title_list.clear();
                account_list.clear();
                password_list.clear();
                note_list.clear();
                strength_list.clear();
                view_list.clear();
                for(int i = 0; i < totalNumberOfAccount; i++)
                {
                    title_list.append(result.at(1+totalNumberOfAccount*0+i));
                    account_list.append(result.at(2+totalNumberOfAccount*1+i));
                    password_list.append(result.at(3+totalNumberOfAccount*2+i));
                    note_list.append(result.at(4+totalNumberOfAccount*3+i));
                    strength_list.append(result.at(5+totalNumberOfAccount*4+i));
                }
                showImportInformation();
            }
        }

        /*
        *读取同步信息
        */
        QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/syncInfo.txt";
        QFile fileTxt;
        fileTxt.setFileName(fileName);
        fileTxt.open(QIODevice::ReadOnly);

        if(fileTxt.isOpen())
        {
            QTextStream in(&fileTxt);
            while(!in.atEnd())
            {
                QString time = in.readLine();
                QString file = in.readLine();
                QString type = in.readLine();
                QString number = in.readLine();
                emit importAndExportInformation(time, file, type, number);
            }
        }
    }
}

void MainWindow::deleteAccount()
{
    /*
    *判断删除项是否为最后一项
    */
    if(indexOfList == m_list->count() - 1)
        isListLast = true;

    /*
    *第一步先删除左侧列表元素
    */
    QListWidgetItem* temp = m_list->takeItem(indexOfList);
    delete temp;

    /*
    *第二步删除存储在链表中的元素
    */
    title_list.removeAt(indexOfList);
    account_list.removeAt(indexOfList);
    password_list.removeAt(indexOfList);
    note_list.removeAt(indexOfList);
    view_list.removeAt(indexOfList);

    totalNumberOfAccount--;

    /*
    *如果删除的不是列表最后一个元素，则及时更新右侧stack显示内容
    *如果删除的是最后一个元素，且删除后列表不为空，则及时更新右侧内容，注意此时索引号为删除项的上一项
    *如果删除的是最后一个元素，且删除后列表为空， 则将此时右侧stack内容隐藏，以显示空白
    */
    if(isListLast == false)
        emit updateStackInformation(indexOfList);
    else if((isListLast == true) && (m_list->count() > 0))
        emit updateStackInformation(indexOfList-1);
    else
        stack->hide();

    /*
    *标志位归位
    */
    isDelete = false;
    isListLast = false;

}

void MainWindow::searchAccount()
{
    QList<int> searchResult;

    searchOfApp *m_searchOfApp = new searchOfApp(this);
    m_searchOfApp->setGeometry(geometry().x() + m_search->x(), geometry().y() + m_search->y() + 2 * m_search->height(), 192, 200);

    if(totalNumberOfAccount == 0)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::fromLocal8Bit("没有符合条件的记录！"));
        item->setFont(QFont(QString::fromLocal8Bit("微软雅黑"),10,5));
        m_searchOfApp->searchList->addItem(item);
    }
    else
    {
        for(int i = 0; i < totalNumberOfAccount; i++)
        {
            QString temp = title_list.at(i);
            if(temp.contains(m_search->text(), Qt::CaseInsensitive))
                searchResult.append(i);
        }
        if(searchResult.length() == 0)
        {
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(QString::fromLocal8Bit("没有符合条件的记录！"));
            item->setFont(QFont(QString::fromLocal8Bit("微软雅黑"),10,5));
            m_searchOfApp->searchList->addItem(item);
        }
        else
        {
            for(int i = 0; i < searchResult.length(); i++)
            {
                QListWidgetItem *item = new QListWidgetItem;
                item->setText(title_list.at(searchResult.at(i)));
                item->setFont(QFont(QString::fromLocal8Bit("微软雅黑"),10,5));
                m_searchOfApp->searchList->addItem(item);
            }
        }
    }

    if(searchResult.length() > 0)
        connect(m_searchOfApp->searchList, &QListWidget::currentRowChanged, [=](int index){m_list->setCurrentRow(searchResult.at(index));});
    m_searchOfApp->show();
    isSearchShown = true;
}

void MainWindow::showImportInformation()
{
    isDelete = true;
    m_list->clear();
    m_stack->removeWidget(stack);
    isDelete = false;

    for(int i = 0; i < totalNumberOfAccount; i++)
    {
        view_list.append(0);

        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(QIcon(":/image/account.png"));
        item->setText(title_list.at(i));
        item->setFont(QFont(QString::fromLocal8Bit("微软雅黑"),14,5));
        item->setTextColor(QColor(100,139,178));
        item->setSizeHint(QSize(100,60));
        m_list->insertItem(i, item);
        m_list->setCurrentRow(i);

        m_stack->insertWidget(i, stack);
        stack->show();
        emit updateStackInformation(i);
    }
}

void MainWindow::createActions()
{
    newAccount = new QAction(QIcon(":/image/newAccount.png"),QString::fromLocal8Bit("添加"),this);
    newAccount->setShortcut(QKeySequence::New);
    newAccount->setStatusTip(QString::fromLocal8Bit("添加新的账户密码"));

    importFile = new QAction(QIcon(":/image/import.png"),QString::fromLocal8Bit("导入"),this);
    importFile->setShortcut(QKeySequence(tr("Ctrl+i")));
    importFile->setStatusTip(QString::fromLocal8Bit("导入备份文件"));

    exportFile = new QAction(QIcon(":/image/export.png"),QString::fromLocal8Bit("导出"),this);
    exportFile->setShortcut(QKeySequence(tr("Ctrl+e")));
    exportFile->setStatusTip(QString::fromLocal8Bit("导出备份文件"));

    exitApp = new QAction(QIcon(":/image/exit.png"),QString::fromLocal8Bit("关闭"),this);
    exitApp->setShortcut(QKeySequence::Close);
    exitApp->setStatusTip(QString::fromLocal8Bit("关闭软件"));

    hidePassword = new QAction(QIcon(":/image/switch_on.png"),QString::fromLocal8Bit("隐藏敏感"),this);
    hidePassword->setStatusTip(QString::fromLocal8Bit("隐藏/显示密码"));

    lockApp = new QAction(QIcon(),QString::fromLocal8Bit("软件上锁"),this);
    lockApp->setShortcut(QKeySequence(tr("Ctrl+l")));
    lockApp->setStatusTip(QString::fromLocal8Bit("软件界面上锁"));
    if(mainPasswordOfApp.isEmpty())
        lockApp->setEnabled(false);

    generatePassword = new QAction(QIcon(),QString::fromLocal8Bit("生成密码"),this);
    generatePassword->setShortcut(QKeySequence(tr("Ctrl+g")));
    generatePassword->setStatusTip(QString::fromLocal8Bit("为你的账号生成安全可靠的密码"));

    settings = new QAction(QIcon(":/image/settings.png"),QString::fromLocal8Bit("软件设置"),this);
    settings->setShortcut(QKeySequence(tr("Ctrl+Alt+S")));
    settings->setStatusTip(QString::fromLocal8Bit("软件设置"));

    userGuideManual = new QAction(QString::fromLocal8Bit("使用说明"),this);
    userGuideManual->setStatusTip(QString::fromLocal8Bit("用户使用手册"));

    contactUs = new QAction(QString::fromLocal8Bit("联系我们"),this);
    contactUs->setStatusTip(QString::fromLocal8Bit("联系我们的方式"));

    aboutApp = new QAction(QIcon(":/image/about.png"),QString::fromLocal8Bit("关于软件"),this);
    aboutApp->setShortcut(QKeySequence(tr("Ctrl+a")));
    aboutApp->setStatusTip(QString::fromLocal8Bit("关于软件"));

}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(QString::fromLocal8Bit("文件"));
    fileMenu->addAction(newAccount);
    fileMenu->addSeparator();
    fileMenu->addAction(importFile);
    fileMenu->addAction(exportFile);
    fileMenu->addSeparator();
    fileMenu->addAction(exitApp);

    toolMenu = menuBar()->addMenu(QString::fromLocal8Bit("工具"));
    toolMenu->addAction(hidePassword);
    toolMenu->addAction(lockApp);
    toolMenu->addAction(generatePassword);
    toolMenu->addSeparator();
    toolMenu->addAction(settings);

    helpMenu = menuBar()->addMenu(QString::fromLocal8Bit("帮助"));
    helpMenu->addAction(userGuideManual);
    helpMenu->addAction(contactUs);
    helpMenu->addAction(aboutApp);
}

void MainWindow::createConnections()
{
    connect(newAccount, SIGNAL(triggered(bool)), this, SLOT(create_newAccount()));
    connect(importFile, SIGNAL(triggered(bool)), this, SLOT(import_file()));
    connect(exportFile, SIGNAL(triggered(bool)), this, SLOT(export_file()));
    connect(exitApp, SIGNAL(triggered(bool)), qApp, SLOT(closeAllWindows()));
    connect(hidePassword, SIGNAL(triggered(bool)), this, SLOT(hide_password()));
    connect(lockApp, SIGNAL(triggered(bool)), this, SLOT(lock_app()));
    connect(generatePassword, SIGNAL(triggered(bool)), this, SLOT(generate_password()));
    connect(settings, SIGNAL(triggered(bool)), this, SLOT(set_app()));
    connect(userGuideManual, SIGNAL(triggered(bool)), this, SLOT(open_userGuide()));
    connect(contactUs, SIGNAL(triggered(bool)), this, SLOT(contact_us()));
    connect(aboutApp, SIGNAL(triggered(bool)), this, SLOT(open_aboutApp()));

    connect(lockApp_Btn, SIGNAL(clicked(bool)), this, SLOT(lock_app()));
    connect(createNewAccount_Btn, SIGNAL(clicked(bool)), this, SLOT(create_newAccount()));

    connect(currentTime, SIGNAL(timeout()), this, SLOT(update_time()));

    connect(m_list, SIGNAL(currentRowChanged(int)), this, SLOT(showStackInformation(int)));

    connect(mainpassword, &passwordOfApp::mainPasswordFinished, [=](){lock_app();lockApp->setEnabled(true);lockApp_Btn->setEnabled(true);});
    connect(mainpassword, &passwordOfApp::mainPasswordNotFinished, [=](){show();});

    connect(mlock, SIGNAL(correctPassword()), this, SLOT(showAll()));

    connect(m_newAccount, SIGNAL(saveAccountToListFinished()), this, SLOT(insertNewAccountToStack()));
    connect(m_newAccount, SIGNAL(editAccountFinished()), this, SLOT(editAccountFinished()));

    connect(this, SIGNAL(editStackInformation()), m_newAccount, SLOT(changeAccount()));

    connect(this, SIGNAL(updateStackInformation(int)), stack, SLOT(updateInformation(int)));
    connect(stack, SIGNAL(editInformationOfAccount()), this, SLOT(changeInformationOfAccount()));
    connect(stack, SIGNAL(deleteAccount()), this, SLOT(deleteAccount()));

    connect(this, SIGNAL(importAndExportInformation(QString,QString,QString,QString)), setting, SLOT(insertRowOfTable(QString,QString,QString,QString)));

    connect(search_Btn, SIGNAL(clicked(bool)), this, SLOT(searchAccount()));
    connect(m_search, &QLineEdit::returnPressed, [=](){
                                                       if(m_search->hasFocus())
                                                       {
                                                       searchAccount();
                                                       }
                                                      });

}

void MainWindow::writeSettings()
{
    QSettings settings("iPass Inc.",QString::fromLocal8Bit("iPass"));   //组织的名字，应用程序的名称
    settings.setValue("geometry",saveGeometry());
    settings.setValue("automaticLockApp", automaticLockApp);
    settings.setValue("valueOfIntervalTime", valueOfIntervalTime);
    settings.setValue("unitOfIntervalTime", unitOfIntervalTime);
    settings.setValue("isMaskPassword", isMaskPassword);
    settings.setValue("clearClipboard", clearClipboard);
    settings.setValue("clearTime", clearTime);
    settings.setValue("backupLog", backupLog);
    settings.setValue("isFirstOpenApp", isFirstOpenApp);
    settings.setValue("mainPasswordOfApp", mainPasswordOfApp);
    settings.setValue("enKey", enKey);
    settings.setValue("timeOfLockInterval", timeOfLockInterval);
    settings.setValue("lastSyncTime", lastSyncTime);
    settings.setValue("totalNumberOfAccount", totalNumberOfAccount);
}

void MainWindow::readSettings()
{
    QSettings settings("iPass Inc.",QString::fromLocal8Bit("iPass"));   //组织的名字，应用程序的名称
    restoreGeometry(settings.value("geometry").toByteArray());

    automaticLockApp = settings.value("automaticLockApp", QVariant(true)).toBool();
    valueOfIntervalTime = settings.value("valueOfIntervalTime", QVariant(30)).toInt();
    unitOfIntervalTime = settings.value("unitOfIntervalTime", QVariant(2)).toInt();
    isMaskPassword = settings.value("isMaskPassword", QVariant(true)).toBool();
    clearClipboard = settings.value("clearClipboard", QVariant(true)).toBool();
    clearTime = settings.value("clearTime", QVariant(10)).toInt();
    backupLog = settings.value("backupLog", QVariant(true)).toBool();
    isFirstOpenApp = settings.value("isFirstOpenApp", QVariant(true)).toBool();
    mainPasswordOfApp = settings.value("mainPasswordOfApp", QVariant(QString())).toString();
    enKey = settings.value("enKey", QVariant(0)).toULongLong();
    timeOfLockInterval = settings.value("timeOfLockInterval", QVariant(30)).toInt();
    lastSyncTime = settings.value("lastSyncTime", QVariant(QString::fromLocal8Bit("尚未备份"))).toString();
    totalNumberOfAccount = settings.value("totalNumberOfAccount").toInt();
}

void MainWindow::hideAll()
{
    if(user->isShown == true)
        findChild<userGuide *>("userGuide")->hide();
    if(setting->isShown == true)
        findChild<settingsOfApp *>("settingsOfApp")->hide();
    if(newpassword->isShown == true)
        findChild<generatePassWord *>("generatePassWord")->hide();
    if(m_contact->isShown == true)
        findChild<contact *>("contact")->hide();
    if(m_newAccount->isShown == true)
        findChild<addNewAccount *>("addNewAccount")->hide();

    hide();
}

void MainWindow::showAll()
{
    if(user->isShown == true)
        findChild<userGuide *>("userGuide")->show();
    if(setting->isShown == true)
        findChild<settingsOfApp *>("settingsOfApp")->show();
    if(newpassword->isShown == true)
        findChild<generatePassWord *>("generatePassWord")->show();
    if(m_contact->isShown == true)
        findChild<contact *>("contact")->show();
    if(m_newAccount->isShown == true)
        findChild<addNewAccount *>("addNewAccount")->show();

    show();
}
