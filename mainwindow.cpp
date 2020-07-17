#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPainter"
#include "QKeyEvent"
#include "form.h"
#include "CModel3d.h"
#include "CStlReader.h"
#include "CShaderManager.h"
#include "CGlWidget.h"
#include "CMyStyle.h"
#include "formitem.h"
#include <QStyledItemDelegate>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    form=new Form(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_T:{
        CTest test;
        auto ret = test.nfptest();
        polygons=std::get<0>(ret);
        nfps=std::get<1>(ret);
    }break;
    case Qt::Key_D:
    {
        form->setWindowFlag(Qt::Dialog);
        form->show();
    }break;
    case Qt::Key_G:
    {

        gls=std::make_shared<CGlWidget>();
        gls->setWindowFlag(Qt::Dialog);
        gls->show();

        CStlReader reader;
        CGeometry*geom=reader.readModel("helm.stl",[&](int t,int p){
//            qDebug()<<"loading ["<<p<<"/"<<t<<"]";
        });
        std::shared_ptr<CGeometry> geomp=std::make_shared<CGeometry>(*geom);
        geoms.push_back(geomp);
        delete geom;
        gls->addModels(geoms);
    }break;
    case Qt::Key_A:{
        QPolygonF p1=QSettings("pp0.pl",QSettings::IniFormat).value("pl").value<QPolygonF>();
        QPolygonF p2=QSettings("pp1.pl",QSettings::IniFormat).value("pl").value<QPolygonF>();
        npPolygonPtr pp1=std::make_shared<npPolygon>();
        for(auto v:p1)
        {
            pp1->add(v.x(),v.y());
        }
        polygons.push_back(pp1);
        npPolygonPtr pp2=std::make_shared<npPolygon>();
        for(auto v:p2)
        {
            pp2->add(v.x(),v.y());
        }
        polygons.push_back(pp2);
    }break;
    case Qt::Key_N:
    {
        CTest test;
        double w=width();
        double h=height();
        test.config.binWidth=w;
        test.config.binHeight=h;
        test.config.spaceing=0;
        test.config.concave=false;
        polygons=test.nestSameParts();
        nfps.clear();
        for(auto poly:polygons)
        {
            if(poly->parent)
                nfps.push_back(poly->parent);
        }
    }break;
    }
    update();
}

void MainWindow::paintEvent(QPaintEvent *e)
{
//return QMainWindow::paintEvent(e);
    QFont f;
    f.setFamily("PingFang SC");
    f.setPointSize(17);
    auto toImg=[](QFont font,QString msg,QColor color=Qt::black)
    {
        QFontMetrics fm(font);
        QImage img(fm.horizontalAdvance(msg),fm.height(), QImage::Format_ARGB32_Premultiplied);

        if(img.isNull())return QImage();
        img.fill(Qt::white);
        QPen pen;
        pen.setColor(color);
        QTextOption option(Qt::AlignRight | Qt::AlignTop);
        option.setWrapMode(QTextOption::WordWrap);
        QRectF rect(0, 0, img.width(), img.height());
        QPainter painter;
        painter.begin(&img);
        //painter.setRenderHint(QPainter::Antialiasing);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(0, 0, img.width(), img.height(), Qt::red);
        painter.setFont(font);
        painter.setPen(pen);
        painter.drawText(rect, msg, option);
        painter.setCompositionMode(QPainter::CompositionMode_Destination);
        painter.end();
        return img;
    };
    int angle=360;
    QImage img=toImg(f,QString("%1 %2 : %3¡å").arg(tr("rotate")).arg("Y").arg(angle));
    QPainter painter(this);
    painter.drawImage(100,100,img);
    painter.setPen(Qt::red);
    npRectangle::value_type offx=150;
    npRectangle::value_type offy=150;
    npRectangle::value_type scale=5.f;
    for(auto poly:polygons)
    {
        npRectangle bound=GeometryUtil::getPolygonBounds(*poly);
        npRectangle::value_type hh=bound.h*0.5;
        npRectangle::value_type hw=bound.w*0.5;
        npRectangle::coord_type c=bound.center();
        QPolygonF dpoly;
        for(auto v:*poly)
        {
            dpoly.append(QPointF(v.x*scale+offx,v.y*scale+offy));
        }
        painter.drawPolygon(dpoly);
    }
    painter.setPen(Qt::black);

    for(auto poly:nfps)
    {
        npRectangle bound=GeometryUtil::getPolygonBounds(*poly);
        npRectangle::value_type hh=bound.h*0.5;
        npRectangle::value_type hw=bound.w*0.5;
        npRectangle::coord_type c=bound.center();
        QPolygonF dpoly;
        for(auto v:*poly)
        {
            dpoly.append(QPointF(v.x*scale+offx,v.y*scale+offy));
        }
        painter.drawPolygon(dpoly);
    }

}
