#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QTreeWidget>

namespace Ui {
class Form;
}
class QStandardItemModel;
class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
protected slots:
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onFont();
    void onShowMessageBox();
protected:
    void paintEvent(QPaintEvent*e);
private:
    QStandardItemModel*model;
    Ui::Form *ui;
};

#endif // FORM_H
