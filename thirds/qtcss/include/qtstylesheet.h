#ifndef QTSTYLESHEET_H
#define QTSTYLESHEET_H
#include <QtWidgets/QStyle>
#include <qtcssparser.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProxyStyle>
#include <QtWidgets/QWidget>
#include <QtCore/QSet>
#include <QtCore/QObject>
enum PseudoElement {
    PseudoElement_None,
    PseudoElement_DownArrow,
    PseudoElement_UpArrow,
    PseudoElement_LeftArrow,
    PseudoElement_RightArrow,
    PseudoElement_Indicator,
    PseudoElement_ExclusiveIndicator,
    PseudoElement_PushButtonMenuIndicator,
    PseudoElement_ComboBoxDropDown,
    PseudoElement_ComboBoxArrow,
    PseudoElement_Item,
    PseudoElement_SpinBoxUpButton,
    PseudoElement_SpinBoxUpArrow,
    PseudoElement_SpinBoxDownButton,
    PseudoElement_SpinBoxDownArrow,
    PseudoElement_GroupBoxTitle,
    PseudoElement_GroupBoxIndicator,
    PseudoElement_ToolButtonMenu,
    PseudoElement_ToolButtonMenuArrow,
    PseudoElement_ToolButtonDownArrow,
    PseudoElement_ToolBoxTab,
    PseudoElement_ScrollBarSlider,
    PseudoElement_ScrollBarAddPage,
    PseudoElement_ScrollBarSubPage,
    PseudoElement_ScrollBarAddLine,
    PseudoElement_ScrollBarSubLine,
    PseudoElement_ScrollBarFirst,
    PseudoElement_ScrollBarLast,
    PseudoElement_ScrollBarUpArrow,
    PseudoElement_ScrollBarDownArrow,
    PseudoElement_ScrollBarLeftArrow,
    PseudoElement_ScrollBarRightArrow,
    PseudoElement_SplitterHandle,
    PseudoElement_ToolBarHandle,
    PseudoElement_ToolBarSeparator,
    PseudoElement_MenuScroller,
    PseudoElement_MenuTearoff,
    PseudoElement_MenuCheckMark,
    PseudoElement_MenuSeparator,
    PseudoElement_MenuIcon,
    PseudoElement_MenuRightArrow,
    PseudoElement_TreeViewBranch,
    PseudoElement_HeaderViewSection,
    PseudoElement_HeaderViewUpArrow,
    PseudoElement_HeaderViewDownArrow,
    PseudoElement_ProgressBarChunk,
    PseudoElement_TabBarTab,
    PseudoElement_TabBarScroller,
    PseudoElement_TabBarTear,
    PseudoElement_SliderGroove,
    PseudoElement_SliderHandle,
    PseudoElement_SliderAddPage,
    PseudoElement_SliderSubPage,
    PseudoElement_SliderTickmark,
    PseudoElement_TabWidgetPane,
    PseudoElement_TabWidgetTabBar,
    PseudoElement_TabWidgetLeftCorner,
    PseudoElement_TabWidgetRightCorner,
    PseudoElement_DockWidgetTitle,
    PseudoElement_DockWidgetCloseButton,
    PseudoElement_DockWidgetFloatButton,
    PseudoElement_DockWidgetSeparator,
    PseudoElement_MdiCloseButton,
    PseudoElement_MdiMinButton,
    PseudoElement_MdiNormalButton,
    PseudoElement_TitleBar,
    PseudoElement_TitleBarCloseButton,
    PseudoElement_TitleBarMinButton,
    PseudoElement_TitleBarMaxButton,
    PseudoElement_TitleBarShadeButton,
    PseudoElement_TitleBarUnshadeButton,
    PseudoElement_TitleBarNormalButton,
    PseudoElement_TitleBarContextHelpButton,
    PseudoElement_TitleBarSysMenu,
    PseudoElement_ViewItem,
    PseudoElement_ViewItemIcon,
    PseudoElement_ViewItemText,
    PseudoElement_ViewItemIndicator,
    PseudoElement_ScrollAreaCorner,
    PseudoElement_TabBarTabCloseButton,
    NumPseudoElements
};

