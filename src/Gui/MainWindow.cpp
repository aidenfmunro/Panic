#include "Gui/MainWindow.hpp"
#include "Core/host.hpp"
#include "Net/portScanner.hpp"

#include <QComboBox>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      table(new QTableWidget),
      controller(new MonitorController(this))
{
    auto *central = new QWidget;
    auto *layout = new QVBoxLayout(central);

    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Host", "Status", "Port", "Actions"});
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

//     int startPort = QInputDialog::getInt(this, "Port Range", "Start Port:", 20, 1, 65535, 1, &ok);
//     if (!ok)
//         return;
//
//     int endPort = QInputDialog::getInt(this, "Port Range", "End Port:", 100, startPort, 65535, 1, &ok);
//     if (!ok)
//         return;
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


void MainWindow::updateResult(const QString &hostName, bool alive, int rtt) {
    // Поиск строки с хостом
    for (int i = 0; i < table->rowCount(); ++i) {
        if (table->item(i, 0)->text() == hostName) {
            table->item(i, 1)->setText(alive ? "🟢" : "🔴");
            // table->item(i, 2)->setText(alive ? QString::number(rtt) : ""); TODO: ports
            return;
        }
    }
    // Если не найдено, добавить новую строку
    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(hostName));
    table->setItem(row, 1, new QTableWidgetItem(alive ? "🟢" : "🔴"));

    QComboBox *portCombo = new QComboBox();
    portCombo->addItems({"22", "80", "443", "8080"});
    portCombo->setCurrentText("80");
    table->setCellWidget(row, 2, portCombo);

    // Scan Ports button
    QPushButton *scanButton = new QPushButton("Scan Ports");
    table->setCellWidget(row, 3, scanButton);

    // Capture row and connect button to slot
    connect(scanButton, &QPushButton::clicked, this, [this, hostName, portCombo]() {
        int startPort = QInputDialog::getInt(this, "Start Port", "Enter start port:", 20, 1, 65535);
        int endPort = QInputDialog::getInt(this, "End Port", "Enter end port:", 100, startPort, 65535);

        // Create a copy of the Host from controller
        panic::Host* host = controller->getHost(hostName);  // You might need to implement getHost()

        panic::PortScanner::scanPorts(*host, startPort, endPort);

        portCombo->clear();
        for (auto it = host->ports_begin(); it != host->ports_end(); ++it) {
            portCombo->addItem(QString::number(it->first));
        }
    });
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
