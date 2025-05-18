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
    void onRemoveSelectedHost();
    void onPingSuccess(QString host, QString ip, double rtt);
    void onPingFailure(QString host, QString error);

private:
    HostListModel* hostModel;
    MonitorController* monitor;

    QListView* listView;
    QLineEdit* inputHost;
    QPushButton* addButton;
    QPushButton* removeButton;

    int findHostIndexByName(const QString& name) const;
};

