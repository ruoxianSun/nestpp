#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QPushButton>

class CPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CPushButton(QWidget *parent = nullptr) : QPushButton(parent)
    {

    }
    void paintEvent(QPaintEvent*e);
signals:

public slots:
};

#endif // CPUSHBUTTON_H
