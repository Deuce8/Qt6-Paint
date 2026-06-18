#include "widgets/ribbon_lower.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

RibbonLower::RibbonLower(QWidget* parent) : QToolBar(parent) {
    
    // --------------------------------
    // |         Widget Setup         |
    // --------------------------------

    QSettings settings(this);

    setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    setOrientation(Qt::Orientation::Horizontal);
    setFixedHeight(32);
    layout()->setSpacing(4);
    addSeparator();

    // --------------------------------
    // |          Mouse Info          |
    // --------------------------------

    QLabel* mouse_icon = new QLabel(this);
    mouse_icon->setPixmap(QIcon(QStringLiteral(":/assets/icons/mouse.png")).pixmap(iconSize()));
    
    mouse_text = new QLabel(this);
    mouse_text->setText("-1, -1");
    mouse_text->setFixedWidth(128);

    addWidget(mouse_icon);
    addWidget(mouse_text);
    addSeparator();

    // --------------------------------
    // |        Selection Info        |
    // --------------------------------

    QLabel* selection_icon = new QLabel(this);
    selection_icon->setPixmap(QIcon(QStringLiteral(":/assets/icons/selection.png")).pixmap(iconSize()));
    
    selection_text = new QLabel(this);
    selection_text->setText("0x 0");
    selection_text->setFixedWidth(128);

    addWidget(selection_icon);
    addWidget(selection_text);
    addSeparator();

    // --------------------------------
    // |         Canvas Info          |
    // --------------------------------

    QLabel* canvas_icon = new QLabel(this);
    canvas_icon->setPixmap(QIcon(QStringLiteral(":/assets/icons/canvas.png")).pixmap(iconSize()));
    
    canvas_text = new QLabel(this);
    canvas_text->setText(settings.value("Canvas/width", -1).toString() + "x " + settings.value("Canvas/height", -1).toString());
    canvas_text->setFixedWidth(128);

    addWidget(canvas_icon);
    addWidget(canvas_text);
    addSeparator();

    // --------------------------------
    // |          Layer Info          |
    // --------------------------------

    QLabel* layer_icon = new QLabel(this);
    layer_icon->setPixmap(QIcon(QStringLiteral(":/assets/icons/layers.png")).pixmap(iconSize()));
    
    layer_text = new QLabel(this);
    layer_text->setText("Layer 0");
    layer_text->setFixedWidth(128);

    addWidget(layer_icon);
    addWidget(layer_text);
    addSeparator();

    // --------------------------------
    // |          Zoom Info           |
    // --------------------------------
    
    QWidget* stretch = new QWidget(this);
    stretch->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

    QLabel* zoom_icon = new QLabel(this);
    zoom_icon->setPixmap(QIcon(QStringLiteral(":/assets/icons/zoom.png")).pixmap(iconSize()));
    
    zoom_text = new QLabel(this);
    zoom_text->setText("100%");
    zoom_text->setFixedWidth(64);

    addWidget(stretch);
    addSeparator();
    addWidget(zoom_icon);
    addWidget(zoom_text);
    addSeparator();
}

// ------------------------------------------------
// |                    Slots                     |
// ------------------------------------------------

void RibbonLower::updateMousePosition(QPoint position) {
    mouse_text->setText(QString::number(position.x()) + ", " +  QString::number(position.y()));
}

void RibbonLower::updateSelectionSize(QSize size) {
    selection_text->setText(QString::number(size.width()) + "x " +  QString::number(size.height()));
}

void RibbonLower::updateCanvasSize(QSize size) {
    canvas_text->setText(QString::number(size.width()) + "x " +  QString::number(size.height()));
}

void RibbonLower::updateLayer(QString layer) {
    layer_text->setText(layer);
}

void RibbonLower::updateZoom(float zoom) {
    zoom_text->setText(QString::number(static_cast<int>(zoom * 100)) + "%");
}