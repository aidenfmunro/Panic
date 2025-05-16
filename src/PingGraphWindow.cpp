// #include "PingGraphWindow.h"
//
// #include <QtCharts/QtCharts>
// #include <QVBoxLayout>
//
// PingGraphWindow::PingGraphWindow(const QString& hostName, QWidget* parent)
//     : QWidget(parent), hostName_(hostName) {
//
//     series_ = new QLineSeries(this);
//
//     auto *chart = new QChart();
//     chart->addSeries(series_);
//     chart->setTitle(QString("Ping graph for %1").arg(hostName_));
//     chart->createDefaultAxes();
//
//     // Настроим оси
//     auto *axisX = new QValueAxis();
//     axisX->setRange(0, maxPoints_);
//     axisX->setLabelFormat("%d");
//     axisX->setTitleText("Ping count");
//     chart->setAxisX(axisX, series_);
//
//     auto *axisY = new QValueAxis();
//     axisY->setRange(0, 500); // 0-500 ms диапазон, можно менять динамически, но для простоты - фиксирован
//     axisY->setTitleText("Ping (ms)");
//     chart->setAxisY(axisY, series_);
//
//     chartView_ = new QChartView(chart, this);
//     chartView_->setRenderHint(QPainter::Antialiasing);
//
//     auto *layout = new QVBoxLayout(this);
//     layout->addWidget(chartView_);
//
//     setWindowTitle(QString("Ping graph - %1").arg(hostName_));
//     resize(600, 400);
// }
//
// void PingGraphWindow::addPingMeasurement(int ms) {
//     if (lastPings_.size() >= maxPoints_)
//         lastPings_.pop_front();
//     lastPings_.push_back(ms);
//
//     updateChart();
// }
//
// void PingGraphWindow::updateChart() {
//     series_->clear();
//     int i = 0;
//     for (auto ping : lastPings_) {
//         series_->append(i++, ping >= 0 ? ping : 0);
//     }
// }
//
