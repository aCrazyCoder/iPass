#include "mylistwidget.h"

myListWidget::myListWidget()
{
    setFixedSize(192,200);
    setMouseTracking(true);
    setStyleSheet("QListWidget{border:1px solid blue;}"
                              "QListWidget::Item{border-bottom:1px solid rgb(64,203,239);}"
                              "QListWidget::Item:selected{background:white; color:black; }");
}

void myListWidget::focusOutEvent(QFocusEvent *event)
{
    /*
    *重写ListWidget的焦点离开函数，焦点不在时，立即关闭搜索窗口
    */
    event->accept();
    emit focusOut();
}
