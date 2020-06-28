#include "private/qstylesheetstyle_p.h"
#include "CMyStyle.h"
#include <QStyleOptionComplex>
#include <QPainter>
#include <QDebug>
#include <QStyleOptionButton>
static QWindow*qt_getWindow(const QWidget*w)
{
    return w?w->window()->windowHandle():0;
}




QRect CMyStyle::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, const QWidget *widget) const
{
    switch (cc) {
    case QStyle::CC_ComboBox:
    {
        switch (sc) {
        case QStyle::SC_ComboBoxListBoxPopup:
        {
            return opt->rect.adjusted(0,4,0,4);
        }break;
        default:
            break;
        }
    }break;
    default:break;
    }
    return QProxyStyle::subControlRect(cc,opt,sc,widget);
}

QRect CMyStyle::subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget) const
{
//    qDebug()<<element<<widget;
    switch (element) {
    case QStyle::SE_ItemViewItemText:{
//        qDebug()<<qstyleoption_cast<const QStyleOptionViewItem*>(option)->text;
//        return option->rect.adjusted(20,1,1,1);
    }break;
    case QStyle::SE_ItemViewItemDecoration:{
//        qDebug()<<qstyleoption_cast<const QStyleOptionViewItem*>(option)->text;
    }break;
    case QStyle::SE_ItemViewItemCheckIndicator:{
    }break;
    }
    return QProxyStyle::subElementRect(element,option,widget);
}

void CMyStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    QProxyStyle::drawPrimitive(element,option,painter,widget);
}

void CMyStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    QProxyStyle::drawControl(element,option,painter,widget);
}








void CMyStyle::drawItemText(QPainter *painter, const QRect &rect, int flags,
                            const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    QProxyStyle::drawItemText(painter,rect.adjusted(20,0,0,0),flags,pal,enabled,text,textRole);
}
