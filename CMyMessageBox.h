#ifndef CMYMESSAGEBOX_H
#define CMYMESSAGEBOX_H

#include <QDialog>

namespace Ui {
class CMyMessageBox;
}

class CMyMessageBox : public QDialog
{
    Q_OBJECT

public:
    static int warning(QWidget*parent,const QString&title,const QString&text);
    explicit CMyMessageBox(QWidget *parent = nullptr);
    ~CMyMessageBox();
    void setText(const QString&msg);

private:
    Ui::CMyMessageBox *ui;
};

#endif // CMYMESSAGEBOX_H
