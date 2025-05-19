#pragma once

#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QString>
#include <QVBoxLayout>
#include <QList>
#include <QValueAxis>
#include <QPushButton>

class ChartWindow : public QWidget {
    Q_OBJECT
public:
    explicit ChartWindow(const QString &host, QWidget *parent = nullptr);
    ChartWindow(const QString &host, const QList<int> &history, QWidget *parent = nullptr);

    void appendRtt(int rtt);

private slots:
    void clearGraph();

private:
    QString hostName;
    QChart *chart = nullptr;
    QLineSeries *series = nullptr;
    int xIndex = 0;

    QPushButton *clearButton = nullptr;

    void setupChart();
    void setHistory(const QList<int> &history);
};
