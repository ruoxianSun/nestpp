#ifndef CTREEVIEW_H
#define CTREEVIEW_H

#include <QWidget>
#include <QTreeView>
class CTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CTreeView(QWidget *parent = nullptr) : QTreeView(parent)
    {

    }
    void drawRow(QPainter *painter, const QStyleOptionViewItem &option,
                 const QModelIndex &index) const;
signals:

public slots:
};

#endif // CTREEVIEW_H
