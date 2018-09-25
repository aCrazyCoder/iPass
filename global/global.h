#ifndef GLOBAL_H
#define GLOBAL_H

#include <QStringList>
#include <QString>
#include <QTimer>
#include <QApplication>

extern QStringList title_list;
extern QStringList account_list;
extern QStringList password_list;
extern QStringList note_list;
extern QStringList strength_list;
extern QList<int> view_list;

extern bool isFirstOpenApp;

extern QString mainPasswordOfApp;
extern QByteArray partOfMainPassword;
extern quint64 enKey;

extern int totalNumberOfAccount;

extern int indexOfList;

extern bool isEdit;

extern bool isDelete;
extern bool isListLast;

extern bool isLock;


/*
*设置中的变量
*/
extern bool automaticLockApp;
extern int valueOfIntervalTime;
extern int unitOfIntervalTime;
extern bool isMaskPassword;
extern bool clearClipboard;
extern int clearTime;
extern bool backupLog;

extern QTimer *timer;

extern int timeOfLockInterval;

extern bool isSearchShown;

extern QString currentDirOfApp;

extern QString lastSyncTime;

extern QStringList syncInfoOfTime;
extern QStringList syncInfoOfFile;
extern QStringList syncInfoOfType;
extern QStringList syncInfoOfNumber;


#endif // GLOBAL_H
