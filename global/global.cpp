#include "global/global.h"

QStringList title_list;
QStringList account_list;
QStringList password_list;
QStringList note_list;
QStringList strength_list;
/*
*控制每一条密码的显示
*/
QList<int> view_list;

/*
*是否第一次打开APP
*/
bool isFirstOpenApp = true;

/*
*主密码
*/
QString mainPasswordOfApp;

/*
*导入导出文件密钥，仅在第一次设置主密码时生成
*/
QByteArray partOfMainPassword;
quint64 enKey = 0;

/*
*总账号数量
*/
int totalNumberOfAccount = 0;

/*
*当前鼠标点击的索引号
*/
int indexOfList = 0;

/*
*是否为编辑模式
*/
bool isEdit = false;

/*
*是否为删除模式
*/
bool isDelete = false;

/*
*是否为列表最后一项
*/
bool isListLast = false;

/*
*设置变量
*/
bool automaticLockApp = true;
int valueOfIntervalTime = 30;
int unitOfIntervalTime = 2;
bool isMaskPassword = true;
bool clearClipboard = true;
int clearTime = 10;
bool backupLog = true;

/*
*监测程序空闲时间用的计时器
*/
QTimer *timer = new QTimer;

/*
*判断应用程序当前是否处于lock状态,防止应用多次被锁
*/
bool isLock = false;

/*
*设置上锁时间间断
*/
int timeOfLockInterval = 30;

/*
*判断搜索框是否显示
*/
bool isSearchShown = false;

/*
*保存当前工作目录
*/
QString currentDirOfApp;

/*
*最近一次的同步信息
*/
QString lastSyncTime = QString::fromLocal8Bit("尚未备份");

/*
*同步信息保存
*/
QStringList syncInfoOfTime;
QStringList syncInfoOfFile;
QStringList syncInfoOfType;
QStringList syncInfoOfNumber;
