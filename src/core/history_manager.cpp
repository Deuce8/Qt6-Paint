#include "core/history_manager.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

HistoryManager::HistoryManager(QObject* parent, QWidget* canvas, PaintManager* paint_manager, LayerMenu* layer_menu) : QObject(parent) {
    cache_index = -1;
   
    this->canvas = canvas;
    this->paint_manager = paint_manager;
    this->layer_menu = layer_menu;

    QSettings settings = QSettings();

    cache_max = settings.value("History/max_restores", 32).toInt();
}

// ------------------------------------------------
// |                    Slots                     |
// ------------------------------------------------

void HistoryManager::undo() {
    if (cache_index != 0)
        cache_index -= 1;

    emit indexChanged(cache_layer_index[cache_index]);
    emit layersChanged(cache_layers[cache_index]);
    emit layerMenuChanged(cache_layer_items[cache_index], cache_layer_row[cache_index]);
    emit imageSizeChanged(cache_image_size[cache_index]);

    emit stateChanged();

    canvas->update();
}

void HistoryManager::redo() {
    if (cache_index >= cache_layer_index.count() - 1)
        return;

    cache_index += 1;

    emit indexChanged(cache_layer_index[cache_index]);
    emit layersChanged(cache_layers[cache_index]);
    emit layerMenuChanged(cache_layer_items[cache_index], cache_layer_row[cache_index]);
    emit imageSizeChanged(cache_image_size[cache_index]);

    emit stateChanged();

    canvas->update();
}

void HistoryManager::createSnapshot() {
    if (cache_index < cache_layer_index.count() - 1) {
        cache_layers.erase(cache_layers.begin() + cache_index + 1, cache_layers.end());
        cache_layer_row.erase(cache_layer_row.begin() + cache_index + 1, cache_layer_row.end());
        cache_layer_items.erase(cache_layer_items.begin() + cache_index + 1, cache_layer_items.end());
        cache_image_size.erase(cache_image_size.begin() + cache_index + 1, cache_image_size.end());
        cache_layer_index.erase(cache_layer_index.begin() + cache_index + 1, cache_layer_index.end());
    }

    cache_layer_index.append(paint_manager->getIndex());
    cache_layers.append(paint_manager->getLayers());
    cache_layer_row.append(layer_menu->getIndex());
    cache_layer_items.append(layer_menu->getLayers());
    cache_image_size.append(paint_manager->getImageSize());

    if (cache_max < cache_layer_index.count() - 1) {
        cache_layer_index.pop_front();
        cache_layers.pop_front();
        cache_layer_row.pop_front();
        cache_layer_items.pop_front();
        cache_image_size.pop_front();
    }
    else {
        cache_index++;
    }
}