#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QDate>
#include <QMessageBox>

// Библиотеки для Задания А (Всплывающее окно редактирования)
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Личный финансовый трекер (PRO версия)");
    resize(800, 750);
    setupUI();
}

void MainWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QStringList categories = {"Еда", "Транспорт", "Жильё", "Развлечения", "Здоровье", "Одежда", "Другое"};

    // === 1. ВЕРХНЯЯ ЧАСТЬ: ФОРМА ===
    QGroupBox *inputGroup = new QGroupBox("Новый расход");
    QHBoxLayout *inputLayout = new QHBoxLayout();

    categoryCombo = new QComboBox();
    categoryCombo->addItems(categories);
    categoryCombo->setMinimumWidth(120);

    descriptionEdit = new QLineEdit();
    descriptionEdit->setPlaceholderText("Описание...");

    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(0.01, 999999.99);
    amountSpin->setPrefix("₽ ");
    amountSpin->setDecimals(2);

    addBtn = new QPushButton("Добавить");
    addBtn->setStyleSheet("background-color: #27ae60; color: white; font-weight: bold; padding: 6px 16px;");

    inputLayout->addWidget(new QLabel("Категория:"));
    inputLayout->addWidget(categoryCombo);
    inputLayout->addWidget(new QLabel("Описание:"));
    inputLayout->addWidget(descriptionEdit, 1);
    inputLayout->addWidget(new QLabel("Сумма:"));
    inputLayout->addWidget(amountSpin);
    inputLayout->addWidget(addBtn);
    inputGroup->setLayout(inputLayout);
    mainLayout->addWidget(inputGroup);

    // === 2. ТАБЛИЦА (С ЗАДАНИЕМ C: СОРТИРОВКА) ===
    table = new QTableWidget();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Дата", "Категория", "Описание", "Сумма"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);

    // Включаем сортировку по клику на заголовок!
    table->setSortingEnabled(true);
    mainLayout->addWidget(table, 1);

    // === 3. ПАНЕЛЬ ФИЛЬТРА ===
    QHBoxLayout *controlRow = new QHBoxLayout();
    controlRow->addWidget(new QLabel("Фильтр:"));

    filterCombo = new QComboBox();
    filterCombo->addItem("Все");
    filterCombo->addItems(categories);
    controlRow->addWidget(filterCombo);
    controlRow->addStretch();

    deleteBtn = new QPushButton("Удалить выбранное");
    deleteBtn->setStyleSheet("background-color: #e74c3c; color: white; padding: 6px 12px;");
    controlRow->addWidget(deleteBtn);
    mainLayout->addLayout(controlRow);

    // === 4. ОБЩИЕ ИТОГИ ===
    QGroupBox *summaryGroup = new QGroupBox("Итоги");
    QHBoxLayout *summaryLayout = new QHBoxLayout();

    totalLabel = new QLabel("Потрачено: 0.00 ₽");
    budgetLabel = new QLabel("Бюджет: " + QString::number(budget, 'f', 2) + " ₽");
    remainLabel = new QLabel("Остаток: 0.00 ₽");

    summaryLayout->addWidget(totalLabel);
    summaryLayout->addWidget(budgetLabel);
    summaryLayout->addWidget(remainLabel);
    summaryGroup->setLayout(summaryLayout);
    mainLayout->addWidget(summaryGroup);

    // === 5. ПРОГРЕСС-БАРЫ ПО КАТЕГОРИЯМ (ЗАДАНИЕ B) ===
    QGroupBox *categoryGroup = new QGroupBox("Детализация по категориям");
    QVBoxLayout *catLayout = new QVBoxLayout();

    for (const QString &cat : categories) {
        QHBoxLayout *row = new QHBoxLayout();
        QLabel *lbl = new QLabel(cat + ": 0.00 ₽");
        lbl->setMinimumWidth(150);

        QProgressBar *bar = new QProgressBar();
        bar->setRange(0, 100);
        bar->setValue(0);

        row->addWidget(lbl);
        row->addWidget(bar);
        catLayout->addLayout(row);

        categoryLabels[cat] = lbl;
        categoryBars[cat] = bar;
    }
    categoryGroup->setLayout(catLayout);
    mainLayout->addWidget(categoryGroup);

    // ПОДКЛЮЧАЕМ СИГНАЛЫ
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addExpense);
    connect(descriptionEdit, &QLineEdit::returnPressed, this, &MainWindow::addExpense);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteSelected);
    connect(filterCombo, &QComboBox::currentTextChanged, this, &MainWindow::filterByCategory);

    // Подключаем сигнал двойного клика к нашему окну редактирования (ЗАДАНИЕ А)
    connect(table, &QTableWidget::cellDoubleClicked, this, &MainWindow::editExpense);

    updateSummary();
}

void MainWindow::addExpense() {
    QString desc = descriptionEdit->text().trimmed();
    if (desc.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите описание.");
        return;
    }

    Expense e = {categoryCombo->currentText(), desc, amountSpin->value(), QDate::currentDate().toString("dd.MM.yyyy")};
    expenses.append(e);

    descriptionEdit->clear();
    amountSpin->setValue(0.01);
    refreshTable();
    updateSummary();
}

