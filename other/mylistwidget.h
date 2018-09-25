#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QFocusEvent>

class myListWidget : public QListWidget
{
    Q_OBJECT
public:
    myListWidget();
    void focusOutEvent(QFocusEvent *event);

signals:
    void focusOut();
};

#endif // MYLISTWIDGET_H
