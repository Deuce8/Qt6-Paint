#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWidget>
#include <QSettings>
#include <QWheelEvent>
#include <QMouseEvent>

class Viewport : public QObject {
    Q_OBJECT

public:
    Viewport(QObject* parent, QWidget* canvas);

    QPoint canvasSpace(QPoint point);

private:
    float zoom_factor;
    float zoom_minimum;
    float zoom_maximum;
    
    float zoom;
    QPointF offset;
    bool panning;
    QPointF last_pan_point;
    QSize image_size;

    QWidget* canvas;

    float scale();
    float inverseScale();
    int imageSizePrimary();
    int canvasSizePrimary();

public slots:
    void updateImageSize(QSize size);
    void clampOffset();
    void resetZoom();

    void wheel(QWheelEvent* event);
    void mouseMove(QMouseEvent* event);
    void mousePress(QMouseEvent* event);
    void mouseRelease(QMouseEvent* event);

signals:
    void offset_changed(QPointF);
    void zoom_changed(float);
};

#endif