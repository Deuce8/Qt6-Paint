#include "core/settings.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

Settings::Settings(QObject* parent) : QObject(parent) {
    QSettings settings(this);

    if (settings.contains("Program/initialized"))
        return;

    settings.setValue("Program/initialized", true);

    settings.setValue("Colors/secondary_color", "#ffffff");
    settings.setValue("Colors/primary_color", "#000000");

    settings.setValue("Canvas/width", 1920);
    settings.setValue("Canvas/height", 1080);

    settings.setValue("History/max_restores", 32);

    settings.setValue("Selection/highlight_color", "#c40f0f");

    settings.setValue("Zoom/factor", 1.5);
    settings.setValue("Zoom/minimum", 0.25);
    settings.setValue("Zoom/maximum", 0.125);

    settings.setValue("Tools/size", 1);
    settings.setValue("Tools/airbrush_diameter", 8);
    settings.setValue("Tools/airbrush_density", 16);

    settings.sync();
}