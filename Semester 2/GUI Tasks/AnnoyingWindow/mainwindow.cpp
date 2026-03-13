#include "mainwindow.h"
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QGuiApplication>
#include <QScreen>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>

// ==========================================
// ЛОГИКА УБЕГАЮЩЕЙ КНОПКИ (Фаза 1)
// ==========================================
RunawayButton::RunawayButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent) {
    setStyleSheet("background-color: red; color: white; font-size: 20px; font-weight: bold; border-radius: 5px;");
    setFixedSize(120, 50);
}

void RunawayButton::enterEvent(QEnterEvent *event) {
    if (!parentWidget()) return;

    int parentWidth = parentWidget()->width();
    int parentHeight = parentWidget()->height();

    int newX = 0, newY = 0;
    int currentX = x();
    int currentY = y();

    // Ищем новую точку, которая минимум на 100px дальше текущей
    do {
        newX = QRandomGenerator::global()->bounded(parentWidth - width());
        newY = QRandomGenerator::global()->bounded(parentHeight - height());
    } while (abs(newX - currentX) < 100 && abs(newY - currentY) < 100);

    move(newX, newY); // Прыгаем!
    QPushButton::enterEvent(event);
}

// ==========================================
// ЛОГИКА ОКОН-КЛОНОВ (Фаза 3)
// ==========================================
CloneWindow::CloneWindow(bool isSpecial) : special(isSpecial), isSurrendering(false) {
    setAttribute(Qt::WA_DeleteOnClose); // Удалить объект из памяти при закрытии
    setFixedSize(250, 150);
    setWindowTitle("Клон");

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(special ? "О, ты меня нашел!" : "Не это окно! Попробуй другое.");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    if (special) {
        setStyleSheet("background-color: #e0ffe0;"); // Слегка зеленоватый фон
        QPushButton *btn = new QPushButton("Сдаюсь, ты победил!");
        layout->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, [this]() {
            isSurrendering = true; // Чтобы не плодить клонов при закрытии
            emit surrendered();
        });
    } else {
        setStyleSheet("background-color: white;");
    }
}

void CloneWindow::closeEvent(QCloseEvent *event) {
    if (!isSurrendering) {
        emit closedByPlayer(special); // Говорим главному окну, что нас убили
    }
    event->accept();
}

// ==========================================
// ЛОГИКА ГЛАВНОГО ОКНА
// ==========================================
MainWindow::MainWindow(QWidget *parent) : QWidget(parent), cloneCount(0), isWon(false) {
    setFixedSize(500, 400);
    setWindowTitle("Головоломка");

    stackedWidget = new QStackedWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(stackedWidget);

    setupPhase1();
    setupPhase2();
    setupPhase3();
    setupPhase4();
}

// --- ФАЗА 1 ---
void MainWindow::setupPhase1() {
    phase1Widget = new QWidget();
    QLabel *instr = new QLabel("Нажми ВЫХОД, чтобы закрыть", phase1Widget);
    instr->setAlignment(Qt::AlignCenter);
    instr->setGeometry(0, 50, 500, 50); // Абсолютное позиционирование

    runawayBtn = new RunawayButton("ВЫХОД", phase1Widget);
    runawayBtn->move(190, 175); // По центру

    QLabel *hint = new QLabel("Подсказка: мышка — не единственный способ управления...", phase1Widget);
    hint->setStyleSheet("color: gray; font-size: 10px;");
    hint->setGeometry(10, 370, 480, 20);

    // Если получилось нажать (через Tab + Enter)
    connect(runawayBtn, &QPushButton::clicked, this, &MainWindow::goToPhase2);

    stackedWidget->addWidget(phase1Widget);
}

void MainWindow::goToPhase2() {
    stackedWidget->setCurrentIndex(1);
    setWindowTitle("Убегающее окно");
}

// --- ФАЗА 2 ---
void MainWindow::setupPhase2() {
    phase2Widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(phase2Widget);

    QLabel *text = new QLabel("Ладно, просто закрой окно крестиком (X).");
    text->setAlignment(Qt::AlignCenter);
    layout->addWidget(text);

    hiddenInput = new QLineEdit();
    hiddenInput->hide(); // Спрятано!
    layout->addWidget(hiddenInput);

    phase2Feedback = new QLabel("");
    phase2Feedback->setStyleSheet("color: red;");
    layout->addWidget(phase2Feedback);

    QLabel *hint = new QLabel("Подсказка: Ctrl+Q");
    hint->setStyleSheet("color: #f0f0f0;"); // Почти сливается с фоном
    hint->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    layout->addWidget(hint);

    connect(hiddenInput, &QLineEdit::returnPressed, this, &MainWindow::checkPhase2Word);

    stackedWidget->addWidget(phase2Widget);
}

void MainWindow::checkPhase2Word() {
    if (hiddenInput->text() == "quit") {
        startPhase3();
    } else {
        hiddenInput->clear();
        phase2Feedback->setText("Не-а, попробуй ещё");
    }
}

// --- ФАЗА 3 ---
void MainWindow::setupPhase3() {
    phase3Widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(phase3Widget);
    QLabel *lbl = new QLabel("Ты думал, это конец?\nИщи правильное окно!");
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet("font-size: 24px; font-weight: bold;");
    layout->addWidget(lbl);
    stackedWidget->addWidget(phase3Widget);
}

void MainWindow::startPhase3() {
    stackedWidget->setCurrentIndex(2);
    // Спавним 4 обычных и 1 особое окно
    for(int i=0; i<4; i++) spawnClone(false);
    spawnClone(true);
}

