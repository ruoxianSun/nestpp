#include "form.h"
#include "ui_form.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include "formitem.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFontDialog>
#include <QFontMetrics>
#include <QStandardItemModel>
#include "CMyStyle.h"
#include <QStyledItemDelegate>
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
//    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(onFont()));

    ui->comboBox->setStyle(new CMyStyle);
    ui->comboBox->setItemDelegate(new QStyledItemDelegate);
    ui->comboBox->addItems(QStringList()<<"hello"<<"world");


    model = new QStandardItemModel(ui->treeView);
    ui->treeView->setModel(model);
    ui->treeView->setTreePosition(1);

    model->setHorizontalHeaderLabels(QStringList()
                                     <<QStringLiteral("Project")
                                     <<QStringLiteral("tag1")
                                     <<QStringLiteral("tag2")
                                     <<QStringLiteral("tag3")
                                     <<QStringLiteral("tag4")
                                     <<QStringLiteral("tag5"));
    QStandardItem * item = new QStandardItem(tr("item one"));

    model->setItem(0,0,new QStandardItem(""));
    model->setItem(1,0,new QStandardItem(""));
    model->setItem(2,0,new QStandardItem(""));
    model->setItem(0,1,item);
    model->setItem(1,1,new QStandardItem(tr("item two")));
    model->setItem(2,1,new QStandardItem(tr("item three")));
    QList<QStandardItem*> list;
    list<<model->item(0,0)<<model->item(1,0)<<model->item(2,0);
    for(int i = 0;i<list.length();i++)
    {
        QStandardItem* item = list.at(i);
        for(int j=0;j<5;j++)
            item->setChild(0,j,new QStandardItem(QString("sub item %1").arg(j)));
        item->setChild(0,5,new QStandardItem(""));//不加这一行不行
    }


    ui->listWidget->setViewMode(QListWidget::IconMode);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setWrapping(true);
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setSpacing(0);
    for(int i=0;i<2;i++)
    {
        auto form=new FormItem;
        auto item=new QListWidgetItem;
        item->setSizeHint(form->size());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item,form);
    }
}

Form::~Form()
{
    delete ui;
}

void Form::onItemClicked(QTreeWidgetItem *item, int column)
{

}

void Form::onFont()
{
    bool isok=false;
    QFont f=QFontDialog::getFont(&isok,this->font());
    if(isok)ui->pushButton->setFont(f);
    qDebug()<<f;
    update();
    QFontMetrics met(this->font());
    qDebug()<<"label height:"<<ui->label->height()<<", font height:"<<met.height();
    qDebug()<<"ascent:"<<met.ascent()<<" ,descent:"<<met.descent()<<" ,overline pos:"<<met.overlinePos()
           <<",cap height:"<<met.capHeight()<<",height:"<<met.boundingRect("HH").height()<<",tight height:"<<met.tightBoundingRect("HH").height();
}

void Form::paintEvent(QPaintEvent *e)
{
    ///params config
    int shadowSize=10;
    int radSize=3;
    QColor borderColor("#aaaaaa");
    QColor backgroundColor("#ffffff");
    QVector<QVector<double>> gradientCurve={{0,150},{0.2,75},{0.75,10},{1,0}};
    ///end params config

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    auto setGradient=[&](QGradient*g){
        for(auto id:gradientCurve)
        {
            borderColor.setAlpha((int)id[1]);
            g->setColorAt(id[0],borderColor);
        }
    };
    shadowSize+=radSize;
#if 1 //left
    QRectF rectLeft(0, 0, shadowSize*2, shadowSize*2);
    int startAngle = 90 * 16;
    int spanAngle = 90 * 16;
    QRadialGradient radialGradient(rectLeft.center(), rectLeft.width()*0.5,rectLeft.center());
    setGradient(&radialGradient);
    painter.setBrush(QBrush(radialGradient));
    painter.drawPie(rectLeft,startAngle,spanAngle);
    QLinearGradient lgLeft(shadowSize,0,0,0);
    setGradient(&lgLeft);
    painter.setBrush(QBrush(lgLeft));
    painter.drawRect(QRect(0,shadowSize,shadowSize,this->height()-shadowSize*2));
#endif

#if 1//right
    QRectF rectRight(width()-shadowSize*2, height()-shadowSize*2, shadowSize*2, shadowSize*2);
    QRadialGradient rgRight(rectRight.center(), rectRight.width()*0.5,rectRight.center());

    setGradient(&rgRight);
    painter.setBrush(QBrush(rgRight));
    painter.drawPie(rectRight,270*16,90*16);
    QLinearGradient lgRight(width()-shadowSize,0,width(),0);

    setGradient(&lgRight);
    painter.setBrush(QBrush(lgRight));
    painter.drawRect(QRect(width()-shadowSize,shadowSize,shadowSize,this->height()-shadowSize*2));
#endif


#if 1 //top
    QRectF rectTop(width()-shadowSize*2, 0, shadowSize*2, shadowSize*2);
    QRadialGradient rgTop(rectTop.center(), rectTop.width()*0.5,rectTop.center());

    setGradient(&rgTop);
    painter.setBrush(QBrush(rgTop));
    painter.drawPie(rectTop,0*16,90*16);
    QLinearGradient lgTop(0,shadowSize,0,0);

    setGradient(&lgTop);
    painter.setBrush(QBrush(lgTop));
    painter.drawRect(QRect(shadowSize,0,this->width()-2*shadowSize,shadowSize));
#endif

#if 1 //bottom
    QRectF rectBottom(0, height()-2*shadowSize, shadowSize*2, shadowSize*2);
    QRadialGradient rgBottom(rectBottom.center(), rectBottom.width()*0.5,rectBottom.center());

    setGradient(&rgBottom);
    painter.setBrush(QBrush(rgBottom));
    painter.drawPie(rectBottom,180*16,90*16);
    QLinearGradient lgBottom(0,height()-shadowSize,0,height());

    setGradient(&lgBottom);
    painter.setBrush(QBrush(lgBottom));
    painter.drawRect(QRect(shadowSize,height()-shadowSize,this->width()-2*shadowSize,shadowSize));
#endif


    int adj=shadowSize-radSize;
    QRect inner=rect().adjusted(adj,adj,-adj,-adj);
    painter.setBrush(backgroundColor);
#if 0 //normal
    painter.drawRect(inner);
#else
    painter.drawRoundRect(inner,radSize,radSize);
#endif

}
