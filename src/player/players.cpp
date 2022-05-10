//
// Created by ColorsWind on 2022/5/7.
//
#include "players.h"
#include "hurricane.hpp"
#include "thumbnail.hpp"

void registerPlayerQML() {
    qmlRegisterType<Hurricane>("HurricanePlayer", 1, 0, "HurricanePlayer");
    qmlRegisterType<Thumbnail>("Thumbnail", 1, 0, "Thumbnail");
}