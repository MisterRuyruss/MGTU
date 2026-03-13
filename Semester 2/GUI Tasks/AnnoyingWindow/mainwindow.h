#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QList>

// ==========================================
// КЛАСС 1: Убегающая кнопка (Для Фазы 1)
// ==========================================
class RunawayButton : public QPushButton {
    Q_OBJECT
public:
    RunawayButton(const QString &text, QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *event) override; // Отслеживаем наведение мыши
};

// ==========================================
// КЛАСС 2: Окно-клон (Для Фазы 3)
// ==========================================
class CloneWindow : public QWidget {
    Q_OBJECT
public:
    CloneWindow(bool isSpecial);

signals:
    void closedByPlayer(bool wasSpecial); // Сигнал, если игрок закрыл крестиком
    void surrendered();                   // Сигнал, если игрок нажал кнопку в особом окне

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    bool special;
    bool isSurrendering;
};

// ==========================================
// КЛАСС 3: Главное окно игры
// ==========================================
class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    // Переопределяем системные события
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void goToPhase2();
    void checkPhase2Word();
    void startPhase3();
    void spawnClone(bool isSpecial);
    void onCloneSurrendered();

    // Слоты для таймеров (Фаза 4)
    void phase4Move();
    void phase4Resize();
    void phase4TitleBlink();

private:
    void setupPhase1();
    void setupPhase2();
    void setupPhase3();
    void setupPhase4();
    void winGame();

    QStackedWidget *stackedWidget;

    // Фаза 1
    QWidget *phase1Widget;
    RunawayButton *runawayBtn;

    // Фаза 2
    QWidget *phase2Widget;
    QLineEdit *hiddenInput;
    QLabel *phase2Feedback;

    // Фаза 3
    QWidget *phase3Widget;
    QList<CloneWindow*> clones;
    int cloneCount;

    // Фаза 4
    QWidget *phase4Widget;
    QTimer *moveTimer;
    QTimer *resizeTimer;
    QTimer *titleTimer;
    QString titleWord;
    int titleIndex;
    bool isWon;
};

#endif // MAINWINDOW_H
