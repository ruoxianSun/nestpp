#include "formtableview.h"
#include "ui_formtableview.h"
#include <QStandardItemModel>
FormTableView::FormTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTableView)
{
    ui->setupUi(this);
    QStandardItemModel*model=new QStandardItemModel();
    model->setColumnCount(6);
    model->setRowCount(10);
    QStringList headers;
    for(auto i=0;i<6;i++)
    {
        headers<<QString("Head %1").arg(i);
    }
    model->setHorizontalHeaderLabels(headers);
    for(int r=0;r<10;r++)
        for(int c=0;c<6;c++)
        {
            model->setItem(r,c,new QStandardItem(QString("Item(%1,%2)").arg(r).arg(c)));
        }

    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setItemDelegate(new CCustomItemDelegate(this));
    for(auto i=0;i<model->rowCount();i++)
    {
        ui->tableView->setRowHeight(i,50);
    }
    ui->tableView->setFocus(Qt::FocusReason::MouseFocusReason);
}

FormTableView::~FormTableView()
{
    delete ui;
}
