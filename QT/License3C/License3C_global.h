#ifndef LICENSE3C_GLOBAL_H
#define LICENSE3C_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LICENSE3C_LIBRARY)
#  define LICENSE3C_EXPORT Q_DECL_EXPORT
#else
#  define LICENSE3C_EXPORT Q_DECL_IMPORT
#endif

#endif // LICENSE3C_GLOBAL_H
