#include "CPushButton.h"
#include <QStyleOptionButton>
#include <QStylePainter>
#include <QPainter>
#include <QDebug>
CPushButton::CPushButton(QWidget *parent) :
    QPushButton(parent)
{
    iconHover=QIcon();
    iconPressed=QIcon();
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
        button.icon=geticonHover();
        button.palette.setBrush(QPalette::ButtonText,getTextColorHover());
    }
    if(button.state&QStyle::State_Sunken)
    {
        button.icon=getIconP();
        button.palette.setBrush(QPalette::ButtonText,getTextColorPressed());
    }
    if(button.state&QStyle::State_On)
    {
        button.icon=geticonChecked();
        button.palette.setBrush(QPalette::ButtonText,getTextColorChecked());
    }
    if(!(button.state&QStyle::State_Enabled))
    {
        qDebug()<<"enabled cpushbutton";
        button.icon=geticonDisabled();
        button.palette.setBrush(QPalette::ButtonText,getTextColorDisabled());
    }
    QRect iconRect({getIconPosition(),
                   button.rect.height()/2-iconSize().height()/2},iconSize());
    button.icon.paint(&painter,iconRect);
    QRect textRect=button.rect.adjusted(getIconPosition()+iconSize().width()+getIconTextSpace(),0,0,0);
    painter.drawItemText(textRect, Qt::AlignVCenter|Qt::AlignLeft, button.palette, (button.state & QStyle::State_Enabled),
                         text(), QPalette::ButtonText);
}
