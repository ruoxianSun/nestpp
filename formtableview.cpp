#include "formtableview.h"
#include "ui_formtableview.h"
#include <QStandardItemModel>
#include "qtstylesheethelper.h"
using namespace QtCss;
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
    for(auto r=0;r<model->rowCount();r++)
    {
        for(auto c=0;c<model->columnCount();c++)
        {
            ui->tableView->openPersistentEditor(model->index(r,c));
        }
    }
    ui->tableView->setFocus(Qt::FocusReason::MouseFocusReason);
}

FormTableView::~FormTableView()
{
    delete ui;
}

QWidget *CCustomItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column()==1)
    {
        auto combobox=new QComboBox(parent);
        combobox->addItems(QStringList()<<"Continue"<<"Discreate");
        combobox->setCurrentText(option.text);
        return combobox;
    }
    else if(index.column()>1&&index.column()<index.model()->columnCount())
    {
        auto edit=new QLineEdit(parent);
        edit->setText(option.text);
        return edit;
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent,option,index);
    }
}

void CCustomItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt;
    opt.rect=option.rect;

    return QStyledItemDelegate::paint(painter,opt,index);
    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    if(index.column()==1)
    {
        QStyleOptionComboBox*combox=new QStyleOptionComboBox;
        combox->rect=option.rect.adjusted(3,3,-3,-3);
        combox->currentText=option.text;
        style->drawComplexControl(QStyle::CC_ComboBox,combox,painter,option.widget);
    }
    else if(index.column()>1&&index.column()<index.model()->columnCount())
    {
        QStyleOptionFrameV3 opt;
        opt.rect=option.rect.adjusted(3,3,-3,-3);
        style->drawPrimitive(QStyle::PE_PanelLineEdit,&opt,painter,widget);


    }

    // draw the text
    if (!index.data().toString().isEmpty()) {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        opt.rect=option.rect.adjusted(3,3,-3,-3);

        QRect textRect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemText,  &opt, widget);
        const int textMargin = style->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, widget) + 1;

        textRect = textRect.adjusted(textMargin, 0, -textMargin, 0); // remove width padding
        auto icon=index.data(Qt::DecorationRole);
        auto ricon=qvariant_cast<QIcon>(icon);
        if(icon.isValid())
        {
            textRect=textRect.adjusted(option.decorationSize.width()+4,0,0,0);
        }
        style->drawItemText(painter,textRect,
                            Qt::AlignVCenter,option.palette,widget->isEnabled(),
                            index.data().toString(),QPalette::Text);
    }
//    QStyleOptionViewItem*item=new QStyleOptionViewItem(option);
//    item->displayAlignment=Qt::AlignVCenter|Qt::AlignHCenter;
//    QStyledItemDelegate::paint(painter,*item,index);

}

void CCustomItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect.adjusted(6,6,-6,-6));
}
