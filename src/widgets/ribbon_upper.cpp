#include "widgets/ribbon_upper.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

RibbonUpper::RibbonUpper(QWidget* parent) : QToolBar(parent) {

    // --------------------------------
    // |         Widget Setup         |
    // --------------------------------

    QSettings settings(this);

    setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    setOrientation(Qt::Orientation::Horizontal);
    setFixedHeight(96);
    
    primary_color = QColor::fromString(settings.value("Colors/primary_color", "000000").toString());
    secondary_color = QColor::fromString(settings.value("Colors/secondary_color", "ffffff").toString());
    
    QButtonGroup* tool_group = new QButtonGroup(this);
    tool_group->setExclusive(true);
    connect(tool_group, &QButtonGroup::idClicked, this, &RibbonUpper::toolChanged);

    QButtonGroup* cap_group = new QButtonGroup(this);
    cap_group->setExclusive(true);
    connect(cap_group, &QButtonGroup::idClicked, this, &RibbonUpper::capChanged);

    addSeparator();

    // --------------------------------
    // |            Image             |
    // --------------------------------

    // Image Widget & Layout
    QWidget* image_widget = new QWidget(this);
    QGridLayout* image_layout = new QGridLayout(image_widget);

    // Image Label
    image_layout->addWidget(createLabel(image_widget, "Image"), 2, 0, 1, 4);

    // Rotate Image Left
    QToolButton* image_rotate_left = createButton(image_widget, "Rotate 90 degrees to the left", ":/assets/icons/rotate_left.png", "Ctrl+Q");
    rotate_left = image_rotate_left->defaultAction();
    image_layout->addWidget(image_rotate_left, 0, 0, 1, 1);

    // Rotate Image Right
    QToolButton* image_rotate_right = createButton(image_widget, "Rotate 90 degrees to the right", ":/assets/icons/rotate_right.png", "Ctrl+E");
    rotate_right = image_rotate_right->defaultAction();
    image_layout->addWidget(image_rotate_right, 0, 1, 1, 1);

    // Crop
    image_layout->addWidget(createToolButton(image_widget, "Crop", ":/assets/icons/crop.png", 10, tool_group), 0, 2, 1, 1);

    // Mirror Horizontal
    QToolButton* image_mirror_horizontal = createButton(image_widget, "Mirror Horizontal", ":/assets/icons/mirror_horizontal.png");
    mirror_horizontal = image_mirror_horizontal->defaultAction();
    image_layout->addWidget(image_mirror_horizontal, 1, 0, 1, 1);

    // Mirror Vertical
    QToolButton* image_mirror_vertical = createButton(image_widget, "Mirror Vertical", ":/assets/icons/mirror_vertical.png");
    mirror_vertical = image_mirror_vertical->defaultAction();
    image_layout->addWidget(image_mirror_vertical, 1, 1, 1, 1);

    // Clear Image
    QToolButton* image_clear = createButton(image_widget, "Clear Canvas", ":/assets/icons/clear.png");
    clear = image_clear->defaultAction();
    image_layout->addWidget(image_clear, 1, 2, 1, 1);

    // Reset Zoom
    QToolButton* image_reset_zoom = createButton(image_widget, "Reset Zoom", ":/assets/icons/zoom.png", "Space");
    reset_zoom = image_reset_zoom->defaultAction();
    image_layout->addWidget(image_reset_zoom, 0, 3, 1, 1);

    // Final Setup
    image_widget->setLayout(image_layout);
    addWidget(image_widget);
    addSeparator();

    // --------------------------------
    // |            Shapes            |
    // --------------------------------

    // Shapes Widget & Layout
    QWidget* shape_widget = new QWidget(this);
    QGridLayout* shape_layout = new QGridLayout(shape_widget);

    // Label
    shape_layout->addWidget(createLabel(shape_widget, "Shapes"), 2, 0, 1, 2);

    // Shapes
    shape_layout->addWidget(createToolButton(shape_widget, "Line", ":/assets/icons/line.png", 6, tool_group), 0, 0, 1, 1);
    shape_layout->addWidget(createToolButton(shape_widget, "Ellipse", ":/assets/icons/ellipse.png", 7, tool_group), 0, 1, 1, 1);
    shape_layout->addWidget(createToolButton(shape_widget, "Rectangle", ":/assets/icons/rectangle.png", 8, tool_group), 1, 0, 1, 1);

    // Final Setup
    shape_widget->setLayout(shape_layout);
    addWidget(shape_widget);
    addSeparator();

    // --------------------------------
    // |            Tools             |
    // --------------------------------

    // Tools Widget & Layout
    QWidget* tool_widget = new QWidget(this);
    QGridLayout* tool_layout = new QGridLayout(tool_widget);

    // Label
    tool_layout->addWidget(createLabel(tool_widget, "Tools"), 2, 0, 1, 3);

    // Tools
    tool_layout->addWidget(createToolButton(tool_widget, "Pencil", ":/assets/icons/pencil.png", 0, tool_group), 0, 0, 1, 1);
    tool_layout->addWidget(createToolButton(tool_widget, "Fill", ":/assets/icons/fill.png", 1, tool_group), 0, 1, 1, 1);
    tool_layout->addWidget(createToolButton(tool_widget, "Fill All", ":/assets/icons/fill_all.png", 2, tool_group), 0, 2, 1, 1);
    tool_layout->addWidget(createToolButton(tool_widget, "Eraser", ":/assets/icons/eraser.png", 3, tool_group), 1, 0, 1, 1);
    tool_layout->addWidget(createToolButton(tool_widget, "Eyedropper", ":/assets/icons/eyedropper.png", 4, tool_group), 1, 1, 1, 1);
    tool_layout->addWidget(createToolButton(tool_widget, "Airbrush", ":/assets/icons/airbrush.png", 5, tool_group), 1, 2, 1, 1);

    // Final Setup
    tool_widget->setLayout(tool_layout);
    addWidget(tool_widget);
    addSeparator();

    // --------------------------------
    // |           History            |
    // --------------------------------

    // History Widget & Layout
    QWidget* history_widget = new QWidget(this);
    QGridLayout* history_layout = new QGridLayout(history_widget);

    // Label
    history_layout->addWidget(createLabel(history_widget, "History"), 2, 0, 1, 2);

    // Undo
    QToolButton* history_undo = createButton(history_widget, "Undo", ":/assets/icons/undo.png");
    history_undo->defaultAction()->setShortcut(QKeySequence::StandardKey::Undo);
    undo = history_undo->defaultAction();
    history_layout->addWidget(history_undo, 0, 0, 1, 1);

    // Redo
    QToolButton* history_redo = createButton(history_widget, "Redo", ":/assets/icons/redo.png");
    history_redo->defaultAction()->setShortcut(QKeySequence::StandardKey::Redo);
    redo = history_redo->defaultAction();
    history_layout->addWidget(history_redo, 0, 1, 1, 1);

    // Empty Space
    history_layout->addWidget(new QWidget(history_widget), 1, 0, 1, 2);

    // Final Setup
    history_widget->setLayout(history_layout);
    addWidget(history_widget);
    addSeparator();

    // --------------------------------
    // |            Colors            |
    // --------------------------------

    // Colors Widget & Layout
    QWidget* colors_widget = new QWidget(this);
    QGridLayout* colors_layout = new QGridLayout(colors_widget);

    // Label
    colors_layout->addWidget(createLabel(colors_widget, "Colors"), 2, 0, 1, 2);

    // Color Primary
    color_primary = createButton(colors_widget, "Primary Color", "");
    connect(color_primary->defaultAction(), &QAction::triggered, this, &RibbonUpper::askPrimaryColor);
    color_primary->setStyleSheet("QToolButton{background-color: " + primary_color.name() + ";}");
    color_primary->setFixedHeight(58);
    colors_layout->addWidget(color_primary, 0, 0, 2, 1);

    // Color Secondary
    color_secondary = createButton(colors_widget, "Secondary Color", "");
    connect(color_secondary->defaultAction(), &QAction::triggered, this, &RibbonUpper::askSecondaryColor);
    color_secondary->setStyleSheet("QToolButton{background-color: " + secondary_color.name() + ";}");
    color_secondary->setFixedHeight(58);
    colors_layout->addWidget(color_secondary, 0, 1, 2, 1);

    // Final Setup
    colors_widget->setLayout(colors_layout);
    addWidget(colors_widget);
    addSeparator();

    // --------------------------------
    // |              IO              |
    // --------------------------------

    // IO Widget & Layout
    QWidget* IO_widget = new QWidget(this);
    QGridLayout* IO_layout = new QGridLayout(IO_widget);

    // Label
    IO_layout->addWidget(createLabel(IO_widget, "File"), 2, 0, 1, 3);

    // Save
    QToolButton* IO_save = createButton(IO_widget, "Save", ":/assets/icons/save.png");
    IO_save->defaultAction()->setShortcut(QKeySequence::StandardKey::Save);
    save = IO_save->defaultAction();
    IO_layout->addWidget(IO_save, 0, 0, 1, 1);

    // Save As
    QToolButton* IO_save_as = createButton(IO_widget, "Save As", ":/assets/icons/save_as.png");
    IO_save_as->defaultAction()->setShortcut(QKeySequence::StandardKey::SaveAs);
    save_as = IO_save_as->defaultAction();
    IO_layout->addWidget(IO_save_as, 0, 1, 1, 1);

    // Load
    QToolButton* IO_load = createButton(IO_widget, "Load", ":/assets/icons/load.png");
    IO_load->defaultAction()->setShortcut(QKeySequence::StandardKey::Open);
    load = IO_load->defaultAction();
    IO_layout->addWidget(IO_load, 0, 2, 1, 1);

    // Copy
    QToolButton* IO_copy = createButton(IO_widget, "Copy", ":/assets/icons/copy.png");
    IO_copy->defaultAction()->setShortcut(QKeySequence::StandardKey::Copy);
    copy = IO_copy->defaultAction();
    IO_layout->addWidget(IO_copy, 1, 0, 1, 1);

    // Paste
    QToolButton* IO_paste = createButton(IO_widget, "Paste", ":/assets/icons/paste.png");
    IO_paste->defaultAction()->setShortcut(QKeySequence::StandardKey::Paste);
    paste = IO_paste->defaultAction();
    IO_layout->addWidget(IO_paste, 1, 1, 1, 1);

    // New
    QToolButton* IO_new = createButton(IO_widget, "New File", ":/assets/icons/new_file.png");
    IO_new->defaultAction()->setShortcut(QKeySequence::StandardKey::New);
    new_file = IO_new->defaultAction();
    IO_layout->addWidget(IO_new, 1, 2, 1, 1);

    // Final Setup
    IO_widget->setLayout(IO_layout);
    addWidget(IO_widget);
    addSeparator();

    // --------------------------------
    // |            Layers            |
    // --------------------------------

    // Layers Widget & Layout
    QWidget* layers_widget = new QWidget(this);
    QGridLayout* layers_layout = new QGridLayout(layers_widget);

    // Label
    layers_layout->addWidget(createLabel(layers_widget, "Layers"), 2, 0, 1, 2);

    // Layers Button
    QToolButton* layers_button = createButton(layers_widget, "Layers", ":/assets/icons/layers.png");
    layers_button->defaultAction()->setCheckable(true);
    layers_button->setFixedSize(58, 58);
    layers_button->setIconSize(layers_button->sizeHint() * 2);
    layers_button->setChecked(false);
    toggle_layers = layers_button->defaultAction();
    layers_layout->addWidget(layers_button, 0, 0, 2, 2);

    // Final Setup
    layers_widget->setLayout(layers_layout);
    addWidget(layers_widget);
    addSeparator();

    // --------------------------------
    // |          Selection           |
    // --------------------------------

    // Selection Widget & Layout
    QWidget* selection_widget = new QWidget(this);
    QGridLayout* selection_layout = new QGridLayout(selection_widget);

    // Label
    selection_layout->addWidget(createLabel(selection_widget, "Selection"), 2, 0, 1, 2);

    // Selection Button
    selection_layout->addWidget(createToolButton(selection_widget, "Selection", ":/assets/icons/selection.png", 9, tool_group), 0, 0, 1, 1);

    // Selection Delete
    QToolButton* selection_delete = createButton(selection_widget, "Delete current selection", ":/assets/icons/delete_selection.png");
    selection_delete->defaultAction()->setShortcut(QKeySequence(Qt::Key::Key_Backspace));
    delete_selection = selection_delete->defaultAction();
    selection_layout->addWidget(selection_delete, 0, 1, 1, 1);

    // Transparent Selection
    QToolButton* selection_transparent = createButton(selection_widget, "Transparent Selection", ":/assets/icons/transparent_selection.png");
    selection_transparent->defaultAction()->setCheckable(true);
    transparent_selection = selection_transparent->defaultAction();
    selection_layout->addWidget(selection_transparent, 1, 0, 1, 1);

    // Selection Place
    QToolButton* selection_place = createButton(selection_widget, "Place current selection", ":/assets/icons/place_selection.png");
    selection_place->defaultAction()->setShortcut(QKeySequence(Qt::Key::Key_Return));
    place_selection = selection_place->defaultAction();
    selection_layout->addWidget(selection_place, 1, 1, 1, 1);

    // Final Setup
    selection_widget->setLayout(selection_layout);
    addWidget(selection_widget);
    addSeparator();

    // --------------------------------
    // |          Tool Size           |
    // --------------------------------

    // Tool Size Widget & Layout
    QWidget* tool_size_widget = new QWidget(this);
    tool_size_widget->setFixedWidth(128);
    QGridLayout* tool_size_layout = new QGridLayout(tool_size_widget);

    // Label    
    tool_size_layout->addWidget(createLabel(tool_size_widget, "Size"), 2, 0, 1, 3);

    // Tool Size Icon
    QLabel* tool_size_tool_icon = new QLabel(tool_size_widget);
    tool_size_tool_icon->setPixmap(QIcon(":/assets/icons/pencil.png").pixmap(iconSize()));
    tool_size_tool_icon->setStyleSheet("QLabel{border: 1px solid #ffffff;}");
    tool_size_layout->addWidget(tool_size_tool_icon, 0, 0, 1, 1);

    // Tool Size
    QLineEdit* tool_size_size = new QLineEdit(settings.value("Tools/size", "1").toString(), tool_size_widget);
    tool_size_size->setValidator(new QIntValidator(1, 1000));
    tool_size_size->setToolTip("Tool Size");
    tool_size_size->installEventFilter(this);
    tool_size_size->setFixedHeight(tool_size_tool_icon->height() - 4);
    connect(tool_size_size, &QLineEdit::textChanged, this, &RibbonUpper::toolSizeChanged);
    tool_size_layout->addWidget(tool_size_size, 0, 1, 1, 2);

    // Airbrush Size Icon
    QLabel* tool_size_airbrush_icon = new QLabel(tool_size_widget);
    tool_size_airbrush_icon->setPixmap(QIcon(":/assets/icons/airbrush.png").pixmap(iconSize()));
    tool_size_airbrush_icon->setStyleSheet("QLabel{border: 1px solid #ffffff;}");
    tool_size_layout->addWidget(tool_size_airbrush_icon, 1, 0, 1, 1);

    // Airbrush Density
    QLineEdit* tool_size_density = new QLineEdit(settings.value("Tools/airbrush_density", "16").toString(), tool_size_widget);
    tool_size_density->setValidator(new QIntValidator(1, 1000));
    tool_size_density->setToolTip("Airbrush Density");
    tool_size_density->installEventFilter(this);
    tool_size_density->setFixedHeight(tool_size_tool_icon->height() - 4);
    connect(tool_size_density, &QLineEdit::textChanged, this, &RibbonUpper::toolDensityChanged);
    tool_size_layout->addWidget(tool_size_density, 1, 1, 1, 1);

    // Airbrush Diameter
    QLineEdit* tool_size_diameter = new QLineEdit(settings.value("Tools/airbush_diameter", "8").toString(), tool_size_widget);
    tool_size_diameter->setValidator(new QIntValidator(1, 1000));
    tool_size_diameter->setToolTip("Airbrush Diameter");
    tool_size_diameter->installEventFilter(this);
    tool_size_diameter->setFixedHeight(tool_size_tool_icon->height() - 4);
    connect(tool_size_diameter, &QLineEdit::textChanged, this, &RibbonUpper::toolDiameterChanged);
    tool_size_layout->addWidget(tool_size_diameter, 1, 2, 1, 1);

    // Final Setup
    tool_size_widget->setLayout(tool_size_layout);
    addWidget(tool_size_widget);
    addSeparator();

    // --------------------------------
    // |          Cap Style           |
    // --------------------------------

    // Cap Widget & Layout
    QWidget* cap_widget = new QWidget(this);
    QGridLayout* cap_layout = new QGridLayout(cap_widget);

    // Label    
    cap_layout->addWidget(createLabel(cap_widget, "Cap Style"), 2, 0, 1, 2);

    // Cap Styles
    cap_layout->addWidget(createToolButton(cap_widget, "Round Cap", ":/assets/icons/round_cap.png", 0, cap_group), 0, 0, 1, 1);
    cap_layout->addWidget(createToolButton(cap_widget, "Square Cap", ":/assets/icons/square_cap.png", 1, cap_group), 0, 1, 1, 1);
    cap_layout->addWidget(createToolButton(cap_widget, "Flat Cap", ":/assets/icons/flat_cap.png", 2, cap_group), 1, 0, 1, 1);

    // Final Setup
    cap_widget->setLayout(cap_layout);
    addWidget(cap_widget);
    addSeparator();
}

