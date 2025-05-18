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
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onAddHost();
    void onRemoveHost();
    void updateResult(const QString &hostName, bool alive, int rtt);
    void onTableCellClicked(int row, int column);

private:
    HostListModel* hostModel;
    MonitorController* monitor;

    QListView* listView;
    QLineEdit* inputHost;
    QPushButton* addButton;
    QPushButton* removeButton;

    int findHostIndexByName(const QString& name) const;
};

