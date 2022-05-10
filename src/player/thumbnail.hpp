//
// Created by ColorsWind on 2022/5/10.
//

#pragma once

#include <QQmlContext>
#include "fireworks.hpp"
#include "hurricane.hpp"

class Thumbnail : public Fireworks {
    Q_OBJECT
public:
    Thumbnail(QQuickItem *parent) : Fireworks(parent) {
        auto id = this->property("hurricane");
        auto *hurricane = reinterpret_cast<Hurricane *>(qmlContext(parent)->objectForName(id.toString()));
        if (!hurricane) {
            throw std::runtime_error("Cannot not get Hurricane by id:" + id.toString().toStdString());
        }

    }
};
