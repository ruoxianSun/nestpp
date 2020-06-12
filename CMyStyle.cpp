#include "CMyStyle.h"
#include <QStyleOptionComplex>
#include <QPainter>
#include <QDebug>

QRect CMyStyle::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, const QWidget *widget) const
{
    switch (cc) {
        case QStyle::CC_ComboBox:
    {
        switch (sc) {
            case QStyle::SC_ComboBoxListBoxPopup:
        {
            return opt->rect.adjusted(0,2,0,2);
        }break;
        default:
            return QProxyStyle::subControlRect(cc,opt,sc,widget);
        }
    }break;
    default:
        return QProxyStyle::subControlRect(cc,opt,sc,widget);
    }
}

QRect CMyStyle::subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    switch (element) {
    case QStyle::SE_ItemViewItemText:{
        qDebug()<<qstyleoption_cast<const QStyleOptionViewItem*>(option)->text;
        return option->rect.adjusted(20,1,1,1);
    }break;
    }
    return QProxyStyle::subElementRect(element,option,widget);
}






void CMyStyle::drawItemText(QPainter *painter, const QRect &rect, int flags,
                            const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    QProxyStyle::drawItemText(painter,rect.adjusted(10,0,0,0),flags,pal,enabled,text,textRole);
}
