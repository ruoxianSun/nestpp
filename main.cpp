#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QDebug>
#include <QDir>
#include <memory>
#include "CMyStyle.h"
#include "FormShortCut.h"
#include <qtstylesheethelper.h>
void readCSSfile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream txtInput(&file);
    QString context = txtInput.readAll();
    qApp->setStyleSheet(context);
    file.close();
}
void addFont()
{
    QString fontPath="./debug/font/";
    QDir fontDir(fontPath);
    QStringList fontList=fontDir.entryList(QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
    for (auto font:fontList)
    {
        int id=QFontDatabase::addApplicationFont(fontPath+font);
        qDebug()<<"family"<<QFontDatabase::applicationFontFamilies(id);

    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    addFont();
    readCSSfile(":/css1.css");
    QtCss::QtStyleSheetHelper::helper()->setStyleSheet(a.styleSheet());
//#define TEST
#ifdef TEST
        FormShortCut sc;
        sc.show();
//    auto item=new FormShortCutItem();
//    item->setObjectName("titel");
//    item->layout()->setContentsMargins(30,0,0,0);
//    item->layout()->setSpacing(350);
//    item->key->setText("value");
//    item->value->setText("value");
//    QFont f("Microsoft YaHei");
//    f.setWeight(1);
//    f.setItalic(true);
//    item->key->setFont(f);
//    item->show();
#else
    MainWindow w;
    w.show();
#endif
    return a.exec();
}
