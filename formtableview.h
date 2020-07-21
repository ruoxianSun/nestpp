#ifndef FORMTABLEVIEW_H
#define FORMTABLEVIEW_H

#include <QWidget>
#include "CCombobox.h"
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QItemEditorFactory>
#include <QApplication>
class CCustomItemDelegate:public QStyledItemDelegate{
    Q_OBJECT
public:
    CCustomItemDelegate(QObject*parent=0):QStyledItemDelegate (parent){

    }
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

namespace Ui {
class FormTableView;
}

class FormTableView : public QWidget
{
    Q_OBJECT

public:
    explicit FormTableView(QWidget *parent = nullptr);
    ~FormTableView();

private:
    Ui::FormTableView *ui;
};

#endif // FORMTABLEVIEW_H
