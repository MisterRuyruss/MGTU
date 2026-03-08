#include "tankmodel.h"

TankModel::TankModel(QObject *parent) : QObject(parent) {
    currentLevel = 0.0;
    inflow = 0.0;
    drain = 0.0;
    leak = 1.0;
    currentStatus = 0;

    simTimer = new QTimer(this);
    simTimer->setInterval(100);
    connect(simTimer, &QTimer::timeout, this, &TankModel::updatePhysics);
    simTimer->start();
}

void TankModel::setInflow(int val) {
    inflow = val * 0.3; // Коэффициент притока
}

void TankModel::setDrain(int val) {
    drain = val * 0.3;  // Коэффициент сброса
}

void TankModel::reset() {
    currentLevel = 0.0;
    emit levelChanged(0);
}

void TankModel::updatePhysics() {
    // приток - утечка - принудительный сброс
    currentLevel += (inflow - leak - drain) * 0.1;

    if (currentLevel < 0) currentLevel = 0;
    if (currentLevel > 100) currentLevel = 100;

    emit levelChanged(static_cast<int>(currentLevel));

    // Логика состояний (отправление сигнала только при смене статуса)
    int newStatus = 0;
    if (currentLevel > 90) newStatus = 2;
    else if (currentLevel > 80) newStatus = 1;

    if (newStatus != currentStatus) {
        currentStatus = newStatus;
        emit alarmTriggered(currentStatus);
    }
}
