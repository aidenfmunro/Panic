#include <QApplication>
#include "Gui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("PANIC — Network Monitor");
    window.resize(800, 600);
    window.show();

    return app.exec();
}
