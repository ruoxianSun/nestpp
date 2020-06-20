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


void CMyStyle::drawControl(QStyle::ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *widget) const
{
    switch (element) {
    case CE_PushButton:
        if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(opt)) {
            proxy()->drawControl(CE_PushButtonBevel, btn, p, widget);
            QStyleOptionButton subopt = *btn;
            subopt.rect = subElementRect(SE_PushButtonContents, btn, widget);
            proxy()->drawControl(CE_PushButtonLabel, &subopt, p, widget);
            if (btn->state & State_HasFocus) {
                QStyleOptionFocusRect fropt;
                fropt.QStyleOption::operator=(*btn);
                fropt.rect = subElementRect(SE_PushButtonFocusRect, btn, widget);
                proxy()->drawPrimitive(PE_FrameFocusRect, &fropt, p, widget);
            }
        }
        break;
    case CE_PushButtonBevel:
        if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(opt)) {
            QRect br = btn->rect;
            int dbi = proxy()->pixelMetric(PM_ButtonDefaultIndicator, btn, widget);
            if (btn->features & QStyleOptionButton::DefaultButton)
                proxy()->drawPrimitive(PE_FrameDefaultButton, opt, p, widget);
            if (btn->features & QStyleOptionButton::AutoDefaultButton)
                br.setCoords(br.left() + dbi, br.top() + dbi, br.right() - dbi, br.bottom() - dbi);
            if (!(btn->features & (QStyleOptionButton::Flat | QStyleOptionButton::CommandLinkButton))
                || btn->state & (State_Sunken | State_On)
                || (btn->features & QStyleOptionButton::CommandLinkButton && btn->state & State_MouseOver)) {
                QStyleOptionButton tmpBtn = *btn;
                tmpBtn.rect = br;
                proxy()->drawPrimitive(PE_PanelButtonCommand, &tmpBtn, p, widget);
            }
            if (btn->features & QStyleOptionButton::HasMenu) {
                int mbi = proxy()->pixelMetric(PM_MenuButtonIndicator, btn, widget);
                QRect ir = btn->rect;
                QStyleOptionButton newBtn = *btn;
                newBtn.rect = QRect(ir.right() - mbi + 2, ir.height()/2 - mbi/2 + 3, mbi - 6, mbi - 6);
                proxy()->drawPrimitive(PE_IndicatorArrowDown, &newBtn, p, widget);
            }
        }
        break;
 case CE_PushButtonLabel:
        if (const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(opt)) {
            QRect textRect = button->rect;
            uint tf = Qt::AlignVCenter | Qt::TextShowMnemonic;
            if (!proxy()->styleHint(SH_UnderlineShortcut, button, widget))
                tf |= Qt::TextHideMnemonic;

            if (!button->icon.isNull()) {
                //Center both icon and text
                QRect iconRect;
                QIcon::Mode mode = button->state & State_Enabled ? QIcon::Normal : QIcon::Disabled;
                if (mode == QIcon::Normal && button->state & State_HasFocus)
                    mode = QIcon::Active;
                QIcon::State state = QIcon::Off;
                if (button->state & State_On)
                    state = QIcon::On;

                QPixmap pixmap = button->icon.pixmap(qt_getWindow(widget), button->iconSize, mode, state);

                int pixmapWidth = pixmap.width() / pixmap.devicePixelRatio();
                int pixmapHeight = pixmap.height() / pixmap.devicePixelRatio();
                int labelWidth = pixmapWidth;
                int labelHeight = pixmapHeight;
                int iconSpacing = 4;//### 4 is currently hardcoded in QPushButton::sizeHint()
                int textWidth = button->fontMetrics.boundingRect(opt->rect, tf, button->text).width();
                if (!button->text.isEmpty())
                    labelWidth += (textWidth + iconSpacing);

                iconRect = QRect(textRect.x() + (textRect.width() - labelWidth) / 2,
                                 textRect.y() + (textRect.height() - labelHeight) / 2,
                                 pixmapWidth, pixmapHeight);

                iconRect = visualRect(button->direction, textRect, iconRect);

                tf |= Qt::AlignLeft; //left align, we adjust the text-rect instead

                if (button->direction == Qt::RightToLeft)
                    textRect.setRight(iconRect.left() - iconSpacing);
                else
                    textRect.setLeft(iconRect.left() + iconRect.width() + iconSpacing);

                if (button->state & (State_On | State_Sunken))
                    iconRect.translate(proxy()->pixelMetric(PM_ButtonShiftHorizontal, opt, widget),
                                       proxy()->pixelMetric(PM_ButtonShiftVertical, opt, widget));
                p->drawPixmap(iconRect, pixmap);
            } else {
                tf |= Qt::AlignHCenter;
            }
            if (button->state & (State_On | State_Sunken))
                textRect.translate(proxy()->pixelMetric(PM_ButtonShiftHorizontal, opt, widget),
                             proxy()->pixelMetric(PM_ButtonShiftVertical, opt, widget));

            if (button->features & QStyleOptionButton::HasMenu) {
                int indicatorSize = proxy()->pixelMetric(PM_MenuButtonIndicator, button, widget);
                if (button->direction == Qt::LeftToRight)
                    textRect = textRect.adjusted(0, 0, -indicatorSize, 0);
                else
                    textRect = textRect.adjusted(indicatorSize, 0, 0, 0);
            }
            proxy()->drawItemText(p, textRect, tf, button->palette, (button->state & State_Enabled),
                         button->text, QPalette::ButtonText);
        }
        break;
    default:
        QCommonStyle::drawControl(element,opt,p,widget);
    }
}

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
            return QCommonStyle::subControlRect(cc,opt,sc,widget);
        }
    }break;
    default:
        return QCommonStyle::subControlRect(cc,opt,sc,widget);
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
    return QCommonStyle::subElementRect(element,option,widget);
}






void CMyStyle::drawItemText(QPainter *painter, const QRect &rect, int flags,
                            const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    QCommonStyle::drawItemText(painter,rect,flags,pal,enabled,text,textRole);
}
