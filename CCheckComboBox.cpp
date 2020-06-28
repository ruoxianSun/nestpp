#include "CCheckComboBox.h"

CCheckComboBox::CCheckComboBox(QWidget *parent) :
    CCombobox(parent)
{

}

void CCheckComboBox::appendItem(const QString &text, bool bChecked)
{
    QIcon icon;
    if( bChecked )
    {
        icon.addFile(":/image/icon_check_select.png");
    }
    else
    {
        icon.addFile(":/image/icon_check_normal.png");
    }

    addItem(icon, text, bChecked);
}

void CCheckComboBox::updateIndexStatus(int index)
{
    bool bChecked = this->itemData(index).toBool();

    if( bChecked )
    {
        this->setItemIcon(index, QIcon(":/image/icon_check_normal.png"));
    }
    else
    {
        this->setItemIcon(index, QIcon(":/image/icon_check_select.png"));
    }

    setItemData(index, !bChecked);

    emit checkedStateChange(index, !bChecked);
}

void CCheckComboBox::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();

    int iconWidth = this->iconSize().width();

    if( x <= iconWidth )
    {
        int index = this->currentIndex();

        updateIndexStatus(index);
    }
    else
    {
        QComboBox::mousePressEvent(event);
    }
}

void CCheckComboBox::hidePopup()
{
    int iconWidth = this->iconSize().width();

    int x = QCursor::pos().x() - mapToGlobal(geometry().topLeft()).x() + geometry().x();

    int index = view()->selectionModel()->currentIndex().row();

    if( x >= 0 && x <= iconWidth )
    {
        updateIndexStatus(index);
    }
    else
    {
        QComboBox::hidePopup();
    }
}

void CCheckComboBox::addItems(const QStringList &list, bool c)
{
    for(auto it:list)
    {
        appendItem(it,c);
    }
}
