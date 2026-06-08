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

    QVector<int> cache_layer_index;
    QVector<QVector<QImage>> cache_layers;
    QVector<int> cache_layer_row;
    QVector<QStringList> cache_layer_items;
    QVector<QSize> cache_image_size;

public slots:
    void undo();
    void redo();
    void createSnapshot();

signals:
    void indexChanged(int);
    void layersChanged(QVector<QImage>);
    void layerMenuChanged(QStringList, int);
    void imageSizeChanged(QSize);
    void stateChanged();
};

#endif