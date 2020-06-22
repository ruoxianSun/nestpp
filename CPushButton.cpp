#include "CPushButton.h"
#include <QStyleOptionButton>
#include <QStylePainter>
#include <QPainter>
CPushButton::CPushButton(QWidget *parent) :
    QPushButton(parent)
{
    aicon=QIcon();
    iconPosition=0;
    iconTextSpace=0;
}

void CPushButton::paintEvent(QPaintEvent *e)
{
    QStyleOptionButton button;
    initStyleOption(&button);
    QStylePainter painter;
    painter.begin(this);
    button.icon=QIcon();
    button.text.clear();
    painter.drawControl(QStyle::CE_PushButton,button);
    button.icon=icon();
    button.palette.setBrush(QPalette::ButtonText,getTextColor());
    if(button.state&QStyle::State_MouseOver)
    {
        button.icon=getAicon();
        button.palette.setBrush(QPalette::ButtonText,getTextColorA());
    }
    QRect iconRect({getIconPosition(),
                   button.rect.height()/2-iconSize().height()/2},iconSize());
    button.icon.paint(&painter,iconRect);
    QRect textRect=button.rect.adjusted(getIconPosition()+iconSize().width()+getIconTextSpace(),0,0,0);
    painter.drawItemText(textRect, Qt::AlignVCenter|Qt::AlignLeft, button.palette, (button.state & QStyle::State_Enabled),
                         text(), QPalette::ButtonText);
}

QIcon CPushButton::getAicon() const {
    return aicon;
}



void CPushButton::setAicon(const QIcon &icon)
{
    aicon=icon;
    update();
    updateGeometry();
}

int CPushButton::getIconPosition()
{
    return iconPosition;
}

void CPushButton::setIconPosition(const int &point)
{
    iconPosition=point;
}

