#ifndef PAINT_MANAGER_H
#define PAINT_MANAGER_H

#include <QWidget>
#include <QSettings>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QRectF>
#include <QImage>
#include <QWidget>
#include <QMimeData>
#include <QGuiApplication>
#include <QClipboard>
#include <QFileDialog>

#include "dialogs/new_file.hpp"

#include <random>
#include <optional>

class PaintManager : public QObject {
    Q_OBJECT

public:
    PaintManager(QObject* parent, QWidget* canvas);

    // --------------------------------
    // |       Layer Functions        |
    // --------------------------------

    QVector<QImage> getLayers();
    int getIndex();
    QSize getImageSize();

private:
    // --------------------------------
    // |          Variables           |
    // --------------------------------

    QString file_path;
    QWidget* canvas;

    // Selection
    std::optional<QRectF> selection_rect;
    std::optional<QImage> selection_image;
    bool dragging_selection;
    std::optional<QPointF> drag_position;
    bool cleared_selection;
    bool transparent_selection;

    // Colors
    QColor primary_color;
    QColor secondary_color;
    QColor color;

    // Coordinates
    QPoint last_mouse_point;
    std::optional<QPoint> last_mouse_point_active;
    std::optional<QPoint> last_mouse_click;
    std::optional<QPoint> last_mouse_release;
    bool left_mouse_pressed;
    bool right_mouse_pressed;

    // Tool Data
    int tool_size;
    int airbrush_diameter;
    int airbrush_density;
    int tool_id;
    int cap_style;

    // Viewport
    QPointF offset;
    float zoom;

    // Layers
    int layer_index;
    QVector<QImage> layers;
    QSize image_size;

    // --------------------------------
    // |     Selection Functions      |
    // --------------------------------

    void pressSelection(QMouseEvent* event);    
    void releaseSelection(QMouseEvent* event);
    void moveSelection(QMouseEvent* event);
    void filterSelection();
    void clearUnderSelection();
    void ensureSelectionFit();

    // --------------------------------
    // |        Crop Functions        |
    // --------------------------------

    void crop(QMouseEvent* event);
    void moveCrop(QMouseEvent* event);
    
    // --------------------------------
    // |       Helper Functions       |
    // --------------------------------

    float scale();
    bool isValidPoint(QPoint point);
    QPoint canvasSpace(QPointF point);
    QPoint canvasSpace(QPoint point);
    QPointF clampPoint(QPointF point);
    QPointF clampPoint(QPoint point);
    QRectF clampRectF(QRectF rect);
    void rotate(int measure);
    QPainter* getPainter();

    // --------------------------------
    // |       Paint Functions        |
    // --------------------------------

    void drawGrid(QPainter* painter);
    void drawBorder(QPainter* painter);
    void drawPreview(QPainter* painter);
    void drawLayerPreviewUnder(QPainter* painter);
    void drawLayerPreviewOver(QPainter* painter);

    // --------------------------------
    // |        Tool Functions        |
    // --------------------------------

    void pencil(QMouseEvent* event);
    void floodFill(QMouseEvent* event);
    void matchingFill(QMouseEvent* event);
    void eraser(QMouseEvent* event);
    void eyedropper(QMouseEvent* event);
    void airbrush(QMouseEvent* event);
    void line(QMouseEvent* event);
    void ellipse(QMouseEvent* event);
    void rectangle(QMouseEvent* event);

public slots:
    // --------------------------------
    // |       Selection Slots        |
    // --------------------------------

    void moveSelectionUp();
    void moveSelectionDown();
    void moveSelectionLeft();
    void moveSelectionRight();

    // --------------------------------
    // |       File Management        |
    // --------------------------------

    void dropEvent(QDropEvent* event);
    void loadImageFromPath(QString file_path);
    void loadImage(QImage image);
    void loadFile();
    void saveFileAs();
    void saveFile();
    void copy();
    void paste();
    void newFile();

    // --------------------------------
    // |       Tool Management        |
    // --------------------------------

    void setTool(int tool_id);
    void setCapStyle(int cap_id);
    void toggleTransparentSelection();
    void updateToolSize(QString size);
    void updateAirbrushDiameter(QString size);
    void updateAirbrushDensity(QString size);

    // --------------------------------
    // |     Variable Management      |
    // --------------------------------

    void updateImageSize(QSize size);
    void updateLayerIndex(int index);
    void updateLayers(QVector<QImage> layers);
    void updateZoom(float zoom);
    void updateOffset(QPointF offset);
    void updateColor(QColor color, bool is_primary);

    // --------------------------------
    // |     Layer Widget Backend     |
    // --------------------------------

    void setCurrentLayer(int index);
    void addLayer();
    void removeCurrentLayer();
    void moveCurrentLayerUp();
    void moveCurrentLayerDown();

    // --------------------------------
    // |        Canvas Backend        |
    // --------------------------------

    void paint();
    void mousePress(QMouseEvent* event);
    void mouseRelease(QMouseEvent* event);
    void mouseMove(QMouseEvent* event);

    // --------------------------------
    // |         Tool Actions         |
    // --------------------------------

    void clear();
    void mirrorHorizontal();
    void mirrorVertical();
    void rotateClockwise();
    void rotateCounterClockwise();
    void deleteSelection();
    void placeSelection();

signals:
    void create_restore_point();
    void layer_size_changed(QSize);
    void selection_size_changed(QSize);
    void colorChanged(QColor, bool);
    void reset_layers();
};

#endif