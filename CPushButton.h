#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QPushButton>
#include <QStylePainter>
class QStyleSheetStyle;
class CPushButtonPrivate;
class CPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QIcon aicon READ getAicon WRITE setAicon)
    Q_PROPERTY(int iconPosition READ getIconPosition WRITE setIconPosition)
    Q_PROPERTY(int iconTextSpace READ getIconTextSpace WRITE setIconTextSpace)
public:
    explicit CPushButton(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*e);
    QIcon getAicon()const;
    void setAicon(const QIcon&icon);
    int getIconPosition();
    void setIconPosition(const int&point);
    void setIconTextSpace(int space){iconTextSpace=space;}
    int getIconTextSpace(){return iconTextSpace;}
signals:

public slots:
protected:
    QIcon aicon;
    int iconPosition;
    int iconTextSpace;
};

#endif // CPUSHBUTTON_H
