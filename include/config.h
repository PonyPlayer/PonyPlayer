#pragma once
#include <QtCore>

class Config : public QObject {
Q_OBJECT
private:
    QString userName;
};