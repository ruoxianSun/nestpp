#include "CPushButton.h"
#include <QStyleOptionButton>

#include <QPainter>

void CPushButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    QStyleOptionButton button;
    button.initFrom(this);
    style()->drawControl(QStyle::CE_PushButton,&button,&painter,this);

    QRect pr=rect().adjusted(0,0,-rect().width()*0.5,0);
    style()->drawItemPixmap(&painter,pr,Qt::AlignRight|Qt::AlignVCenter,icon().pixmap(iconSize()));

    QRect tr=rect().adjusted(rect().width()*0.5+50,0,0,0);
    style()->drawItemText(&painter,tr,Qt::AlignLeft,palette(),true,text());

}
