#ifndef CMYSTYLE_H
#define CMYSTYLE_H

#include <QObject>
#include <QCommonStyle>
#include <QStyleFactory>
class CMyStyle : public QCommonStyle
{
    Q_OBJECT
public:
    explicit CMyStyle() : QCommonStyle()
    {
    }
    ~CMyStyle(){}
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget) const override;
    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;

    void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled,
                      const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;

signals:

public slots:

};

#endif // CMYSTYLE_H
