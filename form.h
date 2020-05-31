#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QTreeWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
protected slots:
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onFont();
protected:
    void paintEvent(QPaintEvent*e);
private:
    Ui::Form *ui;
};

#endif // FORM_H
