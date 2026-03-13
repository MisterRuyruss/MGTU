#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QVector>
#include <QProgressBar>
#include <QMap>

// Структура расхода
struct Expense {
    QString category;
    QString description;
    double amount;
    QString date;
};

// === ЗАДАНИЕ C: КАСТОМНАЯ ЯЧЕЙКА ДЛЯ ПРАВИЛЬНОЙ СОРТИРОВКИ ЧИСЕЛ ===
// Стандартная ячейка сортирует текст ("100" будет меньше "20", потому что '1' < '2').
// Мы переопределяем оператор <, чтобы он переводил текст в числа перед сравнением.
class NumericTableWidgetItem : public QTableWidgetItem {
public:
    NumericTableWidgetItem(const QString &text) : QTableWidgetItem(text) {}

    bool operator<(const QTableWidgetItem &other) const override {
        // Убираем символ " ₽" и пробелы, превращаем в double и сравниваем
        double myValue = this->text().remove(" ₽").toDouble();
        double otherValue = other.text().remove(" ₽").toDouble();
        return myValue < otherValue;
    }
};

// Главное окно
class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void addExpense();
    void deleteSelected();
    void updateSummary();
    void filterByCategory(const QString &category);

    // === ЗАДАНИЕ A: СЛОТ ДЛЯ РЕДАКТИРОВАНИЯ ===
    void editExpense(int row, int column);

private:
    void setupUI();
    void refreshTable();

    QVector<Expense> expenses;
    double budget = 50000.0;

    QComboBox *categoryCombo;
    QLineEdit *descriptionEdit;
    QDoubleSpinBox *amountSpin;
    QPushButton *addBtn;
    QPushButton *deleteBtn;

    QTableWidget *table;
    QComboBox *filterCombo;

    QLabel *totalLabel;
    QLabel *budgetLabel;
    QLabel *remainLabel;

    // Словари для Задания B (Прогресс-бары)
    QMap<QString, QProgressBar*> categoryBars;
    QMap<QString, QLabel*> categoryLabels;
};

#endif // MAINWINDOW_H
