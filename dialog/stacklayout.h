#ifndef STACKLAYOUT_H
#define STACKLAYOUT_H

#include <QWidget>
#include <QClipboard>
#include "global/global.h"

namespace Ui {
class stackLayout;
}

class stackLayout : public QWidget
{
    Q_OBJECT

public:
    explicit stackLayout(QWidget *parent = 0);
    ~stackLayout();

private:
    Ui::stackLayout *ui;

public slots:
    void updateInformation(int index);

signals:
    void editInformationOfAccount();
    void deleteAccount();
};

#endif // STACKLAYOUT_H
