#include "CCombobox.h"
#include "CMyStyle.h"
#include <QListView>
#include <QStyledItemDelegate>
#include <QStylePainter>
#include <QApplication>
#include <QStyleOption>
#include <QDebug>
#include <qtstylesheethelper.h>
#include <qtstylesheet.h>
using namespace QtCss;
CCombobox::CCombobox(QWidget *parent) : QComboBox(parent)
{
    setItemDelegate(new CComboBoxDelegate(view()));
    auto comboBoxView=(QListView*)(view());
    QWidget* containerObj = static_cast<QWidget*>(comboBoxView->parent()); // QComboBoxPrivateContainer对象
    containerObj->setWindowFlag(Qt::NoDropShadowWindowHint); //成功去掉阴
    setStyle(new CMyStyle);
}


void CCombobox::paintEvent(QPaintEvent *e)
{

    QStylePainter painter(this);
    painter.setPen(palette().color(QPalette::Text));

    // draw the combobox frame, focusrect and selected etc.
    QStyleOptionComboBox opt;
    initStyleOption(&opt);
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);

    // draw the icon and text
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);

//    qDebug()<<font();
}

void CComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    const QWidget *widget = opt.widget;

    QtStyleSheetHelper *helper=QtStyleSheetHelper::helper();
    auto rule=helper->renderRule(opt.widget,&option, PseudoElement_ViewItem);
    rule.configurePalette(&opt.palette,opt.state & QStyle::State_Item ? QPalette::HighlightedText : QPalette::Text,
                          opt.state & QStyle::State_Selected ? QPalette::Highlight : QPalette::Base);

    QStyle *style = widget ? widget->style() : QApplication::style();

    QRect checkRect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &option, widget);
    QRect iconRect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemDecoration,  &option, widget);
    QRect textRect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemText,  &option, widget);

    QString tex=opt.text;
    opt.text.clear();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);



    // draw the text
    if (!tex.isEmpty()) {


        const int textMargin = style->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, widget) + 1;

        textRect = textRect.adjusted(textMargin, 0, -textMargin, 0); // remove width padding
        auto icon=index.data(Qt::DecorationRole);
        auto ricon=qvariant_cast<QIcon>(icon);
        if(icon.isValid())
        {
            textRect=textRect.adjusted(opt.decorationSize.width()+4,0,0,0);
        }
        style->drawItemText(painter,textRect,
                            Qt::AlignLeft|Qt::AlignVCenter,opt.palette,widget->isEnabled(),tex,QPalette::Text);
    }



}
