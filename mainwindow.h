#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QStackedWidget>
#include <QListWidget>
#include <QFrame>
#include <QMenuBar>
#include <QAction>
#include <QApplication>
#include <QSettings>
#include <QTimer>
#include <QLabel>
#include <QStatusBar>
#include <QPushButton>
#include <QLineEdit>
#include <QDateTime>
#include <QDebug>
#include <QStandardPaths>
#include <QFileDialog>

#include "dialog/passwordofapp.h"
#include "dialog/lockofapp.h"
#include "dialog/changepasswordofapp.h"
#include "dialog/addnewaccount.h"
#include "dialog/settingsofapp.h"
#include "dialog/generatepassword.h"
#include "dialog/userguide.h"
#include "dialog/contact.h"
#include "dialog/stacklayout.h"
#include "dialog/searchofapp.h"
#include "other/simplecrypt.h"

#include "global/global.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent* event)override;

public slots:
    void create_newAccount();
    void import_file();
    void export_file();
    void hide_password();
    void lock_app();
    void generate_password();
    void set_app();
    void open_userGuide();
    void contact_us();
    void open_aboutApp();

    void update_time();

    void remindMainPassword();

    void insertNewAccountToStack();
    void editAccountFinished();

    void showStackInformation(int index);

    void changeInformationOfAccount();

    void deleteAccount();

    void searchAccount();

    void showImportInformation();

    void showAll();
    void hideAll();

private:
    void initTool();
    void initInterface();

    void createActions();
    void createMenu();
    void createConnections();

    void readSettings();
    void writeSettings();

    QAction* newAccount;
    QAction* importFile;
    QAction* exportFile;
    QAction* exitApp;

    QAction* hidePassword;
    QAction* lockApp;
    QAction* generatePassword;
    QAction* settings;

    QAction* userGuideManual;
    QAction* contactUs;
    QAction* aboutApp;

    QMenu* fileMenu;
    QMenu* toolMenu;
    QMenu* helpMenu;

    QLabel *currentTimeLable;
    QTimer *currentTime;

    /*
    *最后一次同步时间信息
    */
    QLabel *lastSyncInformation;

    /*
    *创建工具栏
    */
    QFrame *m_frame;
    QPushButton *lockApp_Btn;
    QPushButton *createNewAccount_Btn;
    QLabel *searchLeft;
    QLabel *searchRight;
    QLineEdit *m_search;
    QPushButton *search_Btn;
    QFrame *expandFrame;
    QHBoxLayout *frame_Layout;

    /*
    *stack主布局
    */
    QStackedWidget *m_stack;
    QListWidget *m_list;
    QFrame *stackSpilterLine;     //垂直分割线
    QHBoxLayout *stack_Layout;
    void initStackLayout();

    /*
    *APP主布局
    */
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QFrame *spilterLine;     //水平分割线

    /*
    *设置主密码
    */
    passwordOfApp *mainpassword;

    /*
    *增加新的账户信息
    */
    addNewAccount *m_newAccount;

    /*
    *stack右侧布局
    */
    stackLayout *stack;

    /*
    *锁屏
    */
    lockOfApp *mlock;

    /*
    *使用说明
    */
    userGuide *user;

    /*
    *设置对话框
    */
    settingsOfApp* setting;

    /*
    *产生密码
    */
    generatePassWord *newpassword;

    /*
    *联系方式对话框
    */
    contact *m_contact;

    /*
    *保存数据到用户数据文件夹
    */
    void saveData();

    /*
    *保存同步信息到用户数据文件夹
    */
    void saveSyncData();

    /*
    *打开软件后显示数据
    */
    void openAppShow();

signals:
    void updateStackInformation(int index);
    void editStackInformation();
    void searchClose();
    void importAndExportInformation(QString time, QString file, QString type, QString number);

};

#endif // MAINWINDOW_H
