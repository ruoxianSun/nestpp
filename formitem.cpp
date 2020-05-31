#include "formitem.h"
#include "ui_formitem.h"

FormItem::FormItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormItem)
{
    ui->setupUi(this);
}

FormItem::~FormItem()
{
    delete ui;
}
