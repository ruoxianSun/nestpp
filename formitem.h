#ifndef FORMITEM_H
#define FORMITEM_H

#include <QWidget>

namespace Ui {
class FormItem;
}

class FormItem : public QWidget
{
    Q_OBJECT

public:
    explicit FormItem(QWidget *parent = nullptr);
    ~FormItem();

private:
    Ui::FormItem *ui;
};

#endif // FORMITEM_H
