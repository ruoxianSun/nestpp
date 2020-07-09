#include "FormShortCut.h"
#include "ui_FormShortCut.h"

FormShortCut::FormShortCut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormShortCut)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    connect(ui->pbClose,SIGNAL(clicked(bool)),this,SLOT(close()));
    onUpdate();
}

FormShortCut::~FormShortCut()
{
    delete ui;
}

void FormShortCut::onUpdate()
{

    ui->listTitle->layout()->setContentsMargins(30,0,0,0);
    ui->listTitle->key->setText(tr("Scene"));
    ui->listTitle->value->setText(tr("Function key"));
    QStringList keys={
        tr("Delete parts"),
        tr("Undo"),
        tr("Redo"),
        tr("Multiple parts"),
        tr("Box selection support points"),
        tr("Rotate parts"),
        tr("Scale parts"),
    };
    QMap<QString,QString> keyMaps={
        {tr("Delete parts"),                    tr("Delete")},
        {tr("Undo"),                            tr("Ctrl+Z")},
        {tr("Redo"),                            tr("Ctrl+Shift+Z")},
        {tr("Multiple parts"),                  tr("Ctrl+Mouse Left")},
        {tr("Box selection support points"),    tr("Ctrl+Mouse Left")},
        {tr("Rotate parts"),                    tr("Rotation angle+Enter")},
        {tr("Scale parts"),                     tr("Zoom ratio+Enter")},
    };
    auto addItem=[&](QWidget*parent,QString k,QString v){
        auto item=new FormShortCutItem(parent);
        item->layout()->setContentsMargins(30,0,0,0);
        item->key->setText(k);
        item->value->setText(v);
        return item;
    };
    auto addRow=[&](QString k,QString v,bool isBold=false){

        auto ritem=new QListWidgetItem;
        ritem->setSizeHint(QSize(ui->listWidget->width(),50));
        ui->listWidget->addItem(ritem);
        ui->listWidget->setItemWidget(ritem,addItem(ui->listWidget,k,v));
    };
    for (int i=0;i<keys.size();i++) {
        addRow(keys[i],keyMaps[keys[i]]);
    }
}

FormShortCutItem::FormShortCutItem(QWidget *parent):QWidget(parent){
    key=new QLabel(this);
    key->setText("key");
    key->setFixedWidth(450);//调整与后面value标签的距离
    value=new QLabel(this);
    value->setText("value");
    QHBoxLayout*hbl=new QHBoxLayout;
    hbl->addWidget(key);
    hbl->addWidget(value);
    this->setLayout(hbl);
}
