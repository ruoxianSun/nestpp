#ifndef CCOMBOBOX_H
#define CCOMBOBOX_H

#include <QComboBox>
#include <QStyledItemDelegate>
class CComboBoxDelegate:public QStyledItemDelegate{
    Q_OBJECT
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(QColor textColorHover READ getTextColorHover WRITE setTextColorHover)
    Q_PROPERTY(QColor textColorPressed READ getTextColorPressed WRITE setTextColorPressed)
public:
    CComboBoxDelegate(QObject*parent=0):QStyledItemDelegate (parent){}

    // painting
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;
private:
    void setTextColor(const QColor&color){textColor=color;}
    QColor getTextColor()const{return textColor;}
    void setTextColorHover(const QColor&color){textColorHover=color;}
    QColor getTextColorHover()const{return textColorHover;}
    void setTextColorPressed(const QColor&color){textColorPressed=color;}
    QColor getTextColorPressed()const{return textColorPressed;}
private:
        QColor textColor,textColorHover,textColorPressed;

};
class CCombobox : public QComboBox
{
    Q_OBJECT
public:
    explicit CCombobox(QWidget *parent = nullptr);

signals:

public slots:
protected:
    void paintEvent(QPaintEvent*e);
};

#endif // CCOMBOBOX_H
