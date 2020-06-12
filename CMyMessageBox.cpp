#include "CMyMessageBox.h"
#include "ui_CMyMessageBox.h"




int CMyMessageBox::warning(QWidget *parent, const QString &title, const QString &text)
{
    CMyMessageBox box(parent);
    box.setWindowTitle(title);
    box.setText(text);
    return box.exec();
}

CMyMessageBox::CMyMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMyMessageBox)
{
    ui->setupUi(this);
}

CMyMessageBox::~CMyMessageBox()
{
    delete ui;
}

void CMyMessageBox::setText(const QString &msg)
{
    ui->label->setText(msg);
}
