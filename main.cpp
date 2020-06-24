#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QDebug>
#include <QDir>
#include <memory>
#include "CMyStyle.h"
#include "css.h"
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
void testCss(){

    StyleSheet ss;
    int num=ss.parse(qApp->styleSheet().toStdString());
    qDebug()<<"num:"<<num;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    addFont();
    readCSSfile(":/css1.css");
    MainWindow w;
    w.show();
//    testCss();
//    qDebug()<<a.style()<<",,,,,"<<a.styleSheet();
//    auto styel=new CMyStyle;
//    QApplication::setStyle(styel);
//    qDebug()<<a.style()<<",,,,,"<<a.styleSheet();
    return a.exec();
}
