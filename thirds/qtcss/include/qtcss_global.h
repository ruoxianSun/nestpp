#ifndef QTCSS_GLOBAL_H
#define QTCSS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTCSS_LIBRARY)
#  define QTCSS_EXPORT Q_DECL_EXPORT
#else
#  define QTCSS_EXPORT Q_DECL_IMPORT
#endif

#endif // QTCSS_GLOBAL_H
