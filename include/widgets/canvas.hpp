#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QSettings>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPoint>

#include "core/paint_manager.hpp"
#include "core/history_manager.hpp"
#include "core/viewport.hpp"
#include "widgets/layer_menu.hpp"

class Canvas : public QWidget {
    Q_OBJECT

public:
    Canvas(QWidget* parent, LayerMenu* layer_menu);

    Viewport* getViewport() const { return viewport; }
    PaintManager* getPaintManager() const { return paint_manager; }
    HistoryManager* getHistoryManager() const { return history_manager; }

private:
    Viewport* viewport;
    PaintManager* paint_manager;
    HistoryManager* history_manager;

protected:
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void resizeEvent(QResizeEvent* event);

signals:
    void paint_event(QPaintEvent*);
    void mouse_move_event(QMouseEvent*);
    void mouse_press_event(QMouseEvent*);
    void mouse_release_event(QMouseEvent*);
    void wheel_event(QWheelEvent*);
    void resize_event(QResizeEvent*);
    void mouse_position(QPoint);
};

#endif