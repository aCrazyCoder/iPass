#ifndef SEARCHOFAPP_H
#define SEARCHOFAPP_H

#include <QListWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QWidget>
#include <QLayout>
#include "global/global.h"
#include "other/mylistwidget.h"

class searchOfApp : public QWidget
{
    Q_OBJECT
public:
    searchOfApp(QWidget *parent = 0);

    void leaveEvent(QEvent *event)override;
    void focusOutEvent(QFocusEvent *event)override;

    myListWidget *searchList;

private:

    QGridLayout *m_layOut;
};

#endif // SEARCHOFAPP_H