// ------------------------------------------------
// |                    Slots                     |
// ------------------------------------------------

void RibbonUpper::askPrimaryColor(){
    QColorDialog color_dialog(nullptr);
    color_dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
    color_dialog.setCurrentColor(primary_color);

    if (!color_dialog.exec())
        return;
    
    QColor selected_color = color_dialog.selectedColor();
    if (!selected_color.isValid())
        return;

    if (selected_color.alpha() == 0)
        selected_color = QColor(0, 0, 0, 0);

    primary_color = selected_color;
    emit colorChanged(selected_color, true);
    color_primary->setStyleSheet("QToolButton{background-color: " + primary_color.name() + ";}");
}

void RibbonUpper::askSecondaryColor(){
    QColorDialog color_dialog(nullptr);
    color_dialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
    color_dialog.setCurrentColor(secondary_color);

    if (!color_dialog.exec())
        return;
    
    QColor selected_color = color_dialog.selectedColor();
    if (!selected_color.isValid())
        return;

    if (selected_color.alpha() == 0)
        selected_color = QColor(0, 0, 0, 0);

    secondary_color = selected_color;
    emit colorChanged(selected_color, false);
    color_secondary->setStyleSheet("QToolButton{background-color: " + secondary_color.name() + ";}");
}

void RibbonUpper::updateColor(QColor color, bool is_primary){
    if (is_primary) {
        primary_color = color;
        color_primary->setStyleSheet("QToolButton{background-color: " + primary_color.name() + ";}");
    }
    else {
        secondary_color = color;
        color_secondary->setStyleSheet("QToolButton{background-color: " + secondary_color.name() + ";}");
    }
}

