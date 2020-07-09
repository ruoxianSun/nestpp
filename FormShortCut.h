#ifndef FORMSHORTCUT_H
#define FORMSHORTCUT_H

#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
namespace Ui {
class FormShortCut;
}
class FormShortCutItem:public QWidget
{
    Q_OBJECT
public:
    explicit FormShortCutItem(QWidget*parent=0);

public:
    QLabel*key,*value;
};
class FormShortCut : public QDialog
{
    Q_OBJECT

public:
    explicit FormShortCut(QWidget *parent = nullptr);
    ~FormShortCut();
protected slots:
    void onUpdate();
private:
    Ui::FormShortCut *ui;
};

#endif // FORMSHORTCUT_H
