#ifndef HISTORY_MANAGER_H
#define HISTORY_MANAGER_H

#include <QWidget>
#include <QSettings>
#include <QWheelEvent>
#include <QMouseEvent>

#include "core/paint_manager.hpp"
#include "widgets/layer_menu.hpp"

class HistoryManager : public QObject {
    Q_OBJECT

public:
    HistoryManager(QObject* parent, QWidget* canvas, PaintManager* paint_manager, LayerMenu* layer_menu);

private:
    QWidget* canvas;
    PaintManager* paint_manager;
    LayerMenu* layer_menu;

    int cache_index;
    int cache_max;

    QList<int> cache_layer_index;
    QList<QList<QImage>> cache_layers;
    QList<int> cache_layer_row;
    QList<QStringList> cache_layer_items;
    QList<QSize> cache_image_size;

public slots:
    void undo();
    void redo();
    void createSnapshot();

signals:
    void indexChanged(int);
    void layersChanged(QList<QImage>);
    void layerMenuChanged(QStringList, int);
    void imageSizeChanged(QSize);
    void stateChanged();
};

#endif