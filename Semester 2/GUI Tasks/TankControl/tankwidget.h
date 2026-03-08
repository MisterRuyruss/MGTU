#ifndef TANKWIDGET_H
#define TANKWIDGET_H

#include <QWidget>
#include <QPainter>

class TankWidget : public QWidget {
    Q_OBJECT
public:
    explicit TankWidget(QWidget *parent = nullptr);

public slots:
    void setLevel(int val); // Сюда будет приходить сигнал от TankModel

protected:
    void paintEvent(QPaintEvent *event) override; // Переопределяем отрисовку

private:
    int level;
};

#endif // TANKWIDGET_H