struct PseudoElementInfo {
    QStyle::SubControl subControl;
    const char name[19];
};

static const PseudoElementInfo knownPseudoElements[NumPseudoElements] = {
    { QStyle::SC_None, "" },
    { QStyle::SC_None, "down-arrow" },
    { QStyle::SC_None, "up-arrow" },
    { QStyle::SC_None, "left-arrow" },
    { QStyle::SC_None, "right-arrow" },
    { QStyle::SC_None, "indicator" },
    { QStyle::SC_None, "indicator" },
    { QStyle::SC_None, "menu-indicator" },
    { QStyle::SC_ComboBoxArrow, "drop-down" },
    { QStyle::SC_ComboBoxArrow, "down-arrow" },
    { QStyle::SC_None, "item" },
    { QStyle::SC_SpinBoxUp, "up-button" },
    { QStyle::SC_SpinBoxUp, "up-arrow" },
    { QStyle::SC_SpinBoxDown, "down-button" },
    { QStyle::SC_SpinBoxDown, "down-arrow" },
    { QStyle::SC_GroupBoxLabel, "title" },
    { QStyle::SC_GroupBoxCheckBox, "indicator" },
    { QStyle::SC_ToolButtonMenu, "menu-button" },
    { QStyle::SC_ToolButtonMenu, "menu-arrow" },
    { QStyle::SC_None, "menu-indicator" },
    { QStyle::SC_None, "tab" },
    { QStyle::SC_ScrollBarSlider, "handle" },
    { QStyle::SC_ScrollBarAddPage, "add-page" },
    { QStyle::SC_ScrollBarSubPage, "sub-page" },
    { QStyle::SC_ScrollBarAddLine, "add-line" },
    { QStyle::SC_ScrollBarSubLine, "sub-line" },
    { QStyle::SC_ScrollBarFirst, "first" },
    { QStyle::SC_ScrollBarLast, "last" },
    { QStyle::SC_ScrollBarSubLine, "up-arrow" },
    { QStyle::SC_ScrollBarAddLine, "down-arrow" },
    { QStyle::SC_ScrollBarSubLine, "left-arrow" },
    { QStyle::SC_ScrollBarAddLine, "right-arrow" },
    { QStyle::SC_None, "handle" },
    { QStyle::SC_None, "handle" },
    { QStyle::SC_None, "separator" },
    { QStyle::SC_None, "scroller" },
    { QStyle::SC_None, "tearoff" },
    { QStyle::SC_None, "indicator" },
    { QStyle::SC_None, "separator" },
    { QStyle::SC_None, "icon" },
    { QStyle::SC_None, "right-arrow" },
    { QStyle::SC_None, "branch" },
    { QStyle::SC_None, "section" },
    { QStyle::SC_None, "down-arrow" },
    { QStyle::SC_None, "up-arrow" },
    { QStyle::SC_None, "chunk" },
    { QStyle::SC_None, "tab" },
    { QStyle::SC_None, "scroller" },
    { QStyle::SC_None, "tear" },
    { QStyle::SC_SliderGroove, "groove" },
    { QStyle::SC_SliderHandle, "handle" },
    { QStyle::SC_None, "add-page" },
    { QStyle::SC_None, "sub-page"},
    { QStyle::SC_SliderTickmarks, "tick-mark" },
    { QStyle::SC_None, "pane" },
    { QStyle::SC_None, "tab-bar" },
    { QStyle::SC_None, "left-corner" },
    { QStyle::SC_None, "right-corner" },
    { QStyle::SC_None, "title" },
    { QStyle::SC_None, "close-button" },
    { QStyle::SC_None, "float-button" },
    { QStyle::SC_None, "separator" },
    { QStyle::SC_MdiCloseButton, "close-button" },
    { QStyle::SC_MdiMinButton, "minimize-button" },
    { QStyle::SC_MdiNormalButton, "normal-button" },
    { QStyle::SC_TitleBarLabel, "title" },
    { QStyle::SC_TitleBarCloseButton, "close-button" },
    { QStyle::SC_TitleBarMinButton, "minimize-button" },
    { QStyle::SC_TitleBarMaxButton, "maximize-button" },
    { QStyle::SC_TitleBarShadeButton, "shade-button" },
    { QStyle::SC_TitleBarUnshadeButton, "unshade-button" },
    { QStyle::SC_TitleBarNormalButton, "normal-button" },
    { QStyle::SC_TitleBarContextHelpButton, "contexthelp-button" },
    { QStyle::SC_TitleBarSysMenu, "sys-menu" },
    { QStyle::SC_None, "item" },
    { QStyle::SC_None, "icon" },
    { QStyle::SC_None, "text" },
    { QStyle::SC_None, "indicator" },
    { QStyle::SC_None, "corner" },
    { QStyle::SC_None, "close-button" },
};


