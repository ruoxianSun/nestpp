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
                          const QModelIndex &index) const override
    {
        if(index.column()==1)
        {
            auto combobox=new CCombobox(parent);
            combobox->addItems(QStringList()<<"Continue"<<"Discreate");
            combobox->setCurrentText(option.text);
            combobox->setGeometry(option.rect.adjusted(3,3,-3,-3));
            return combobox;
        }
        else if(index.column()>1&&index.column()<index.model()->columnCount())
        {
            auto edit=new QLineEdit(parent);
            edit->setText(option.text);
            return edit;
        }
        else
        {
            return QStyledItemDelegate::createEditor(parent,option,index);
        }
    }
    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyle*style=QApplication::style();
        if(index.column()==1)
        {
            QStyleOptionComboBox*combox=new QStyleOptionComboBox;
            combox->rect=option.rect.adjusted(3,3,-3,-3);
            combox->currentText=option.text;
            style->drawComplexControl(QStyle::CC_ComboBox,combox,painter,option.widget);
        }
        else if(index.column()>1&&index.column()<index.model()->columnCount())
        {
            QStyleOptionViewItem*rect=new QStyleOptionViewItem;
            rect->rect=option.rect.adjusted(3,3,-3,-3);
            rect->text=option.text;
            style->drawPrimitive(QStyle::PE_FrameLineEdit,rect,painter,option.widget);
        }
        QStyleOptionViewItem*item=new QStyleOptionViewItem(option);
        item->displayAlignment=Qt::AlignVCenter|Qt::AlignHCenter;
        QStyledItemDelegate::paint(painter,*item,index);

    }
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