void MainWindow::deleteSelected() {
    int row = table->currentRow();
    if (row < 0) return;

    // ВАЖНО: Из-за сортировки строка 0 в таблице - это НЕ обязательно элемент 0 в векторе.
    // Мы читаем "Секретный ID", который спрятали в ячейку с датой.
    int vectorIndex = table->item(row, 0)->data(Qt::UserRole).toInt();

    expenses.removeAt(vectorIndex);

    refreshTable();
    updateSummary();
}

void MainWindow::refreshTable() {
    // ВАЖНО: Выключаем сортировку во время заполнения, иначе таблица сойдет с ума и всё перемешает!
    table->setSortingEnabled(false);

    QString filter = filterCombo->currentText();
    table->setRowCount(0);

    for (int i = 0; i < expenses.size(); ++i) {
        const Expense &e = expenses[i];
        if (filter != "Все" && e.category != filter) continue;

        int row = table->rowCount();
        table->insertRow(row);

        QTableWidgetItem *dateItem = new QTableWidgetItem(e.date);
        // ПРЯЧЕМ ИНДЕКС ВЕКТОРА В ЯЧЕЙКУ (чтобы легко находить данные при удалении/редактировании)
        dateItem->setData(Qt::UserRole, i);
        table->setItem(row, 0, dateItem);

        table->setItem(row, 1, new QTableWidgetItem(e.category));
        table->setItem(row, 2, new QTableWidgetItem(e.description));

        // Используем наш кастомный класс для сортировки сумм
        NumericTableWidgetItem *amountItem = new NumericTableWidgetItem(QString::number(e.amount, 'f', 2) + " ₽");
        amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        table->setItem(row, 3, amountItem);
    }

    // Включаем сортировку обратно
    table->setSortingEnabled(true);
}

// === ЛОГИКА РЕДАКТИРОВАНИЯ (ЗАДАНИЕ A) ===
void MainWindow::editExpense(int row, int column) {
    Q_UNUSED(column); // Колонка нам не важна, главное строка

    // Узнаем, какой элемент вектора мы реально кликнули (читаем наш секретный ID)
    int vectorIndex = table->item(row, 0)->data(Qt::UserRole).toInt();
    Expense &e = expenses[vectorIndex]; // Получаем ссылку на оригинальную запись

    // Создаем диалоговое окно
    QDialog dialog(this);
    dialog.setWindowTitle("Редактирование расхода");
    dialog.resize(300, 200);

    // Элементы формы
    QComboBox *editCategory = new QComboBox();
    editCategory->addItems({"Еда", "Транспорт", "Жильё", "Развлечения", "Здоровье", "Одежда", "Другое"});
    editCategory->setCurrentText(e.category); // Ставим старое значение

    QLineEdit *editDesc = new QLineEdit();
    editDesc->setText(e.description); // Ставим старое значение

    QDoubleSpinBox *editAmount = new QDoubleSpinBox();
    editAmount->setRange(0.01, 999999.99);
    editAmount->setPrefix("₽ ");
    editAmount->setValue(e.amount); // Ставим старое значение

    // Кнопки Сохранить / Отмена
    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    connect(btnBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // Вертикальная компоновка (Форма)
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Категория:", editCategory);
    formLayout->addRow("Описание:", editDesc);
    formLayout->addRow("Сумма:", editAmount);

    QVBoxLayout *mainDlgLayout = new QVBoxLayout();
    mainDlgLayout->addLayout(formLayout);
    mainDlgLayout->addWidget(btnBox);
    dialog.setLayout(mainDlgLayout);

    // Запускаем окно и ждем. Если нажали "Сохранить" (Accepted):
    if (dialog.exec() == QDialog::Accepted) {
        // Обновляем данные в векторе
        e.category = editCategory->currentText();
        e.description = editDesc->text();
        e.amount = editAmount->value();

        refreshTable();
        updateSummary();
    }
}

void MainWindow::updateSummary() {
    double total = 0.0;
    QMap<QString, double> categoryTotals;

    for (const auto &e : expenses) {
        total += e.amount;
        categoryTotals[e.category] += e.amount;
    }

    double remain = budget - total;

    totalLabel->setText("Потрачено: " + QString::number(total, 'f', 2) + " ₽");
    remainLabel->setText("Остаток: " + QString::number(remain, 'f', 2) + " ₽");

    if (remain < 0) remainLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #e74c3c;");
    else if (remain < budget * 0.2) remainLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #f39c12;");
    else remainLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #27ae60;");

    for (auto it = categoryLabels.begin(); it != categoryLabels.end(); ++it) {
        QString cat = it.key();
        double catSum = categoryTotals.value(cat, 0.0);
        it.value()->setText(cat + ": " + QString::number(catSum, 'f', 2) + " ₽");

        int percent = (total > 0) ? static_cast<int>((catSum / total) * 100) : 0;
        categoryBars[cat]->setValue(percent);
    }
}

void MainWindow::filterByCategory(const QString &category) {
    Q_UNUSED(category);
    refreshTable();
}