struct QStyleSheetBorderImageData : public QSharedData
{
    QStyleSheetBorderImageData()
        : horizStretch(QtCss::TileMode_Unknown), vertStretch(QtCss::TileMode_Unknown)
    {
        for (int i = 0; i < 4; i++)
            cuts[i] = -1;
    }
    int cuts[4];
    QPixmap pixmap;
    QImage image;
    QtCss::TileMode horizStretch, vertStretch;
};

struct QStyleSheetBackgroundData : public QSharedData
{
    QStyleSheetBackgroundData(const QBrush& b, const QPixmap& p, QtCss::Repeat r,
                              Qt::Alignment a, QtCss::Origin o, QtCss::Attachment t, QtCss::Origin c)
        : brush(b), pixmap(p), repeat(r), position(a), origin(o), attachment(t), clip(c) { }

    bool isTransparent() const {
        if (brush.style() != Qt::NoBrush)
            return !brush.isOpaque();
        return pixmap.isNull() ? false : pixmap.hasAlpha();
    }
    QBrush brush;
    QPixmap pixmap;
    QtCss::Repeat repeat;
    Qt::Alignment position;
    QtCss::Origin origin;
    QtCss::Attachment attachment;
    QtCss::Origin clip;
};

struct QStyleSheetBorderData : public QSharedData
{
    QStyleSheetBorderData() : bi(0)
    {
        for (int i = 0; i < 4; i++) {
            borders[i] = 0;
            styles[i] = QtCss::BorderStyle_None;
        }
    }

    QStyleSheetBorderData(int *b, QBrush *c, QtCss::BorderStyle *s, QSize *r) : bi(0)
    {
        for (int i = 0; i < 4; i++) {
            borders[i] = b[i];
            styles[i] = s[i];
            colors[i] = c[i];
            radii[i] = r[i];
        }
    }

    int borders[4];
    QBrush colors[4];
    QtCss::BorderStyle styles[4];
    QSize radii[4]; // topleft, topright, bottomleft, bottomright

    const QStyleSheetBorderImageData *borderImage() const
    { return bi; }
    bool hasBorderImage() const { return bi!=0; }

    QSharedDataPointer<QStyleSheetBorderImageData> bi;

    bool isOpaque() const
    {
        for (int i = 0; i < 4; i++) {
            if (styles[i] == QtCss::BorderStyle_Native || styles[i] == QtCss::BorderStyle_None)
                continue;
            if (styles[i] >= QtCss::BorderStyle_Dotted && styles[i] <= QtCss::BorderStyle_DotDotDash
                && styles[i] != QtCss::BorderStyle_Solid)
                return false;
            if (!colors[i].isOpaque())
                return false;
            if (!radii[i].isEmpty())
                return false;
        }
        if (bi != 0 && bi->pixmap.hasAlpha())
            return false;
        return true;
    }
};


