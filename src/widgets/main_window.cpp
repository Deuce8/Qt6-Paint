#include "widgets/main_window.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

MainWindow::MainWindow(int argc, char **argv) : QMainWindow(nullptr) {
    
    // --------------------------------
    // |         Widget Setup         |
    // --------------------------------

    setAcceptDrops(true);
    setContentsMargins(4, 4, 4, 4);
    setWindowTitle("Qt6-Paint");
    setFocusPolicy(Qt::ClickFocus);
    setWindowFlags(Qt::Window);

    QWidget* central_widget = new QWidget(this);
    central_layout = new QGridLayout(central_widget);
    central_layout->setSpacing(8);
    central_layout->setContentsMargins(0, 0, 0, 0);
    central_layout->setColumnStretch(0, 1);
    central_layout->setColumnStretch(1, 0);

    // --------------------------------
    // |        Systems Setup         |
    // --------------------------------

    RibbonUpper* ribbon_upper = new RibbonUpper(central_widget);
    LayerMenu* layer_menu = new LayerMenu(central_widget);
    layer_menu->setVisible(false);
    Canvas* canvas_widget = new Canvas(central_widget, layer_menu);
    RibbonLower* ribbon_lower = new RibbonLower(central_widget);

    PaintManager* paint_manager = canvas_widget->getPaintManager();
    HistoryManager* history_manager = canvas_widget->getHistoryManager();
    Viewport* viewport = canvas_widget->getViewport();

    central_layout->addWidget(ribbon_upper, 0, 0, 1, 2);
    central_layout->addWidget(canvas_widget, 1, 0);
    central_layout->addWidget(layer_menu, 1, 1);
    central_layout->addWidget(ribbon_lower, 2, 0, 1, 2);

    setCentralWidget(central_widget);

    // --------------------------------
    // |         Wiring Slots         |
    // --------------------------------

    // ------------------------
    // |     Ribbon Upper     |
    // ------------------------

    // Tool & Style
    connect(ribbon_upper, &RibbonUpper::toolChanged, paint_manager, &PaintManager::setTool);
    connect(ribbon_upper, &RibbonUpper::capChanged, paint_manager, &PaintManager::setCapStyle);
    connect(ribbon_upper, &RibbonUpper::colorChanged, paint_manager, &PaintManager::updateColor);

    // Image
    connect(ribbon_upper->getRotateLeft(), &QAction::triggered, paint_manager, &PaintManager::rotateCounterClockwise);
    connect(ribbon_upper->getRotateRight(), &QAction::triggered, paint_manager, &PaintManager::rotateClockwise);
    connect(ribbon_upper->getMirrorHorizontal(), &QAction::triggered, paint_manager, &PaintManager::mirrorHorizontal);
    connect(ribbon_upper->getMirrorVertical(), &QAction::triggered, paint_manager, &PaintManager::mirrorVertical);
    connect(ribbon_upper->getClear(), &QAction::triggered, paint_manager, &PaintManager::clear);
    connect(ribbon_upper->getResetZoom(), &QAction::triggered, viewport, &Viewport::resetZoom);

    // History
    connect(ribbon_upper->getUndo(), &QAction::triggered, history_manager, &HistoryManager::undo);
    connect(ribbon_upper->getRedo(), &QAction::triggered, history_manager, &HistoryManager::redo);

    // IO
    connect(ribbon_upper->getLoad(), &QAction::triggered, paint_manager, &PaintManager::loadFile);
    connect(ribbon_upper->getSave(), &QAction::triggered, paint_manager, &PaintManager::saveFile);
    connect(ribbon_upper->getSaveAs(), &QAction::triggered, paint_manager, &PaintManager::saveFileAs);
    connect(ribbon_upper->getCopy(), &QAction::triggered, paint_manager, &PaintManager::copy);
    connect(ribbon_upper->getPaste(), &QAction::triggered, paint_manager, &PaintManager::paste);

    // Layers
    connect(ribbon_upper->getToggleLayers(), &QAction::triggered, layer_menu, &LayerMenu::toggleVisibility);
    connect(ribbon_upper->getToggleLayers(), &QAction::triggered, this, &MainWindow::toggleLayerWidget);

    // Selection
    connect(ribbon_upper->getTransparentSelection(), &QAction::triggered, paint_manager, &PaintManager::toggleTransparentSelection);
    connect(ribbon_upper->getDeleteSelection(), &QAction::triggered, paint_manager, &PaintManager::deleteSelection);
    connect(ribbon_upper->getPlaceSelection(), &QAction::triggered, paint_manager, &PaintManager::placeSelection);

    // Tool Sizes
    connect(ribbon_upper, &RibbonUpper::toolSizeChanged, paint_manager, &PaintManager::updateToolSize);
    connect(ribbon_upper, &RibbonUpper::toolDensityChanged, paint_manager, &PaintManager::updateAirbrushDensity);
    connect(ribbon_upper, &RibbonUpper::toolDiameterChanged, paint_manager, &PaintManager::updateAirbrushDiameter);

    // ------------------------
    // |     Layer Widget     |
    // ------------------------

    // Layer Updates
    connect(layer_menu, &LayerMenu::layer_name, ribbon_lower, &RibbonLower::updateLayer);
    connect(layer_menu, &LayerMenu::listTextChanged, ribbon_lower, &RibbonLower::updateLayer);
    connect(layer_menu, &LayerMenu::listRowChanged, paint_manager, &PaintManager::setCurrentLayer);
    connect(layer_menu, &LayerMenu::update_layer, paint_manager, &PaintManager::setCurrentLayer);

    // Layer Management
    connect(layer_menu, &LayerMenu::move_layer_up, paint_manager, &PaintManager::moveCurrentLayerUp);
    connect(layer_menu, &LayerMenu::move_layer_down, paint_manager, &PaintManager::moveCurrentLayerDown);
    connect(layer_menu, &LayerMenu::add_layer, paint_manager, &PaintManager::addLayer);
    connect(layer_menu, &LayerMenu::remove_layer, paint_manager, &PaintManager::removeCurrentLayer);

    // ------------------------
    // |    Paint Manager     |
    // ------------------------

    connect(paint_manager, &PaintManager::colorChanged, ribbon_upper, &RibbonUpper::updateColor);
    connect(paint_manager, &PaintManager::layer_size_changed, ribbon_lower, &RibbonLower::updateCanvasSize);
    connect(paint_manager, &PaintManager::selection_size_changed, ribbon_lower, &RibbonLower::updateSelectionSize);
    connect(paint_manager, &PaintManager::reset_layers, layer_menu, &LayerMenu::resetLayers);

    // ------------------------
    // |    Miscellaneous     |
    // ------------------------

    // MainWindow
    connect(this, &MainWindow::drop_event, paint_manager, &PaintManager::dropEvent);
    connect(this, &MainWindow::resize_event, viewport, &Viewport::clampOffset);

    // History Manager
    connect(history_manager, &HistoryManager::imageSizeChanged, ribbon_lower, &RibbonLower::updateCanvasSize);
    connect(history_manager, &HistoryManager::layerMenuChanged, layer_menu, &LayerMenu::updateLayers);

    // Viewport
    connect(viewport, &Viewport::zoom_changed, ribbon_lower, &RibbonLower::updateZoom);

    // Canvas
    connect(canvas_widget, &Canvas::mouse_position, ribbon_lower, &RibbonLower::updateMousePosition);

    // --------------------------------
    // |         ARG Parsing          |
    // --------------------------------

    if (argc > 1)
        paint_manager->loadImageFromPath(argv[1]);
}

// ------------------------------------------------
// |                  Protected                   |
// ------------------------------------------------

void MainWindow::resizeEvent(QResizeEvent* event) {
    emit resize_event(event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
    // Check if an image is being dragged onto the app
    if (event->mimeData()->hasImage()){
        event->acceptProposedAction();
        return;
    }

    // Ignore the event if there are no file paths.
    if (!event->mimeData()->hasUrls()){
        event->ignore();
        return;
    }
    
    // Loop through all file paths and search for images
    for(QUrl url : event->mimeData()->urls()){
        QString file = url.toLocalFile().toLower();
        for (const QString &end : {".png", ".jpg", ".jpeg", ".bmp"}){
            if (file.endsWith(end)){
                event->acceptProposedAction();
                return;
            }
        }
    }
    
    event->ignore();
}

void MainWindow::dropEvent(QDropEvent* event) {
    emit drop_event(event);
}

// ------------------------------------------------
// |                    Slots                     |
// ------------------------------------------------

void MainWindow::toggleLayerWidget(const bool& checked) {
    central_layout->setColumnStretch(0, checked ? 1 : 2);
    central_layout->setColumnStretch(1, checked ? 1 : 0);
}