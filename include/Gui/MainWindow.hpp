#pragma once

#include "MonitorController.hpp"
#include "ChartWindow.hpp"

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMap>

namespace panic {

enum Status {
    Alive,
    Dead
};

inline QString toEmoji(Status status) {
    switch (status) {
        case Status::Alive: return "🟢";
        case Status::Dead:  return "🔴";
    }
    return {};
}
}  // namespace panic

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    enum Position {
        Host,
        IP,
        Status,
        RTT,
        Port,
        Actions,
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTableWidget *table;
    QPushButton *addHostButton;
    QPushButton *removeHostButton;
    MonitorController *controller;

    // Хранит открытые окна графиков по хостам
    QMap<QString, ChartWindow*> openCharts;

private slots:
    void onAddHost();
    void onRemoveHost();
    void updateResult(const QString &hostIP, bool alive, int rtt);
    void onTableCellClicked(int row, int column);

    void onHostChecked(const QString &host, bool alive, int rtt);
};
