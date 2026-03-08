#include "tankwidget.h"

TankWidget::TankWidget(QWidget *parent) : QWidget(parent) {
    level = 0;
    setMinimumSize(100, 200); // Минимальный размер виджета
}

void TankWidget::setLevel(int val) {
    level = val;
    update(); // перерисовка виджета (paintEvent)
}

void TankWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // Сглаживание краев

    int w = width();
    int h = height();

    // внешний контур резервуара
    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(QColor(236, 240, 241)); // Светло-серый фон
    painter.drawRect(10, 10, w - 20, h - 20);

    // внешний цвет жидкости
    QColor liquidColor = QColor(46, 204, 113); // Зеленый
    if (level > 90) {
        liquidColor = QColor(231, 76, 60);     // Красный
    } else if (level > 80) {
        liquidColor = QColor(230, 126, 34);    // Оранжевый
    }

    // сама жидкость (чем выше жидкость тем больше прямоугольник)
    int padding = 12;
    int fillHeight = ((h - padding * 2) * level) / 100;
    int fillY = h - padding - fillHeight;

    painter.setPen(Qt::NoPen); // Жидкость без рамки
    painter.setBrush(liquidColor);
    if (level > 0) {
        painter.drawRect(padding, fillY, w - padding * 2, fillHeight);
    }
}
