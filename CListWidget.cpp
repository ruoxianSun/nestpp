#include "CListWidget.h"



QRect CListWidget::visualRect(const QModelIndex &index) const{
    QRect rect= QListView::visualRect(index);
    if(index.row()%2==1)
    {
        QRect raw=rect;
        raw.setX(viewport()->width()-rect.width());
        raw.setY(rect.y());
        raw.setWidth(rect.width());
        raw.setHeight(rect.height());
        rect=raw;
    }
    return rect;
}


