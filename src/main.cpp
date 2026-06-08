#include <QApplication>
#include <QCoreApplication>
#include <QFile>

#include "widgets/main_window.hpp"
#include "core/settings.hpp"

int main(int argc, char **argv) {

    // Initialize application
    QApplication app(argc, argv);

    // Application details
    app.setOrganizationName("Qt6-Paint");
    app.setApplicationName("Qt6-Paint");

    // Write default settings if no settings are available
    Settings settings;

    // Custom qss style
    QFile style(":/assets/style.qss");
    if (style.open(QFile::ReadOnly))
        app.setStyleSheet(QLatin1String(style.readAll()));
    else
        qWarning() << "Failed to open style.qss";

    // Initialize MainWindow
    MainWindow window(argc, argv);
    window.show();

    // Run event loop
    return app.exec();
}