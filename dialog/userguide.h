#ifndef USERGUIDE_H
#define USERGUIDE_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class userGuide;
}

class userGuide : public QWidget
{
    Q_OBJECT

public:
    explicit userGuide(QWidget *parent = 0);
    ~userGuide();

    void closeEvent(QCloseEvent *event)override;

    bool isShown;

private:
    Ui::userGuide *ui;
};

#endif // USERGUIDE_H
