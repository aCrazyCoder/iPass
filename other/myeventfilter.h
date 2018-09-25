#ifndef MYEVENTFILTER_H
#define MYEVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <global/global.h>

class MyEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit MyEventFilter(QObject *parent = nullptr);

signals:

public slots:

protected:
      bool eventFilter(QObject *obj, QEvent *ev);
};

#endif // MYEVENTFILTER_H
