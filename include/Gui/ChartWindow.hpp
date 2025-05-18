#pragma once

#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QString>
#include <QVBoxLayout>
#include <QList>
#include <QValueAxis>

class ChartWindow : public QWidget {
    Q_OBJECT
public:
    explicit ChartWindow(const QString &host, QWidget *parent = nullptr);
    ChartWindow(const QString &host, const QList<int> &history, QWidget *parent = nullptr);

    void appendRtt(int rtt);

private:
    QString hostName;
    QChart *chart = nullptr;
    QLineSeries *series = nullptr;
    int xIndex = 0;

    void setupChart();
    void setHistory(const QList<int> &history);
};
