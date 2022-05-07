//
// Created by ColorsWind on 2022/5/7.
//
#include "players.h"
#include "hurricane.hpp"

void registerPlayerQML() {
    qmlRegisterType<Hurricane>("HurricanePlayer", 1, 0, "HurricanePlayer");
}