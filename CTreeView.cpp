#include "CTreeView.h"
#include <QPainter>
#include <QModelIndex>
#include <QStyleOptionViewItem>

void CTreeView::drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTreeView::drawRow(painter,option,index);
    QPen oldpen=painter->pen();
    painter->setPen(QColor("#e4e4e4"));
    QRect rect= option.rect.adjusted(visualRect(index).width(),0
                                     ,-visualRect(model()->index(index.row(),model()->columnCount()-1)).width(),0);
    painter->drawLine(rect.bottomLeft(),rect.bottomRight());
    painter->setPen(oldpen);
}
