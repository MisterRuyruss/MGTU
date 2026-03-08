#ifndef TANKMODEL_H
#define TANKMODEL_H

#include <QObject>
#include <QTimer>

class TankModel : public QObject {
    Q_OBJECT // Макрос для работы сигналов и слотов

public:
    explicit TankModel(QObject *parent = nullptr);

    // Текущий уровень
    double getLevel() const { return currentLevel; }

public slots:
    // Слоты для приема значений от ползунков
    void setInflow(int val);
    void setDrain(int val);
    void reset(); // Аварийный сброс

signals:
    // Сигналы для отправки в интерфейс
    void levelChanged(int newLevel);
    void alarmTriggered(int statusMode); // 0 - Норма, 1 - Внимание, 2 - Тревога

private slots:
    void updatePhysics(); // Метод, вызываемый таймером

private:
    QTimer *simTimer;
    double currentLevel;
    double inflow;
    double drain; // Клапан сброса (Задание А)
    double leak;
    int currentStatus; // Чтобы не спамить лог каждую миллисекунду
};

#endif // TANKMODEL_H
