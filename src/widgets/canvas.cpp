#include "widgets/canvas.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

Canvas::Canvas(QWidget* parent, LayerMenu* layer_menu) : QWidget(parent) {
    
    setMouseTracking(true);

    // --------------------------------
    // |        Systems Setup         |
    // --------------------------------

    viewport = new Viewport(this, this);
    paint_manager = new PaintManager(this, this);
    history_manager = new HistoryManager(this, this, paint_manager, layer_menu);

    history_manager->createSnapshot();

    // --------------------------------
    // |         Wiring Slots         |
    // --------------------------------

    // Canvas
    connect(this, &Canvas::paint_event, paint_manager, &PaintManager::paint);
    connect(this, &Canvas::mouse_move_event, viewport, &Viewport::mouseMove);    
    connect(this, &Canvas::mouse_move_event, paint_manager, &PaintManager::mouseMove);
    connect(this, &Canvas::mouse_press_event, viewport, &Viewport::mousePress);    
    connect(this, &Canvas::mouse_press_event, paint_manager, &PaintManager::mousePress);
    connect(this, &Canvas::mouse_release_event, viewport, &Viewport::mouseRelease);    
    connect(this, &Canvas::mouse_release_event, paint_manager, &PaintManager::mouseRelease);
    connect(this, &Canvas::wheel_event, viewport, &Viewport::wheel);    
    connect(this, &Canvas::resize_event, viewport, &Viewport::resetZoom);    

    // Viewport
    connect(viewport, &Viewport::zoom_changed, paint_manager, &PaintManager::updateZoom);    
    connect(viewport, &Viewport::offset_changed, paint_manager, &PaintManager::updateOffset);

    // Paint Manager
    connect(paint_manager, &PaintManager::create_restore_point, history_manager, &HistoryManager::createSnapshot);    
    connect(paint_manager, &PaintManager::layer_size_changed, viewport, &Viewport::updateImageSize);

    // History Manager
    connect(history_manager, &HistoryManager::imageSizeChanged, viewport, &Viewport::updateImageSize);    
    connect(history_manager, &HistoryManager::stateChanged, viewport, &Viewport::clampOffset);
    connect(history_manager, &HistoryManager::imageSizeChanged, paint_manager, &PaintManager::updateImageSize);    
    connect(history_manager, &HistoryManager::layersChanged, paint_manager, &PaintManager::updateLayers);
    connect(history_manager, &HistoryManager::indexChanged, paint_manager, &PaintManager::updateLayerIndex);    
}

// ------------------------------------------------
// |                  Protected                   |
// ------------------------------------------------

void Canvas::paintEvent(QPaintEvent* event) {
    emit paint_event(event);
}

void Canvas::mouseMoveEvent(QMouseEvent* event) {
    emit mouse_move_event(event);
    emit mouse_position(viewport->canvasSpace(event->position().toPoint()));
}

void Canvas::mousePressEvent(QMouseEvent* event) {
    emit mouse_press_event(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent* event) {
    emit mouse_release_event(event);
}

void Canvas::wheelEvent(QWheelEvent* event) {
    emit wheel_event(event);
}

void Canvas::resizeEvent(QResizeEvent* event) {
    emit resize_event(event);
}