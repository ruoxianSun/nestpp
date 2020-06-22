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
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(QColor textColorA READ getTextColorA WRITE setTextColorA)
public:
    explicit CPushButton(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*e);
    QIcon getAicon()const;
    void setAicon(const QIcon&icon);
    int getIconPosition();
    void setIconPosition(const int&point);
    void setIconTextSpace(int space){iconTextSpace=space;}
    int getIconTextSpace(){return iconTextSpace;}
    void setTextColor(const QColor&color){textColor=color;}
    QColor getTextColor()const{return textColor;}
    void setTextColorA(const QColor&color){textColorA=color;}
    QColor getTextColorA()const{return textColorA;}
signals:

public slots:
protected:
    QIcon aicon;
    int iconPosition;
    int iconTextSpace;
    QColor textColor;
    QColor textColorA;
};

#endif // CPUSHBUTTON_H
