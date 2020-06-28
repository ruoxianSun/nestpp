#ifndef CCHECKCOMBOBOX_H
#define CCHECKCOMBOBOX_H

#include "CCombobox.h"
#include <QMouseEvent>
#include <QAbstractItemView>
#include <QItemSelectionModel>

class CCheckComboBox : public CCombobox
{
    Q_OBJECT
public:
    explicit CCheckComboBox(QWidget *parent = 0);
    void appendItem(const QString &text, bool bChecked);
    void hidePopup();
    void addItems(const QStringList&list,bool c=false);
protected:
    void mousePressEvent(QMouseEvent *event);

private:
    void updateIndexStatus(int index);

signals:
    void checkedStateChange(int index, bool bChecked);

public slots:

};

#endif // CCHECKCOMBOBOX_H
