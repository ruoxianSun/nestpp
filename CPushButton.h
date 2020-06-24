#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QPushButton>
#include <QStylePainter>
class QStyleSheetStyle;
class CPushButtonPrivate;
class CPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QIcon iconHover READ geticonHover WRITE seticonHover)
    Q_PROPERTY(QIcon iconPressed READ getIconP WRITE setIconP)
     Q_PROPERTY(QIcon iconChecked READ geticonChecked WRITE seticonChecked)
    Q_PROPERTY(int iconPosition READ getIconPosition WRITE setIconPosition)
    Q_PROPERTY(int iconTextSpace READ getIconTextSpace WRITE setIconTextSpace)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(QColor textColorHover READ getTextColorHover WRITE setTextColorHover)
    Q_PROPERTY(QColor textColorPressed READ getTextColorPressed WRITE setTextColorPressed)
     Q_PROPERTY(QColor textColorChecked READ getTextColorChecked WRITE setTextColorChecked)
public:
    explicit CPushButton(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*e);
    QIcon geticonHover()const {return iconHover;}
    void seticonHover(const QIcon&icon){iconHover=icon;}
    QIcon getIconP()const {return iconPressed;}
    void setIconP(const QIcon&icon){iconPressed=icon;}
    QIcon geticonChecked()const {return iconChecked;}
    void seticonChecked(const QIcon&icon){iconChecked=icon;}
    int getIconPosition(){return iconPosition;}
    void setIconPosition(const int&point){iconPosition=point;}
    void setIconTextSpace(int space){iconTextSpace=space;}
    int getIconTextSpace(){return iconTextSpace;}
    void setTextColor(const QColor&color){textColor=color;}
    QColor getTextColor()const{return textColor;}
    void setTextColorHover(const QColor&color){textColorHover=color;}
    QColor getTextColorHover()const{return textColorHover;}
    void setTextColorPressed(const QColor&color){textColorPressed=color;}
    QColor getTextColorPressed()const{return textColorPressed;}
    void setTextColorChecked(const QColor&color){textColorChecked=color;}
    QColor getTextColorChecked()const{return textColorChecked;}
signals:

public slots:
protected:
    QIcon iconHover,iconPressed,iconChecked;
    int iconPosition;
    int iconTextSpace;
    QColor textColor,textColorHover,textColorPressed,textColorChecked;

};

#endif // CPUSHBUTTON_H
