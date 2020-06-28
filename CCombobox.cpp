#include "CCombobox.h"
#include "CMyStyle.h"
#include <QListView>
#include <QStyledItemDelegate>
#include <QStylePainter>
#include <QApplication>
#include <QStyleOption>
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
}

void CComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    const QWidget *widget = opt.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();

    QRect checkRect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &option, widget);
    QRect iconRect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemDecoration,  &option, widget);
    QRect textRect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemText,  &option, widget);

    QString tex=opt.text;
    opt.text.clear();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);


    const QStyleOptionViewItem *vopt=&option;

    const QAbstractItemView *view = qobject_cast<const QAbstractItemView *>(widget);
    bool newStyle = true;

    QPalette palette = vopt->palette;
    palette.setColor(QPalette::All, QPalette::HighlightedText, palette.color(QPalette::Active, QPalette::Text));
    // Note that setting a saturated color here results in ugly XOR colors in the focus rect
    palette.setColor(QPalette::All, QPalette::Highlight, palette.base().color().darker(108));
    QStyleOptionViewItem adjustedOption = *vopt;
    adjustedOption.palette = palette;
    // We hide the  focusrect in singleselection as it is not required
    if ((view->selectionMode() == QAbstractItemView::SingleSelection)
        && !(vopt->state & QStyle::State_KeyboardFocusChange))
    adjustedOption.state &= ~QStyle::State_HasFocus;

    // draw the text
    if (!tex.isEmpty()) {
        QPalette::ColorGroup cg = vopt->state & QStyle::State_Enabled
                              ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
            cg = QPalette::Inactive;

        if (vopt->state & QStyle::State_Selected) {
            painter->setPen(vopt->palette.color(cg, QPalette::HighlightedText));
        } else {
            painter->setPen(vopt->palette.color(cg, QPalette::Text));
        }
        if (vopt->state & QStyle::State_Editing) {
            painter->setPen(vopt->palette.color(cg, QPalette::Text));
            painter->drawRect(textRect.adjusted(0, 0, -1, -1));
        }

//        d->viewItemDrawText(p, vopt, textRect);
        auto icon=index.data(Qt::DecorationRole);
        auto ricon=qvariant_cast<QIcon>(icon);
        if(icon.isValid())
        {
            textRect=textRect.adjusted(20,0,0,0);
        }
        style->drawItemText(painter,textRect,
                            Qt::AlignLeft|Qt::AlignVCenter,vopt->palette,widget->isEnabled(),tex);
    }



}
