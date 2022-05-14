#ifndef PONYPLAYER_CURSOR_H
#define PONYPLAYER_CURSOR_H

#include <QtCore>
#include <QQmlApplicationEngine>
#include <QQuickView>

class CursorPosProvider : public QObject
{
Q_OBJECT
public:
    explicit CursorPosProvider(QObject *parent = nullptr) : QObject(parent)
    {
    }
    virtual ~CursorPosProvider() = default;

    Q_INVOKABLE QPointF cursorPos()
    {
        return QCursor::pos();
    }
};