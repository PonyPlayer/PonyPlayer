/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#include <lyrics.h>
#include <wave.hpp>

#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif

Q_QMLTYPE_EXPORT void qml_register_types_wave()
{
    qmlRegisterTypesAndRevisions<LyricSentence>("wave", 1);
    qmlRegisterTypesAndRevisions<LyricsData>("wave", 1);
    qmlRegisterTypesAndRevisions<WaveView>("wave", 1);
    qmlRegisterModule("wave", 1, 0);
}

static const QQmlModuleRegistration registration("wave", qml_register_types_wave);
