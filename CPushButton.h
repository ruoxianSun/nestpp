#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QPushButton>
#include <QStylePainter>
class QStyleSheetStyle;
class CPushButtonPrivate;
class CPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QIcon iconA READ getIconA WRITE setIconA)
    Q_PROPERTY(QIcon iconP READ getIconP WRITE setIconP)
    Q_PROPERTY(int iconPosition READ getIconPosition WRITE setIconPosition)
    Q_PROPERTY(int iconTextSpace READ getIconTextSpace WRITE setIconTextSpace)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(QColor textColorA READ getTextColorA WRITE setTextColorA)
    Q_PROPERTY(QColor textColorP READ getTextColorP WRITE setTextColorP)
public:
    explicit CPushButton(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*e);
    QIcon getIconA()const {return iconA;}
    void setIconA(const QIcon&icon){iconA=icon;}
    QIcon getIconP()const {return iconP;}
    void setIconP(const QIcon&icon){iconP=icon;}
    int getIconPosition(){return iconPosition;}
    void setIconPosition(const int&point){iconPosition=point;}
    void setIconTextSpace(int space){iconTextSpace=space;}
    int getIconTextSpace(){return iconTextSpace;}
    void setTextColor(const QColor&color){textColor=color;}
    QColor getTextColor()const{return textColor;}
    void setTextColorA(const QColor&color){textColorA=color;}
    QColor getTextColorA()const{return textColorA;}
    void setTextColorP(const QColor&color){textColorP=color;}
    QColor getTextColorP()const{return textColorP;}
signals:

public slots:
protected:
    QIcon iconA,iconP;
    int iconPosition;
    int iconTextSpace;
    QColor textColor,textColorA,textColorP;

};

#endif // CPUSHBUTTON_H
