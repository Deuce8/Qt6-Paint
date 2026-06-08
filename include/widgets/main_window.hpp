#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QObject>
#include <QResizeEvent>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>

#include "widgets/canvas.hpp"
#include "widgets/layer_menu.hpp"
#include "widgets/ribbon_lower.hpp"
#include "widgets/ribbon_upper.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(int argc, char **argv);

private:
    QGridLayout* central_layout;

protected:
    void resizeEvent(QResizeEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

public slots:
    void toggleLayerWidget(const bool& checked);

signals:
    void resize_event(QResizeEvent*);
    void drop_event(QDropEvent*);
};

#endif