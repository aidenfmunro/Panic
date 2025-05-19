#include "Gui/ChartWindow.hpp"

#include <QtCharts/QtCharts>
#include <QLineSeries>
#include <QChartView>
#include <QVBoxLayout>
#include <QValueAxis>
#include <algorithm>

ChartWindow::ChartWindow(const QString &host, QWidget *parent)
    : QWidget(parent), hostName(host), xIndex(0)
{
    setupChart();
}

ChartWindow::ChartWindow(const QString &host, const QList<int> &history, QWidget *parent)
    : QWidget(parent), hostName(host), xIndex(0)
{
    setupChart();
    setHistory(history);
}

void ChartWindow::setupChart() {
    chart = new QChart();
    series = new QLineSeries();

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Ping RTT history: " + hostName);

    auto *xAxis = qobject_cast<QValueAxis *>(chart->axes(Qt::Horizontal).first());
    auto *yAxis = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).first());

    if (xAxis) xAxis->setTitleText("Time (s)");
    if (yAxis) yAxis->setTitleText("RTT (ms)");

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    clearButton = new QPushButton("Clear");

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    layout->addWidget(clearButton);
    setLayout(layout);

    connect(clearButton, &QPushButton::clicked, this, &ChartWindow::clearGraph);
}

void ChartWindow::setHistory(const QList<int> &history) {
    for (int rtt : history) {
        series->append(xIndex++, rtt);
    }
    if (!history.empty()) {
        chart->axes(Qt::Horizontal).first()->setRange(0, xIndex);
        auto maxRtt = *std::max_element(history.begin(), history.end());
        chart->axes(Qt::Vertical).first()->setRange(0, maxRtt);
    }
}

void ChartWindow::appendRtt(int rtt) {
    series->append(xIndex++, rtt);

    chart->axes(Qt::Horizontal).first()->setRange(0, xIndex);

    auto *yAxis = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());

    if (rtt > yAxis->max()) {
        yAxis->setMax(rtt);
    }
}

void ChartWindow::clearGraph() {
    series->clear();
    xIndex = 0;

    auto *xAxis = qobject_cast<QValueAxis *>(chart->axes(Qt::Horizontal).first());
    auto *yAxis = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).first());

    if (xAxis) {
        xAxis->setRange(0, 10);
    }
    if (yAxis) {
        yAxis->setRange(0, 100);
    }
}
