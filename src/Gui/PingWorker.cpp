#include "Gui/PingWorker.hpp"
#include <QRegularExpression>
#include <QProcess>

bool PingWorker::ping(const QString &host, int &rtt) {
#ifdef _WIN32
    QString program = "ping";
    QStringList arguments = {"-n", "1", host};
#else
    QString program = "ping";
    QStringList arguments = {"-c", "1", "-W", "1", host};
#endif
    pingProcess_.start(program, arguments);
    bool finished = pingProcess_.waitForFinished(2000);
    if (!finished || pingProcess_.exitCode() != 0)
        return false;

    QString output = pingProcess_.readAllStandardOutput();
    QRegularExpression re("time=([0-9.]+) ?ms");
    auto match = re.match(output);
    if (match.hasMatch()) {
        rtt = match.captured(1).toDouble();
        return true;
    }
    return false;
}

void PingWorker::stop() {
    if (pingProcess_.state() == QProcess::Running)
        pingProcess_.kill();
}
