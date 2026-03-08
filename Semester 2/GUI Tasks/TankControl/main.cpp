#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QLCDNumber>
#include <QPushButton>
#include <QTextEdit>
#include <QTime>

#include "tankmodel.h"
#include "tankwidget.h"

// Функция для записи в лог с временем
void logEvent(QTextEdit *logArea, const QString &message) {
    QString timeStr = QTime::currentTime().toString("hh:mm:ss");
    logArea->append(QString("[%1] %2").arg(timeStr, message));
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Создание главного окна
    QWidget window;
    window.setWindowTitle("Продвинутый пульт управления резервуаром");
    window.resize(650, 600);

    QLabel *titleLabel = new QLabel("РЕЗЕРВУАР A-1 ");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    QLabel *statusLabel = new QLabel("Статус: НОРМА");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 14px; padding: 6px; background-color: #27ae60; color: white; border-radius: 4px;");

    // Ползунок подачи
    QLabel *sliderLabel = new QLabel("Клапан подачи (IN):");
    QSlider *inputSlider = new QSlider(Qt::Horizontal);
    inputSlider->setRange(0, 100);

    // Ползунок сброса
    QLabel *drainSliderLabel = new QLabel("Клапан оттока (OUT):");
    QSlider *drainSlider = new QSlider(Qt::Horizontal);
    drainSlider->setRange(0, 100);

    // Наш виджет
    TankWidget *customTank = new TankWidget();

    QLCDNumber *lcdDisplay = new QLCDNumber();
    lcdDisplay->setDigitCount(4);
    lcdDisplay->setMinimumHeight(60);

    QPushButton *normalDrainBtn = new QPushButton("Обычный сброс");
    QPushButton *resetBtn = new QPushButton("АВАРИЙНЫЙ СБРОС");
    resetBtn->setStyleSheet("background-color: #e74c3c; color: white; font-weight: bold; padding: 8px;");

    // Журнал событий
    QTextEdit *eventLog = new QTextEdit();
    eventLog->setReadOnly(true); // Только для чтения
    eventLog->setMaximumHeight(100);

    // Компановка - связывание сигналов и слотов
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(statusLabel);

    QHBoxLayout *valvesLayout = new QHBoxLayout();

    QVBoxLayout *inflowLayout = new QVBoxLayout();
    inflowLayout->addWidget(sliderLabel);
    inflowLayout->addWidget(inputSlider);

    QVBoxLayout *outflowLayout = new QVBoxLayout();
    outflowLayout->addWidget(drainSliderLabel);
    outflowLayout->addWidget(drainSlider);

    valvesLayout->addLayout(inflowLayout);
    valvesLayout->addLayout(outflowLayout);
    mainLayout->addLayout(valvesLayout);

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addWidget(customTank, 1); // Даем хранилищу больше места
    centerLayout->addWidget(lcdDisplay, 1);
    mainLayout->addLayout(centerLayout);

    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->addWidget(normalDrainBtn);
    btnRow->addWidget(resetBtn);
    mainLayout->addLayout(btnRow);

    mainLayout->addWidget(new QLabel("Журнал событий:"));
    mainLayout->addWidget(eventLog);

    window.setLayout(mainLayout);

    // Связь интерфейса и модели
    // Создание модели
    TankModel *model = new TankModel(&window);

    // Соединение ползунков с моделью
    QObject::connect(inputSlider, &QSlider::valueChanged, model, &TankModel::setInflow);
    QObject::connect(drainSlider, &QSlider::valueChanged, model, &TankModel::setDrain);

    // Новый уровень модели - обновление хранлища и цифр
    QObject::connect(model, &TankModel::levelChanged, customTank, &TankWidget::setLevel);
    QObject::connect(model, &TankModel::levelChanged, lcdDisplay, QOverload<int>::of(&QLCDNumber::display));

    // Обработка аварийных статусов из модели
    QObject::connect(model, &TankModel::alarmTriggered, [&](int status) {
        if (status == 2) {
            statusLabel->setText("!!! КРИТИЧЕСКИЙ УРОВЕНЬ !!!");
            statusLabel->setStyleSheet("font-size: 14px; padding: 6px; background-color: #e74c3c; color: white; border-radius: 4px;");
            logEvent(eventLog, "ВНИМАНИЕ! Уровень превысил 90%!");
        } else if (status == 1) {
            statusLabel->setText("ВНИМАНИЕ: высокий уровень");
            statusLabel->setStyleSheet("font-size: 14px; padding: 6px; background-color: #e67e22; color: white; border-radius: 4px;");
            logEvent(eventLog, "Предупреждение: уровень достиг 80%");
        } else {
            statusLabel->setText("Статус: НОРМА");
            statusLabel->setStyleSheet("font-size: 14px; padding: 6px; background-color: #27ae60; color: white; border-radius: 4px;");
            logEvent(eventLog, "Уровень нормализовался.");
        }
    });

    // Кнопки
    QObject::connect(normalDrainBtn, &QPushButton::clicked, [&]() {
        inputSlider->setValue(0);
        drainSlider->setValue(100); // Открываем клапан на максимум
        logEvent(eventLog, "Оператор включил обычный сброс.");
    });

    QObject::connect(resetBtn, &QPushButton::clicked, [&]() {
        inputSlider->setValue(0);
        drainSlider->setValue(0);
        model->reset(); // Мгновенный сброс в модели
        logEvent(eventLog, "АВАРИЙНЫЙ СБРОС АКТИВИРОВАН!");
    });

    logEvent(eventLog, "Система запущена.");
    window.show();
    return app.exec();
}
