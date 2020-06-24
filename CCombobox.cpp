#include "CCombobox.h"
#include "CMyStyle.h"
#include <QListView>
#include <QStyledItemDelegate>
CCombobox::CCombobox(QWidget *parent) : QComboBox(parent)
{
    setItemDelegate(new QStyledItemDelegate());
    QListView* comboBoxView = (QListView*)view();  //QComboBoxListView对象
    QWidget* containerObj = static_cast<QWidget*>(comboBoxView->parent()); // QComboBoxPrivateContainer对象
    containerObj->setWindowFlag(Qt::NoDropShadowWindowHint); //成功去掉阴
    setStyle(new CMyStyle);
}
