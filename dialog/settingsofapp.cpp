#include "settingsofapp.h"
#include "ui_settingsofapp.h"

settingsOfApp::settingsOfApp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingsOfApp)
{
    isShown = false;

    ui->setupUi(this);

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(90);
    ui->tableWidget->horizontalHeader()->setSectionsClickable(false);

    QStringList header;
    header<<QString::fromLocal8Bit("时间")<<QString::fromLocal8Bit("文件")<<QString::fromLocal8Bit("类型")<<QString::fromLocal8Bit("数目");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(font);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(false);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(10);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->resizeSection(0,90);
    ui->tableWidget->horizontalHeader()->resizeSection(1,200);
    ui->tableWidget->horizontalHeader()->resizeSection(2,40);
    ui->tableWidget->horizontalHeader()->resizeSection(3,40);
    ui->tableWidget->horizontalHeader()->setFixedHeight(25);
    ui->tableWidget->setStyleSheet("selection-background-color:lightblue;");
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(179,234,249);}");
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);

    ui->tableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                          "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                          "QScrollBar::handle:hover{background:gray;}"
                                                          "QScrollBar::sub-line{background:transparent;}"
                                                          "QScrollBar::add-line{background:transparent;}");
    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                        "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                        "QScrollBar::handle:hover{background:gray;}"
                                                        "QScrollBar::sub-line{background:transparent;}"
                                                        "QScrollBar::add-line{background:transparent;}");

    /*
    *初始化设置变量
    */
    if(!mainPasswordOfApp.isEmpty())
        ui->mainPassword->setEnabled(true);
    else
        ui->mainPassword->setEnabled(false);
    ui->lock_CB->setChecked(automaticLockApp);
    ui->lock_SB->setValue(valueOfIntervalTime);
    ui->lock_CoB->setCurrentIndex(unitOfIntervalTime);
    ui->mask->setChecked(isMaskPassword);
    ui->clipBoard_CB->setChecked(clearClipboard);
    ui->clipBoard_SB->setValue(clearTime);
    ui->clipBoard_SB->setMaximum(60);
    ui->clipBoard_SB->setMinimum(1);
    ui->checkBox_4->setChecked(backupLog);

    /*
    *限制输入范围
    */
    if(ui->lock_CoB->currentIndex() == 2)
    {
        ui->lock_SB->setMaximum(60);
        ui->lock_SB->setMinimum(5);
    }
    else if(ui->lock_CoB->currentIndex() == 1)
    {
        ui->lock_SB->setMaximum(60);
        ui->lock_SB->setMinimum(1);
    }
    else
    {
        ui->lock_SB->setMaximum(24);
        ui->lock_SB->setMinimum(1);
    }

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [=](){saveSettingVariable();close();emit settingFinished();});
    connect(ui->buttonBox, &QDialogButtonBox::rejected, [=](){close();});

    connect(ui->lock_CoB, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index){if(index == 2)
                                                                                                                    {
                                                                                                                        ui->lock_SB->setMaximum(60);
                                                                                                                        ui->lock_SB->setMinimum(5);
                                                                                                                    }
                                                                                                                else if(index == 1)
                                                                                                                    {
                                                                                                                        ui->lock_SB->setMaximum(60);
                                                                                                                        ui->lock_SB->setMinimum(1);
                                                                                                                    }
                                                                                                                else
                                                                                                                    {
                                                                                                                        ui->lock_SB->setMaximum(24);
                                                                                                                        ui->lock_SB->setMinimum(1);
                                                                                                                    }
                                                                                                                });
    connect(ui->mainPassword, &QPushButton::clicked, [=](){close();
                                                           changePasswordOfApp *m_change = new changePasswordOfApp(this);
                                                           m_change->show();
                                                           m_change->temp_automaticLockApp = automaticLockApp;
                                                           automaticLockApp = false;});

    setWindowIcon(QIcon(":/image/Password.ico"));
    setWindowTitle(QString::fromLocal8Bit("设置"));
    setWindowFlags(Qt::Window);
    setWindowModality(Qt::WindowModal);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);
}

settingsOfApp::~settingsOfApp()
{
    delete ui;
}

void settingsOfApp::closeEvent(QCloseEvent *event)
{
    event->accept();
    isShown = false;
    if(!mainPasswordOfApp.isEmpty())
        ui->mainPassword->setEnabled(true);
}

void settingsOfApp::insertRowOfTable(QString time, QString file, QString type, QString number)
{

    ui->tableWidget->insertRow(0);

    QTableWidgetItem *item = new QTableWidgetItem();
    QTableWidgetItem *item1 = new QTableWidgetItem();
    QTableWidgetItem *item2 = new QTableWidgetItem();
    QTableWidgetItem *item3 = new QTableWidgetItem();

    item->setText(time);
    item1->setText(file);
    item2->setText(type);
    item3->setText(number);

    item->setTextColor(QColor("gray"));
    item2->setTextColor(QColor("gray"));
    item3->setTextColor(QColor("gray"));

    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(0, 0, item);
    ui->tableWidget->setItem(0, 1, item1);
    ui->tableWidget->setItem(0, 2, item2);
    ui->tableWidget->setItem(0, 3, item3);

    syncInfoOfTime.append(time);
    syncInfoOfFile.append(file);
    syncInfoOfType.append(type);
    syncInfoOfNumber.append(number);
}

void settingsOfApp::saveSettingVariable()
{
    automaticLockApp = ui->lock_CB->isChecked();
    valueOfIntervalTime = ui->lock_SB->value();
    unitOfIntervalTime = ui->lock_CoB->currentIndex();
    isMaskPassword = ui->mask->isChecked();
    clearClipboard = ui->clipBoard_CB->isChecked();
    clearTime = ui->clipBoard_SB->value();
    backupLog = ui->checkBox_4->isChecked();

    if(unitOfIntervalTime == 0)
        timeOfLockInterval = 3600 * valueOfIntervalTime;
    else if(unitOfIntervalTime == 1)
        timeOfLockInterval = 60 * valueOfIntervalTime;
    else
        timeOfLockInterval = valueOfIntervalTime;
}
