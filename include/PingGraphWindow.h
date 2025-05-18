// #pragma once
//
// #include <QWidget>
// #include <QtCharts/QtCharts>
// #include <deque>
//
// class PingGraphWindow : public QWidget {
//     Q_OBJECT
//
// public:
//     explicit PingGraphWindow(const QString& hostName, QWidget* parent = nullptr);
//     virtual ~PingGraphWindow() {};
//
//     void addPingMeasurement(int ms);
//
// private:
//     QString hostName_;
//     QChartView* chartView_;
//     QLineSeries* series_;
//
//     std::deque<int> lastPings_; // сохраняем последние 30 пингов
//     static constexpr size_t maxPoints_ = 30;
//
//     void updateChart();
// };
//
