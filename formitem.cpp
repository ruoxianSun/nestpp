#include "formitem.h"
#include "ui_formitem.h"
#include "QListView"
#include <QPainter>
#include "CMyStyle.h"
#include <QComboBox>
#include <QStyledItemDelegate>
FormItem::FormItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormItem)
{
    ui->setupUi(this);
    ui->comboBox->setItemDelegate(new QStyledItemDelegate(this));
    ui->comboBox->setStyle(new CMyStyle);
    ui->comboBox->addItems(QStringList()<<"hello"<<"world");
}

FormItem::~FormItem()
{
    delete ui;
}
