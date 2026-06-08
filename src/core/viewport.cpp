#include "core/viewport.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

Viewport::Viewport(QObject* parent, QWidget* canvas) : QObject(parent) {
    QSettings settings(this);

    zoom_factor = settings.value("Zoom/factor", 1.5f).toFloat();
    zoom_minimum = settings.value("Zoom/minimum", 0.25f).toFloat();
    zoom_maximum = settings.value("Zoom/maximum", 0.125f).toFloat();

    zoom = 0.875f;
    offset = QPointF(0, 0);
    panning = false;
    last_pan_point = QPointF(0, 0);
    image_size = QSize(settings.value("Canvas/width", -1).toInt(), settings.value("Canvas/height", -1).toInt());

    this->canvas = canvas;
}

// ------------------------------------------------
// |                    Slots                     |
// ------------------------------------------------

void Viewport::updateImageSize(QSize size) {
    image_size = size;
}

void Viewport::clampOffset(){
    int canvas_width = canvas->width();
    int canvas_height = canvas->height();
    float zoomed_scale = zoom * scale();
    float view_width = image_size.width() * zoomed_scale;
    float view_height = image_size.height() * zoomed_scale;
    float scaled_margins = 16 * zoomed_scale;

    offset = QPointF(
        view_width  < canvas_width ? (canvas_width  - view_width ) / 2 : std::max(std::min(offset.x(), (qreal)scaled_margins), (qreal)(canvas_width  - view_width  - scaled_margins)),
        view_height < canvas_height ? (canvas_height - view_height) / 2 : std::max(std::min(offset.y(), (qreal)scaled_margins), (qreal)(canvas_height - view_height - scaled_margins))
    );
}

void Viewport::resetZoom(){
    zoom = 0.875f;
    offset = QPointF(0, 0);
    clampOffset();

    emit offset_changed(offset);
    emit zoom_changed(zoom);
    canvas->update();
}

void Viewport::wheel(QWheelEvent* event){
    float old_zoom = zoom;

    zoom *= event->angleDelta().y() > 0 ? zoom_factor : 1 / zoom_factor;
    zoom = std::clamp(zoom, zoom_maximum, imageSizePrimary() * zoom_maximum);

    offset = (offset - event->position()) * (zoom / old_zoom) + event->position();

    clampOffset();

    emit offset_changed(offset);
    emit zoom_changed(zoom);
    canvas->update();
}

void Viewport::mouseMove(QMouseEvent* event) {
    if (!panning)
        return;

    offset += event->pos() - last_pan_point;
    last_pan_point = event->position();
    clampOffset();

    emit offset_changed(offset);
    canvas->update();
}

void Viewport::mousePress(QMouseEvent* event) {
    if (event->button() != Qt::MouseButton::MiddleButton)
        return;

    panning = true;
    last_pan_point = event->position();
}

void Viewport::mouseRelease(QMouseEvent* event) {
    if (event->button() != Qt::MouseButton::MiddleButton)
        return;
    
    panning = false;
}

// ------------------------------------------------
// |                  Functions                   |
// ------------------------------------------------

float Viewport::scale() {
    return std::min(
        static_cast<float>(canvas->width()) / static_cast<float>(image_size.width()), 
        static_cast<float>(canvas->height()) / static_cast<float>(image_size.height())
    );
}

float Viewport::inverseScale() {
    return std::max(
        static_cast<float>(image_size.width()) / static_cast<float>(canvas->width()), 
        static_cast<float>(image_size.height()) / static_cast<float>(canvas->height())
    );
}

int Viewport::imageSizePrimary() {
    return std::max(image_size.width(), image_size.height());
}

int Viewport::canvasSizePrimary() {
    return std::max(canvas->width(), canvas->height());
}

QPoint Viewport::canvasSpace(QPoint point) {
    return QPoint(
        (int)((point.x() - offset.x()) * inverseScale() / zoom),
        (int)((point.y() - offset.y()) * inverseScale() / zoom)
    );
}