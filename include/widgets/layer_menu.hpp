#ifndef LAYER_MENU_H
#define LAYER_MENU_H

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QInputDialog>
#include <QPushButton>

class LayerMenu : public QWidget {
    Q_OBJECT

public:
    LayerMenu(QWidget* parent);

    QStringList getLayers();
    int getIndex();

private:
    QGridLayout* central_layout;
    QListWidget* layer_list;

public slots:
    void moveLayerUp();
    void moveLayerDown();
    void renameLayer();
    void addLayer();
    void removeLayer();
    void resetLayers();
    void updateLayers(QStringList list, int index);
    void toggleVisibility();

signals:
    void move_layer_up();
    void move_layer_down();
    void add_layer();
    void remove_layer();
    void update_layer(int);
    void layer_name(QString);
    void listTextChanged(const QString&);
    void listRowChanged(int);
};

#endif