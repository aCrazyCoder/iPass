#ifndef CONTACT_H
#define CONTACT_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class contact;
}

class contact : public QWidget
{
    Q_OBJECT

public:
    explicit contact(QWidget *parent = 0);
    ~contact();

    bool isShown;
    void closeEvent(QCloseEvent *event)override;

private:
    Ui::contact *ui;
};

#endif // CONTACT_H
