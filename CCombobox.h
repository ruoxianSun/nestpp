#ifndef CCOMBOBOX_H
#define CCOMBOBOX_H

#include <QComboBox>
#include <QStyledItemDelegate>
class CComboBoxDelegate:public QStyledItemDelegate{
public:
    CComboBoxDelegate(QObject*parent=0):QStyledItemDelegate (parent){}

    // painting
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

};
class CCombobox : public QComboBox
{
    Q_OBJECT
public:
    explicit CCombobox(QWidget *parent = nullptr);

signals:

public slots:
protected:
    void paintEvent(QPaintEvent*e);
};

#endif // CCOMBOBOX_H
