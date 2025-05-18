#pragma once

#include "MonitorController.hpp"
#include "ChartWindow.hpp"

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMap>

class MainWindow : public QMainWindow {
    Q_OBJECT

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
    void updateResult(const QString &host, bool alive, int rtt);
    void onTableCellClicked(int row, int column);

    void onHostChecked(const QString &host, bool alive, int rtt);
};
