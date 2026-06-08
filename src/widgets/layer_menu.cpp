#include "widgets/layer_menu.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

LayerMenu::LayerMenu(QWidget* parent) : QWidget(parent) {

    // --------------------------------
    // |         Widget Setup         |
    // --------------------------------

    setFixedWidth(256);

    central_layout = new QGridLayout(this);
    central_layout->setContentsMargins(4, 4, 4, 4);
    central_layout->setSpacing(8);

    // --------------------------------
    // |        Content Setup         |
    // --------------------------------

    // Layer List
    layer_list = new QListWidget(this);
    layer_list->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    layer_list->addItem("Layer 0");
    layer_list->setCurrentRow(0);
    connect(layer_list, &QListWidget::currentTextChanged, this, &LayerMenu::listTextChanged);
    connect(layer_list, &QListWidget::currentRowChanged, this, &LayerMenu::listRowChanged);
    central_layout->addWidget(layer_list, 0, 0, 1, 5);

    // Layer Up Button
    QPushButton* layer_up = new QPushButton(this);
    layer_up->setIcon(QIcon(QStringLiteral(":/assets/icons/up_arrow.png")));
    layer_up->setToolTip("Move Layer Up");
    connect(layer_up, &QPushButton::clicked, this, &LayerMenu::moveLayerUp);
    central_layout->addWidget(layer_up, 1, 0);

    // Layer Down Button
    QPushButton* layer_down = new QPushButton(this);
    layer_down->setIcon(QIcon(QStringLiteral(":/assets/icons/down_arrow.png")));
    layer_down->setToolTip("Move Layer Down");
    connect(layer_down, &QPushButton::clicked, this, &LayerMenu::moveLayerDown);
    central_layout->addWidget(layer_down, 1, 1);

    // Layer Rename Button
    QPushButton* layer_rename = new QPushButton(this);
    layer_rename->setIcon(QIcon(QStringLiteral(":/assets/icons/rename.png")));
    layer_rename->setToolTip("Rename Layer");
    connect(layer_rename, &QPushButton::clicked, this, &LayerMenu::renameLayer);
    central_layout->addWidget(layer_rename, 1, 2);

    // Layer Add Button
    QPushButton* layer_add = new QPushButton(this);
    layer_add->setIcon(QIcon(QStringLiteral(":/assets/icons/add_layer.png")));
    layer_add->setToolTip("Add Layer");
    connect(layer_add, &QPushButton::clicked, this, &LayerMenu::addLayer);
    central_layout->addWidget(layer_add, 1, 3);

    // Layer Remove Button
    QPushButton* layer_remove = new QPushButton(this);
    layer_remove->setIcon(QIcon(QStringLiteral(":/assets/icons/remove_layer.png")));
    layer_remove->setToolTip("Remove Layer");
    connect(layer_remove, &QPushButton::clicked, this, &LayerMenu::removeLayer);
    central_layout->addWidget(layer_remove, 1, 4);

    setLayout(central_layout);
}

// ------------------------------------------------
// |                    Slots                     |
// ------------------------------------------------

void LayerMenu::moveLayerUp() {
    int current_row = layer_list->currentRow();
    if (current_row <= 0)
        return;

    emit move_layer_up();
    layer_list->insertItem(current_row - 1, layer_list->takeItem(current_row));
    layer_list->setCurrentRow(current_row - 1);
}

void LayerMenu::moveLayerDown() {
    int current_row = layer_list->currentRow();
    if (current_row >= layer_list->count() - 1)
        return;

    emit move_layer_down();
    layer_list->insertItem(current_row + 1, layer_list->takeItem(current_row));
    layer_list->setCurrentRow(current_row + 1);
}

void LayerMenu::renameLayer() {
    QListWidgetItem* current_item = layer_list->item(layer_list->currentRow());

    bool ok = false;
    QString name = QInputDialog::getText(
        this, 
        tr("Rename Layer"), 
        tr("New layer name:"),
        QLineEdit::Normal,
        current_item->text(),
        &ok
    );

    if (ok && !name.isEmpty())
        current_item->setText(name);
}

void LayerMenu::addLayer() {
    layer_list->addItem("Layer " + QString::number(layer_list->count()));
    emit add_layer();
}

void LayerMenu::removeLayer() {
    if (layer_list->count() <= 1)
        return;

    layer_list->blockSignals(true);
    layer_list->takeItem(layer_list->currentRow());
    layer_list->blockSignals(false);

    emit remove_layer();
    emit update_layer(layer_list->currentRow());
    emit layer_name(layer_list->currentItem()->text());
}

void LayerMenu::resetLayers() {
    layer_list->clear();
    layer_list->addItem(QString("Layer 0"));
    layer_list->setCurrentRow(0);
}

void LayerMenu::updateLayers(QStringList items, int index) {
    layer_list->clear();
    if (items.count() == 1)
        layer_list->addItem(items[0]);
    else
        layer_list->addItems(items);
    layer_list->setCurrentRow(index);
}

void LayerMenu::toggleVisibility() {
    setVisible(!isVisible());
}

// ------------------------------------------------
// |                  Functions                   |
// ------------------------------------------------

QStringList LayerMenu::getLayers() {
    QStringList list;

    list.reserve(layer_list->count());
    for (int i = 0; i < layer_list->count(); i++)
        list << layer_list->item(i)->text();

    return list;
}

int LayerMenu::getIndex() {
    return layer_list->currentRow();
}