// ------------------------------------------------
// |                  Functions                   |
// ------------------------------------------------

QToolButton* RibbonUpper::createButton(QWidget* parent, QString tooltip, QString icon, QString shortcut) {
    QToolButton* button = new QToolButton(parent);
    QAction* action = new QAction(button);

    action->setIcon(QIcon(icon));
    action->setToolTip(tooltip);

    if (!shortcut.isEmpty())
        action->setShortcut(QKeySequence(shortcut));

    button->setDefaultAction(action);
    button->setIconSize(button->sizeHint());

    return button;
}

QToolButton* RibbonUpper::createToolButton(QWidget* parent, QString tooltip, QString icon, int id, QButtonGroup* group) {
    QToolButton* button = new QToolButton(parent);

    if (!icon.isEmpty())
        button->setIcon(QIcon(icon));

    button->setToolTip(tooltip);
    button->setCheckable(true);
    button->setChecked(id == 0);
    button->setIconSize(button->sizeHint());

    group->addButton(button, id);

    return button;
}

QLabel* RibbonUpper::createLabel(QWidget* parent, QString text) {
    QLabel* label = new QLabel(parent);
    label->setText(text);
    label->setAlignment(Qt::AlignmentFlag::AlignCenter);
    return label;
}

bool RibbonUpper::eventFilter(QObject* object, QEvent* event) {
    if (event->type() == QEvent::Type::Leave)
        if (QLineEdit* line_edit = qobject_cast<QLineEdit*>(object))
            if(line_edit->hasFocus())
                line_edit->clearFocus();

    return QToolBar().eventFilter(object, event);
}