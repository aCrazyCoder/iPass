#include "myeventfilter.h"

MyEventFilter::MyEventFilter(QObject *parent) : QObject(parent)
{

}

bool MyEventFilter::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == QEvent::KeyPress || ev->type() == QEvent::MouseMove || ev->type() == QEvent::MouseButtonPress)
        timer->start(1000 * timeOfLockInterval);
    return QObject::eventFilter(obj, ev);
}
