#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      hostModel(new HostListModel(this)),
      monitor(new MonitorController(this)) {

    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    QHBoxLayout* inputLayout = new QHBoxLayout();

    listView = new QListView(this);
    listView->setModel(hostModel);

    inputHost = new QLineEdit(this);
    addButton = new QPushButton("Add Host", this);
    removeButton = new QPushButton("Remove Selected", this);

    inputLayout->addWidget(inputHost);
    inputLayout->addWidget(addButton);
    inputLayout->addWidget(removeButton);

    mainLayout->addWidget(listView);
    mainLayout->addLayout(inputLayout);
    setCentralWidget(central);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddHost);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveSelectedHost);
    connect(monitor, &MonitorController::pingSuccess, this, &MainWindow::onPingSuccess);
    connect(monitor, &MonitorController::pingFailure, this, &MainWindow::onPingFailure);
}

void MainWindow::onAddHost() {
    QString host = inputHost->text().trimmed();
    if (host.isEmpty()) return;

    if (hostModel->containsHost(host)) {
        QMessageBox::information(this, "Already added", "This host is already being monitored.");
        return;
    }

    HostIpInfo info(host.toStdString(), {});
    hostModel->addHost(info);
    monitor->startMonitoring(host);
}

void MainWindow::onRemoveSelectedHost() {
    QModelIndex idx = listView->currentIndex();
    if (!idx.isValid()) return;

    hostModel->removeHost(idx.row());
}

void MainWindow::onPingSuccess(QString host, QString ip, double rtt) {
    int row = findHostIndexByName(host);
    if (row < 0) return;

    IpInfo ipInfo;
    ipInfo.printable_addr = ip.toStdString();
    hostModel->updateHostStatus(row, true, static_cast<int>(rtt), ipInfo);
}

void MainWindow::onPingFailure(QString host, QString error) {
    int row = findHostIndexByName(host);
    if (row < 0) return;

    hostModel->updateHostStatus(row, false, -1, {});
}

int MainWindow::findHostIndexByName(const QString& name) const {
    for (int i = 0; i < hostModel->rowCount(); ++i) {
        if (hostModel->data(hostModel->index(i), HostListModel::HostNameRole).toString() == name) {
            return i;
        }
    }
    return -1;
}