struct QStyleSheetOutlineData : public QStyleSheetBorderData
{
    QStyleSheetOutlineData()
    {
        for (int i = 0; i < 4; i++) {
            offsets[i] = 0;
        }
    }

    QStyleSheetOutlineData(int *b, QBrush *c, QtCss::BorderStyle *s, QSize *r, int *o)
            : QStyleSheetBorderData(b, c, s, r)
    {
        for (int i = 0; i < 4; i++) {
            offsets[i] = o[i];
        }
    }

    int offsets[4];
};

struct QStyleSheetBoxData : public QSharedData
{
    QStyleSheetBoxData(int *m, int *p, int s) : spacing(s)
    {
        for (int i = 0; i < 4; i++) {
            margins[i] = m[i];
            paddings[i] = p[i];
        }
    }

    int margins[4];
    int paddings[4];

    int spacing;
};

struct QStyleSheetPaletteData : public QSharedData
{
    QStyleSheetPaletteData(const QBrush &fg, const QBrush &sfg, const QBrush &sbg,
                           const QBrush &abg)
        : foreground(fg), selectionForeground(sfg), selectionBackground(sbg),
          alternateBackground(abg) { }

    QBrush foreground;
    QBrush selectionForeground;
    QBrush selectionBackground;
    QBrush alternateBackground;
};

struct QStyleSheetGeometryData : public QSharedData
{
    QStyleSheetGeometryData(int w, int h, int minw, int minh, int maxw, int maxh)
        : minWidth(minw), minHeight(minh), width(w), height(h), maxWidth(maxw), maxHeight(maxh) { }

    int minWidth, minHeight, width, height, maxWidth, maxHeight;
};

struct QStyleSheetPositionData : public QSharedData
{
    QStyleSheetPositionData(int l, int t, int r, int b,QtCss::Origin o, Qt::Alignment p, QtCss::PositionMode m, Qt::Alignment a = 0)
        : left(l), top(t), bottom(b), right(r), origin(o), position(p), mode(m), textAlignment(a) { }

    int left, top, bottom, right;
    QtCss::Origin origin;
    Qt::Alignment position;
    QtCss::PositionMode mode;
    Qt::Alignment textAlignment;
};

struct QStyleSheetImageData : public QSharedData
{
    QStyleSheetImageData(const QIcon &i, Qt::Alignment a, const QSize &sz)
        : icon(i), alignment(a), size(sz) { }

    QIcon icon;
    Qt::Alignment alignment;
    QSize size;
};

class QTCSS_EXPORT QRenderRule
{
public:
    QRenderRule() : features(0), hasFont(false), pal(0), b(0), bg(0), bd(0), ou(0), geo(0), p(0), img(0), clipset(0) { }
    QRenderRule(const QVector<QtCss::Declaration> &, const QObject *);

    QRect borderRect(const QRect &r) const;
    QRect outlineRect(const QRect &r) const;
    QRect paddingRect(const QRect &r) const;
    QRect contentsRect(const QRect &r) const;

    enum { Margin = 1, Border = 2, Padding = 4, All=Margin|Border|Padding };
    QRect boxRect(const QRect &r, int flags = All) const;
    QSize boxSize(const QSize &s, int flags = All) const;
    QRect originRect(const QRect &rect, QtCss::Origin origin) const;

    QPainterPath borderClip(QRect rect);
    void drawBorder(QPainter *, const QRect&);
    void drawOutline(QPainter *, const QRect&);
    void drawBorderImage(QPainter *, const QRect&);
    void drawBackground(QPainter *, const QRect&, const QPoint& = QPoint(0, 0));
    void drawBackgroundImage(QPainter *, const QRect&, QPoint = QPoint(0, 0));
    void drawFrame(QPainter *, const QRect&);
    void drawImage(QPainter *p, const QRect &rect);
    void drawRule(QPainter *, const QRect&);
    void configurePalette(QPalette *, QPalette::ColorGroup, const QWidget *, bool);
    void configurePalette(QPalette *p, QPalette::ColorRole fr, QPalette::ColorRole br);

