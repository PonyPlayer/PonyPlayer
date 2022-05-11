/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#include <quickitem.h>

#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif

Q_QMLTYPE_EXPORT void qml_register_types_quickitem()
{
    qmlRegisterTypesAndRevisions<HurricanePlayer>("quickitem", 1);
    qmlRegisterModule("quickitem", 1, 0);
}

static const QQmlModuleRegistration registration("quickitem", qml_register_types_quickitem);
