#ifndef QTSTYLESHEETHELPER_H
#define QTSTYLESHEETHELPER_H
#include <QVector>
#include <qtstylesheet.h>
namespace QtCss {


static bool unstylable(const QWidget *w);
static QWidget *containerWidget(const QWidget *w);
static void qtCheckIfInternalObject(const QObject **obj, int *element);
static quint64 pseudoClass(QStyle::State state);
/////////////////////////////////////////////////////////////////////////////////////////
// Rendering rules
static QVector<Declaration> declarations(const QVector<StyleRule> &styleRules, const QString &part, quint64 pseudoClass = PseudoClass_Unspecified);
class QtStyleSheetHelper
{
public:
    static QtStyleSheetHelper *helper(){
        if(!_helper)
        {
            _helper=new QtStyleSheetHelper;
        }
        return _helper;
    }
    QVector<StyleRule> styleRules(const QObject*obj)const;
    bool initObject(const QObject *obj) const;
    bool hasStyleRule(const QObject *obj, int part) const;
    QRenderRule renderRule(const QObject *obj, int element, quint64 state=0) const;
    QRenderRule renderRule(const QObject *obj, const QStyleOption *opt, int pseudoElement=0) const;
    QString styleSheet()const{return _styleSheet;}
    void setStyleSheet(const QString&ss){_styleSheet=ss;}
protected:
    QtStyleSheetHelper()
    {
        _styleSheet="";
        styleSheetCaches=new QStyleSheetCaches;
    }
protected:
    QStyleSheetCaches *styleSheetCaches;
    mutable QtCss::Parser parser;
    QString _styleSheet;
    static QtStyleSheetHelper*_helper;
};
}
#endif // QTSTYLESHEETHELPER_H