    const QStyleSheetPaletteData *palette() const { return pal; }
    const QStyleSheetBoxData *box() const { return b; }
    const QStyleSheetBackgroundData *background() const { return bg; }
    const QStyleSheetBorderData *border() const { return bd; }
    const QStyleSheetOutlineData *outline() const { return ou; }
    const QStyleSheetGeometryData *geometry() const { return geo; }
    const QStyleSheetPositionData *position() const { return p; }

    bool hasModification() const;

    bool hasPalette() const { return pal != 0; }
    bool hasBackground() const { return bg != 0 && (!bg->pixmap.isNull() || bg->brush.style() != Qt::NoBrush); }
    bool hasGradientBackground() const { return bg && bg->brush.style() >= Qt::LinearGradientPattern
                                                   && bg->brush.style() <= Qt::ConicalGradientPattern; }

    bool hasNativeBorder() const {
        return bd == 0
               || (!bd->hasBorderImage() && bd->styles[0] == QtCss::BorderStyle_Native);
    }

    bool hasNativeOutline() const {
        return (ou == 0
                || (!ou->hasBorderImage() && ou->styles[0] == QtCss::BorderStyle_Native));
    }

    bool baseStyleCanDraw() const {
        if (!hasBackground() || (background()->brush.style() == Qt::NoBrush && bg->pixmap.isNull()))
            return true;
        if (bg && !bg->pixmap.isNull())
            return false;
        if (hasGradientBackground())
            return features & QtCss::StyleFeature_BackgroundGradient;
        return features & QtCss::StyleFeature_BackgroundColor;
    }

    bool hasBox() const { return b != 0; }
    bool hasBorder() const { return bd != 0; }
    bool hasOutline() const { return ou != 0; }
    bool hasPosition() const { return p != 0; }
    bool hasGeometry() const { return geo != 0; }
    bool hasDrawable() const { return !hasNativeBorder() || hasBackground() || hasImage(); }
    bool hasImage() const { return img != 0; }

    QSize minimumContentsSize() const
    { return geo ? QSize(geo->minWidth, geo->minHeight) : QSize(0, 0); }
    QSize minimumSize() const
    { return boxSize(minimumContentsSize()); }

    QSize contentsSize() const
    { return geo ? QSize(geo->width, geo->height)
                 : ((img && img->size.isValid()) ? img->size : QSize()); }
    QSize contentsSize(const QSize &sz) const
    {
        QSize csz = contentsSize();
        if (csz.width() == -1) csz.setWidth(sz.width());
        if (csz.height() == -1) csz.setHeight(sz.height());
        return csz;
    }
    bool hasContentsSize() const
    { return (geo && (geo->width != -1 || geo->height != -1)) || (img && img->size.isValid()); }

    QSize size() const { return boxSize(contentsSize()); }
    QSize size(const QSize &sz) const { return boxSize(contentsSize(sz)); }
    QSize adjustSize(const QSize &sz)
    {
        if (!geo)
            return sz;
        QSize csz = contentsSize();
        if (csz.width() == -1) csz.setWidth(sz.width());
        if (csz.height() == -1) csz.setHeight(sz.height());
        if (geo->maxWidth != -1 && csz.width() > geo->maxWidth) csz.setWidth(geo->maxWidth);
        if (geo->maxHeight != -1 && csz.height() > geo->maxHeight) csz.setHeight(geo->maxHeight);
        csz=csz.expandedTo(QSize(geo->minWidth, geo->minHeight));
        return csz;
    }

    bool hasStyleHint(const QString &sh) const { return styleHints.contains(sh); }
    QVariant styleHint(const QString &sh) const { return styleHints.value(sh); }

    void fixupBorder(int);

