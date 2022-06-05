/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#include <fireworks.hpp>
#include <hurricane.hpp>
#include <thumbnail.hpp>

#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif

Q_QMLTYPE_EXPORT void qml_register_types_player()
{
    qmlRegisterTypesAndRevisions<Fireworks>("player", 1);
    qmlRegisterTypesAndRevisions<Hurricane>("player", 1);
    qmlRegisterTypesAndRevisions<Thumbnail>("player", 1);
    qmlRegisterModule("player", 1, 0);
}

static const QQmlModuleRegistration registration("player", qml_register_types_player);
