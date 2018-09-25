#include "searchofapp.h"

searchOfApp::searchOfApp(QWidget *parent) : QWidget(parent)
{
    searchList = new myListWidget;

    m_layOut = new QGridLayout;
    m_layOut->addWidget(searchList);
    m_layOut->setMargin(0);
    m_layOut->setSpacing(0);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
//    setWindowModality(Qt::WindowModal);
    setFixedSize(192,200);
    setLayout(m_layOut);
    setMouseTracking(true);

    connect(searchList, &myListWidget::focusOut, [=](){if(isSearchShown == true) {close(); isSearchShown = false;}});

}

void searchOfApp::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    close();
    isSearchShown = false;
}

void searchOfApp::focusOutEvent(QFocusEvent *event)
{
    event->accept();
    if(isSearchShown == true)
    {
        close();
        isSearchShown = false;
    }
}