    // Shouldn't be here
    void setClip(QPainter *p, const QRect &rect);
    void unsetClip(QPainter *);

public:
    int features;
    QBrush defaultBackground;
    QFont font;
    bool hasFont;

    QHash<QString, QVariant> styleHints;

    QSharedDataPointer<QStyleSheetPaletteData> pal;
    QSharedDataPointer<QStyleSheetBoxData> b;
    QSharedDataPointer<QStyleSheetBackgroundData> bg;
    QSharedDataPointer<QStyleSheetBorderData> bd;
    QSharedDataPointer<QStyleSheetOutlineData> ou;
    QSharedDataPointer<QStyleSheetGeometryData> geo;
    QSharedDataPointer<QStyleSheetPositionData> p;
    QSharedDataPointer<QStyleSheetImageData> img;

    int clipset;
    QPainterPath clipPath;
};
Q_DECLARE_TYPEINFO(QRenderRule, Q_MOVABLE_TYPE);



QTCSS_EXPORT static QList<QVariant> subControlLayout(const QString& layout);

namespace {
    struct ButtonInfo {
        QRenderRule rule;
        int element;
        int offset;
        int where;
        int width;
    };
}
template <> class QTypeInfo<ButtonInfo> : public QTypeInfoMerger<ButtonInfo, QRenderRule, int> {};
QPixmap loadPixmap(const QString &fileName, const QObject *context);
// Style rules
#define OBJECT_PTR(x) (static_cast<QObject *>(x.ptr))
static inline QObject *parentObject(const QObject *obj)
{
#if QT_CONFIG(tooltip)
    if (qobject_cast<const QLabel *>(obj) && qstrcmp(obj->metaObject()->className(), "QTipLabel") == 0) {
        QObject *p = qvariant_cast<QObject *>(obj->property("_q_stylesheet_parent"));
        if (p)
            return p;
    }
#endif
    return obj->parent();
}
class QStyleSheetStyleSelector : public QtCss::StyleSelector
{
public:
    QStyleSheetStyleSelector() { }

    QStringList nodeNames(NodePtr node) const override
    {
        if (isNullNode(node))
            return QStringList();
        const QMetaObject *metaObject = OBJECT_PTR(node)->metaObject();
#ifndef QT_NO_TOOLTIP
        if (qstrcmp(metaObject->className(), "QTipLabel") == 0)
            return QStringList(QLatin1String("QToolTip"));
#endif
        QStringList result;
        do {
            result += QString::fromLatin1(metaObject->className()).replace(QLatin1Char(':'), QLatin1Char('-'));
            metaObject = metaObject->superClass();
        } while (metaObject != 0);
        return result;
    }
    QString attribute(NodePtr node, const QString& name) const override
    {
        if (isNullNode(node))
            return QString();

        QHash<QString, QString> &cache = m_attributeCache[OBJECT_PTR(node)];
        QHash<QString, QString>::const_iterator cacheIt = cache.constFind(name);
        if (cacheIt != cache.constEnd())
            return cacheIt.value();

        QObject *obj = OBJECT_PTR(node);
        QVariant value = obj->property(name.toLatin1());
        if (!value.isValid()) {
            if (name == QLatin1String("class")) {
                QString className = QString::fromLatin1(obj->metaObject()->className());
                if (className.contains(QLatin1Char(':')))
                    className.replace(QLatin1Char(':'), QLatin1Char('-'));
                cache[name] = className;
                return className;
            } else if (name == QLatin1String("style")) {
                QWidget *w = qobject_cast<QWidget *>(obj);
                QProxyStyle *proxy = w ? qobject_cast<QProxyStyle*>(w->style()) : 0;
                if (proxy) {
                    QString styleName = QString::fromLatin1(proxy->baseStyle()->metaObject()->className());
                    cache[name] = styleName;
                    return styleName;
                }
            }
        }
        QString valueStr;
        if(value.type() == QVariant::StringList || value.type() == QVariant::List)
            valueStr = value.toStringList().join(QLatin1Char(' '));
        else
            valueStr = value.toString();
        cache[name] = valueStr;
        return valueStr;
    }
    bool nodeNameEquals(NodePtr node, const QString& nodeName) const override
    {
        if (isNullNode(node))
            return false;
        const QMetaObject *metaObject = OBJECT_PTR(node)->metaObject();
#ifndef QT_NO_TOOLTIP
        if (qstrcmp(metaObject->className(), "QTipLabel") == 0)
            return nodeName == QLatin1String("QToolTip");
#endif
        do {
            const ushort *uc = (const ushort *)nodeName.constData();
            const ushort *e = uc + nodeName.length();
            const uchar *c = (const uchar *)metaObject->className();
            while (*c && uc != e && (*uc == *c || (*c == ':' && *uc == '-'))) {
                ++uc;
                ++c;
            }
            if (uc == e && !*c)
                return true;
            metaObject = metaObject->superClass();
        } while (metaObject != 0);
        return false;
    }
    bool hasAttributes(NodePtr) const override
    { return true; }
    QStringList nodeIds(NodePtr node) const override
    { return isNullNode(node) ? QStringList() : QStringList(OBJECT_PTR(node)->objectName()); }
    bool isNullNode(NodePtr node) const override
    { return node.ptr == 0; }
    NodePtr parentNode(NodePtr node) const override
    { NodePtr n; n.ptr = isNullNode(node) ? 0 : parentObject(OBJECT_PTR(node)); return n; }
    NodePtr previousSiblingNode(NodePtr) const override
    { NodePtr n; n.ptr = 0; return n; }
    NodePtr duplicateNode(NodePtr node) const override
    { return node; }
    void freeNode(NodePtr) const override
    { }

private:
    mutable QHash<const QObject *, QHash<QString, QString> > m_attributeCache;
};


