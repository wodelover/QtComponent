#ifndef TCPSERVERCOM_GLOBAL_H
#define TCPSERVERCOM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TCPSERVERCOM_LIBRARY)
#  define TCPSERVERCOMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TCPSERVERCOMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TCPSERVERCOM_GLOBAL_H