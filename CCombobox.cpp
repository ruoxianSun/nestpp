#include "CCombobox.h"
#include "CMyStyle.h"


CCombobox::CCombobox(QWidget *parent) : QComboBox(parent)
{
    setStyle(new CMyStyle);
}
