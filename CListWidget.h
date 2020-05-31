#ifndef CLISTWIDGET_H
#define CLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QDebug>
class CListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit CListWidget(QWidget *parent = nullptr) : QListWidget(parent)
    {

    }
    QRect visualRect(const QModelIndex&index)const override;
signals:

public slots:
};

#endif // CLISTWIDGET_H
