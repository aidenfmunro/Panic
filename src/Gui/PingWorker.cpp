#include "Gui/PingWorker.hpp"
#include <QRegularExpression>
#include <QProcess>

bool PingWorker::ping(const QString &host, int &rtt) {
    QProcess ping;
#ifdef _WIN32
    QString program = "ping";
    QStringList arguments = {"-n", "1", host};
#else
    QString program = "ping";
    QStringList arguments = {"-c", "1", "-W", "1", host};
#endif
    ping.start(program, arguments);
    bool finished = ping.waitForFinished(2000);
    if (!finished || ping.exitCode() != 0)
        return false;

    QString output = ping.readAllStandardOutput();
    QRegularExpression re("time=([0-9.]+) ?ms");
    auto match = re.match(output);
    if (match.hasMatch()) {
        rtt = match.captured(1).toDouble();
        return true;
    }
    return false;
}
