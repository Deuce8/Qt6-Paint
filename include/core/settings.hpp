#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QObject>

class Settings : public QObject {

public:
    Settings(QObject* parent = nullptr);
};

#endif