class QTCSS_EXPORT QStyleSheetCaches : public QObject
{
    Q_OBJECT
public Q_SLOTS:
    void objectDestroyed(QObject *o)
    {
        styleRulesCache.remove(o);
        hasStyleRuleCache.remove(o);
        renderRulesCache.remove(o);
        customPaletteWidgets.remove((const QWidget *)o);
        customFontWidgets.remove(static_cast<QWidget *>(o));
        styleSheetCache.remove(o);
        autoFillDisabledWidgets.remove((const QWidget *)o);
    }

    void styleDestroyed(QObject *o)
    {
        styleSheetCache.remove(o);
    }
public:
    QHash<const QObject *, QVector<QtCss::StyleRule> > styleRulesCache;
    QHash<const QObject *, QHash<int, bool> > hasStyleRuleCache;
    typedef QHash<int, QHash<quint64, QRenderRule> > QRenderRules;
    QHash<const QObject *, QRenderRules> renderRulesCache;
    QHash<const void *, QtCss::StyleSheet> styleSheetCache; // parsed style sheets
    QSet<const QWidget *> autoFillDisabledWidgets;
    // widgets with whose palettes and fonts we have tampered:
    template <typename T>
    struct Tampered {
        T oldWidgetValue;
        uint resolveMask;

        // only call this function on an rvalue *this (it mangles oldWidgetValue)
        T reverted(T current)
#ifdef Q_COMPILER_REF_QUALIFIERS
        &&
#endif
        {
            oldWidgetValue.resolve(oldWidgetValue.resolve() & resolveMask);
            current.resolve(current.resolve() & ~resolveMask);
            current.resolve(oldWidgetValue);
            current.resolve(current.resolve() | oldWidgetValue.resolve());
            return current;
        }
    };
    QHash<const QWidget *, Tampered<QPalette>> customPaletteWidgets;
    QHash<const QWidget *, Tampered<QFont>> customFontWidgets;
};
template <typename T>
class QTypeInfo<QStyleSheetCaches::Tampered<T>>
    : QTypeInfoMerger<QStyleSheetCaches::Tampered<T>, T> {};
#endif // QTSTYLESHEET_H