void MainWindow::spawnClone(bool isSpecial) {
    if (cloneCount >= 20) return; // Защита от зависания (требование ТЗ)

    CloneWindow *clone = new CloneWindow(isSpecial);

    // Раскидываем окна по всему экрану
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    int x = QRandomGenerator::global()->bounded(screenGeometry.width() - 250);
    int y = QRandomGenerator::global()->bounded(screenGeometry.height() - 150);
    clone->move(x, y);
    clone->show();

    cloneCount++;
    clones.append(clone);

    // Связываем сигналы
    connect(clone, &CloneWindow::closedByPlayer, this, [this, clone](bool wasSpecial) {
        cloneCount--;
        clones.removeOne(clone); // <--- ИСПРАВЛЕНИЕ: Убираем убитого клона из списка!

        // Наказание: закрыл крестиком — получай два новых
        spawnClone(false);
        spawnClone(wasSpecial);
    });

    connect(clone, &CloneWindow::surrendered, this, &MainWindow::onCloneSurrendered);
}

void MainWindow::onCloneSurrendered() {
    // Безопасно прячем и удаляем все оставшиеся клоны
    for (CloneWindow *c : clones) {
        if (c) {
            c->hide();
            c->deleteLater();
        }
    }
    clones.clear();

    stackedWidget->setCurrentIndex(3); // Переход к боссу

    // === ДОБАВЬ ВОТ ЭТИ 3 СТРОЧКИ ===
    // Запускаем безумие босса сразу при переходе!
    moveTimer->start(400);
    resizeTimer->start(500);
    titleTimer->start(200);
}

// --- ФАЗА 4 (БОСС) ---
void MainWindow::setupPhase4() {
    // --- ИСПРАВЛЕНИЕ: Снимаем жесткую блокировку размера окна! ---
    setMinimumSize(0, 0);                 // Разрешаем сжимать окно
    setMaximumSize(16777215, 16777215);   // Разрешаем растягивать (это макс. значение в Qt)
    // --------------------------------------------------------------

    phase4Widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(phase4Widget);

    QLabel *lbl = new QLabel("ФИНАЛЬНЫЙ БОСС\nЧто-то пошло не так...");
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet("font-size: 30px; font-weight: bold; color: purple;");
    layout->addWidget(lbl);
    stackedWidget->addWidget(phase4Widget);

    titleWord = "ПРОБЕЛ";
    titleIndex = 1;

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &MainWindow::phase4Move);

    resizeTimer = new QTimer(this);
    connect(resizeTimer, &QTimer::timeout, this, &MainWindow::phase4Resize);

    titleTimer = new QTimer(this);
    connect(titleTimer, &QTimer::timeout, this, &MainWindow::phase4TitleBlink);
}

void MainWindow::phase4Move() {
    QRect screen = QGuiApplication::primaryScreen()->availableGeometry();
    int x = QRandomGenerator::global()->bounded(screen.width() - width());
    int y = QRandomGenerator::global()->bounded(screen.height() - height());
    move(x, y);
}

void MainWindow::phase4Resize() {
    int w = QRandomGenerator::global()->bounded(200, 800);
    int h = QRandomGenerator::global()->bounded(150, 600);
    resize(w, h);
}

void MainWindow::phase4TitleBlink() {
    setWindowTitle(titleWord.left(titleIndex));
    titleIndex++;
    if (titleIndex > titleWord.length()) titleIndex = 1;
}

// --- ПОБЕДА ---
void MainWindow::winGame() {
    isWon = true;
    moveTimer->stop();
    resizeTimer->stop();
    titleTimer->stop();

    setWindowTitle("Победа!");

    // Очищаем окно фазы 4 и показываем экран победы
    qDeleteAll(phase4Widget->children());
    QVBoxLayout *layout = new QVBoxLayout(phase4Widget);

    QLabel *winText = new QLabel("🎉 ПОЗДРАВЛЯЮ! ТЫ ПОБЕДИЛ ОКНО! 🎉");
    winText->setAlignment(Qt::AlignCenter);
    winText->setStyleSheet("font-size: 24px; color: green; font-weight: bold;");
    layout->addWidget(winText);

    QPushButton *realExitBtn = new QPushButton("Закрыть по-настоящему");
    realExitBtn->setStyleSheet("padding: 15px; font-size: 18px;");
    layout->addWidget(realExitBtn);
    connect(realExitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
}

// ==========================================
// ПЕРЕХВАТ СИСТЕМНЫХ СОБЫТИЙ (Крестик и Кнопки)
// ==========================================
void MainWindow::closeEvent(QCloseEvent *event) {
    int phase = stackedWidget->currentIndex();

    if (phase == 1) { // Фаза 2: окно убегает от крестика
        event->ignore();
        phase4Move(); // Используем ту же функцию для прыжка
    }
    else if (phase == 2) { // Фаза 3: главное окно нельзя закрыть
        event->ignore();
    }
    else if (phase == 3 && !isWon) { // Фаза 4: пока босс жив, крестик не работает
        event->ignore();
    }
    else {
        event->accept(); // Разрешаем закрыть в 1 фазе или если победил
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int phase = stackedWidget->currentIndex();

    if (phase == 1) { // Ловим Ctrl+Q на второй фазе
        if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q) {
            hiddenInput->show();
            hiddenInput->setFocus();
        }
    }
    else if (phase == 3 && !isWon) { // Ловим Пробел на боссе
        if (event->key() == Qt::Key_Space) {
            winGame(); // Если нажали пробел - побеждаем!
        }
    }

    QWidget::keyPressEvent(event);
}
