#include "MainWindow.h"
#include <QInputDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      table(new QTableWidget),
      controller(new MonitorController(this))
{
    auto *central = new QWidget;
    auto *layout = new QVBoxLayout(central);

    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Host", "Status", "RTT (ms)"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    addHostButton = new QPushButton("Add Host");
    removeHostButton = new QPushButton("Remove Host");

    layout->addWidget(table);
    layout->addWidget(addHostButton);
    layout->addWidget(removeHostButton);
    setCentralWidget(central);

    connect(addHostButton, &QPushButton::clicked, this, &MainWindow::onAddHost);
    connect(removeHostButton, &QPushButton::clicked, this, &MainWindow::onRemoveHost);
    connect(controller, &MonitorController::hostChecked, this, &MainWindow::onHostChecked);
    connect(table, &QTableWidget::cellClicked, this, &MainWindow::onTableCellClicked);
}

MainWindow::~MainWindow() {
    // Очистим все открытые окна графиков
    qDeleteAll(openCharts);
    openCharts.clear();
}

void MainWindow::onAddHost() {
    bool ok;
    QString host = QInputDialog::getText(this, "Add Host", "Hostname:", QLineEdit::Normal, "", &ok);
    if (ok && !host.isEmpty()) {
        controller->addHost(host);
    }
}

void MainWindow::onRemoveHost() {
    int row = table->currentRow();
    if (row < 0)
        return;

    QString host = table->item(row, 0)->text();

    controller->removeHost(host);

    table->removeRow(row);

    if (openCharts.contains(host)) 
    {
        openCharts[host]->close();
        delete openCharts[host];
        openCharts.remove(host);
    }
}


void MainWindow::updateResult(const QString &host, bool alive, int rtt) {
    // Поиск строки с хостом
    for (int i = 0; i < table->rowCount(); ++i) {
        if (table->item(i, 0)->text() == host) {
            table->item(i, 1)->setText(alive ? "🟢" : "🔴");
            table->item(i, 2)->setText(alive ? QString::number(rtt) : "");
            return;
        }
    }
    // Если не найдено, добавить новую строку
    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(host));
    table->setItem(row, 1, new QTableWidgetItem(alive ? "🟢" : "🔴"));
    table->setItem(row, 2, new QTableWidgetItem(alive ? QString::number(rtt) : ""));
}

void MainWindow::onTableCellClicked(int row, int /*column*/) {
    QString host = table->item(row, 0)->text();
    const auto &history = controller->getRttHistory(host);
    if (history.isEmpty()) {
        return; // Нет данных - не показываем
    }

    // Если окно с графиком уже открыто, просто покажем его
    if (openCharts.contains(host)) {
        openCharts[host]->show();
        openCharts[host]->raise();
        openCharts[host]->activateWindow();
        return;
    }

    // Создаем и показываем новое окно графика
    auto *chart = new ChartWindow(host, history);
    openCharts.insert(host, chart);

    // Когда окно закроется, удаляем его из map
    connect(chart, &QWidget::destroyed, this, [this, host]() {
        openCharts.remove(host);
    });

    chart->show();
}

void MainWindow::onHostChecked(const QString &host, bool alive, int rtt) {
    updateResult(host, alive, rtt);

    if (alive && openCharts.contains(host)) {
        openCharts[host]->appendRtt(rtt);
    }
}
