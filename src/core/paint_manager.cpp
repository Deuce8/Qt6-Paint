#include "core/paint_manager.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

PaintManager::PaintManager(QObject* parent, QWidget* canvas) : QObject(parent) {
    file_path = QDir::homePath();
    
    this->canvas = canvas;
    QSettings settings = QSettings(this);

    // Selection
    selection_rect.reset();
    selection_image.reset();
    dragging_selection = false;
    drag_position.reset();
    cleared_selection = false;
    transparent_selection = false;

    // Colors
    primary_color = QColor(settings.value("Colors/primary_color", "000000").toString());
    secondary_color = QColor(settings.value("Colors/secondary_color", "000000").toString());
    color = primary_color;

    // Coordinates
    last_mouse_point = QPoint(0, 0);
    last_mouse_point_active.reset();
    last_mouse_release.reset();
    last_mouse_click.reset();
    left_mouse_pressed = false;
    right_mouse_pressed = false;

    // Tool Data
    tool_id = 0;
    tool_size = settings.value("Tools/size", 1).toInt();;
    airbrush_diameter = settings.value("Tools/airbrush_diameter", 8).toInt();
    airbrush_density = settings.value("Tools/airbrush_density", 16).toInt();

    // Viewport
    offset = QPointF(0, 0);
    zoom = 1;

    // Layers
    layer_index = 0;
    image_size = QSize(settings.value("Canvas/width", -1).toInt(), settings.value("Canvas/height", -1).toInt());
    QImage layer = QImage(image_size, QImage::Format::Format_ARGB32);
    layer.fill(secondary_color);
    layers = QVector<QImage>(1, layer);
}

// ------------------------------------------------
// |                    Slots                     |
// ------------------------------------------------

// --------------------------------
// |       Selection Slots        |
// --------------------------------

void PaintManager::moveSelectionUp() {
    if (!selection_rect)
        return;
    
    if (!cleared_selection)
        clearUnderSelection();

    selection_rect.value().translate(0, -1);
    selection_rect = clampRect(selection_rect.value());
    canvas->update();
}

void PaintManager::moveSelectionDown() {
    if (!selection_rect)
        return;
    
    if (!cleared_selection)
        clearUnderSelection();

    selection_rect.value().translate(0, 1);
    selection_rect = clampRect(selection_rect.value());
    canvas->update();
}

void PaintManager::moveSelectionLeft() {
    if (!selection_rect)
        return;
    
    if (!cleared_selection)
        clearUnderSelection();

    selection_rect.value().translate(-1, 0);
    selection_rect = clampRect(selection_rect.value());
    canvas->update();
}

void PaintManager::moveSelectionRight() {
    if (!selection_rect)
        return;
    
    if (!cleared_selection)
        clearUnderSelection();

    selection_rect.value().translate(1, 0);
    selection_rect = clampRect(selection_rect.value());
    canvas->update();
}

// --------------------------------
// |       File Management        |
// --------------------------------

void PaintManager::dropEvent(QDropEvent* event) {
    const QMimeData* mime_data = event->mimeData();

    if (mime_data->hasImage())
        loadImage(mime_data->imageData().value<QImage>());
    
    if (mime_data->hasUrls()) {
        QString file_path = mime_data->urls()[0].toLocalFile();
        if (file_path.isEmpty())
            return;

        loadImageFromPath(file_path);
    }
}

void PaintManager::loadImageFromPath(QString file_path) {
    if (file_path.isEmpty())
        return;

    QImage image = QImage(file_path);
    if (image.isNull())
        return;

    loadImage(image);
    this->file_path = file_path;
}

void PaintManager::loadImage(QImage image) {
    if (image.isNull())
        return;

    if (image.format() != QImage::Format::Format_ARGB32)
        image = image.convertToFormat(QImage::Format::Format_ARGB32);

    layers.clear();
    layers.append(image);
    layer_index = 0;
    image_size = image.size();

    emit layer_size_changed(image_size);
    emit reset_layers();
    emit create_restore_point();
    canvas->update();
}

void PaintManager::loadFile() {
    file_path = QFileDialog::getOpenFileName(canvas, "Open Image File", file_path, "Image Files (*.png *.jpg *.bmp *.jpeg);;All Files (*)");
    if (file_path.isEmpty())
        return;
    
    loadImageFromPath(file_path);
}

void PaintManager::saveFileAs() {
    file_path = QFileDialog::getSaveFileName(canvas, "Save Image File", file_path, "Image Files (*.png *.jpg *.bmp *.jpeg);;All Files (*)");
    if (file_path.isEmpty())
        return;

    QImage image = QImage(image_size, QImage::Format::Format_ARGB32);
    image.fill(Qt::GlobalColor::transparent);
    QPainter* painter = new QPainter(&image);

    for (int i = 0; i < layers.count(); i++)
        painter->drawImage(0, 0, layers[i]);

    painter->end();
    delete painter;

    image.save(file_path);
}

void PaintManager::saveFile() {
    if (!file_path.endsWith(".png", Qt::CaseInsensitive) &&
        !file_path.endsWith(".bmp", Qt::CaseInsensitive) &&
        !file_path.endsWith(".jpg", Qt::CaseInsensitive) &&
        !file_path.endsWith(".jpeg", Qt::CaseInsensitive)) {
        saveFileAs();
        return;
    }

    QImage image = QImage(image_size, QImage::Format::Format_ARGB32);
    image.fill(Qt::GlobalColor::transparent);
    QPainter* painter = new QPainter(&image);
 
    for (int i = 0; i < layers.count(); i++)
        painter->drawImage(0, 0, layers[i]);

    painter->end();
    delete painter;

    image.save(file_path);
}

void PaintManager::copy() {
    if (!selection_image) {
        QImage image(image_size, QImage::Format::Format_ARGB32);
        image.fill(Qt::GlobalColor::transparent);
        QPainter* painter = new QPainter(&image);
        for (int i = 0; i < layers.count(); i++)
            painter->drawImage(0, 0, layers[i]);
        painter->end();
        delete painter;

        QGuiApplication::clipboard()->setImage(image);
    }
    else {
        QGuiApplication::clipboard()->setImage(selection_image.value());
    }
}

void PaintManager::paste() {
    QClipboard* clipboard = QGuiApplication::clipboard();

    if (!clipboard->mimeData()->hasImage())
        return;

    QImage image = clipboard->image();
    if (image.isNull())
        return;

    if (image.format() != QImage::Format::Format_ARGB32)
        image = image.convertToFormat(QImage::Format::Format_ARGB32);

    if (image.width() <= image_size.width() && image.height() <= image_size.height()) {
        if (tool_id == 9) {
            placeSelection();
            selection_image = image;
            selection_rect = QRect(QPoint(0, 0), image.size());
            selection_rect.value().moveTo(last_mouse_point + (selection_rect.value().topLeft() - selection_rect.value().center()));
            cleared_selection = true;
            selection_rect = clampRect(selection_rect.value());
            canvas->update();
            emit selection_size_changed(selection_rect.value().size());

            return;
        }

        QPainter* painter = new QPainter(&layers[layer_index]);
        painter->drawImage(0, 0, image);
        painter->end();
        delete painter;
        canvas->update();
        return;
    }

    selection_rect.reset();
    selection_image.reset();
    dragging_selection = false;
    drag_position.reset();
    cleared_selection = false;

    if (image.width() > image_size.width())
        image_size = QSize(image.width(), image_size.height());
    if (image.height() > image_size.height())
        image_size = QSize(image_size.width(), image.height());

    emit layer_size_changed(image_size);

    for (int i = 0; i < layers.count(); i++) {
        QImage layer = QImage(image_size, QImage::Format::Format_ARGB32);
        layer.fill(secondary_color);
        
        QPainter* painter = new QPainter(&layer);
        painter->drawImage(layers[i].rect(), layers[i]);
        painter->end();
        delete painter;

        layers[i] = layer;
    }

    QPainter* painter = new QPainter(&layers[layer_index]);
    painter->drawImage(0, 0, image);
    painter->end();
    delete painter;

    emit create_restore_point();
    canvas->update();
}

void PaintManager::newFile() {
    NewFile new_file(nullptr, image_size);
    if (new_file.exec() == QDialog::DialogCode::Rejected)
        return;
    
    if (new_file.getSize().isNull())
        return;

    image_size = new_file.getSize();
    QImage layer = QImage(image_size, QImage::Format::Format_ARGB32);
    layer.fill(secondary_color);

    layers.clear();
    layers.append(layer);
    layer_index = 0;

    emit layer_size_changed(image_size);
    emit reset_layers();
    emit create_restore_point();
    canvas->update();
}

// --------------------------------
// |       Tool Management        |
// --------------------------------

void PaintManager::setTool(int tool_id) {
    this->tool_id = tool_id;
    
    placeSelection();
}

void PaintManager::setCapStyle(int cap_id) {
    cap_style = cap_id;
}

void PaintManager::toggleTransparentSelection() {
    transparent_selection = !transparent_selection;
}

void PaintManager::updateToolSize(QString size) {
    if (size.isEmpty())
        return;

    tool_size = size.toInt();
}

void PaintManager::updateAirbrushDiameter(QString size) {
    if (size.isEmpty())
        return;

    airbrush_diameter = size.toInt();
}

void PaintManager::updateAirbrushDensity(QString size) {
    if (size.isEmpty())
        return;

    airbrush_density = size.toInt();
}

// --------------------------------
// |     Variable Management      |
// --------------------------------

void PaintManager::updateImageSize(QSize size) {
    image_size = size;
}

void PaintManager::updateLayerIndex(int index) {
    layer_index = index;
}

void PaintManager::updateLayers(QVector<QImage> layers) {
    this->layers = layers;
}

void PaintManager::updateZoom(float zoom) {
    this->zoom = zoom;
}

void PaintManager::updateOffset(QPointF offset) {
    this->offset = offset;
}

void PaintManager::updateColor(QColor color, bool is_primary) {
    this->color = color;
    if (is_primary)
        primary_color = color;
    else
        secondary_color = color;
}

// --------------------------------
// |     Layer Widget Backend     |
// --------------------------------

void PaintManager::setCurrentLayer(int index) {
    clearUnderSelection();
    layer_index = index;
    canvas->update();
}

void PaintManager::addLayer() {
    QImage layer = QImage(image_size, QImage::Format::Format_ARGB32);
    layer.fill(secondary_color);

    layers.append(layer);
    emit create_restore_point();
    canvas->update();
}

void PaintManager::removeCurrentLayer() {
    layers.remove(layer_index);
    emit create_restore_point();
    canvas->update();
}

void PaintManager::moveCurrentLayerUp() {
    if (layer_index == 0)
        return;
    
    layers.move(layer_index, layer_index - 1);
    
    emit create_restore_point();
    canvas->update();
}

void PaintManager::moveCurrentLayerDown() {
    if (layer_index == layers.count() - 1)
        return;

    layers.move(layer_index, layer_index + 1);

    emit create_restore_point();
    canvas->update();
}

// --------------------------------
// |        Canvas Backend        |
// --------------------------------

void PaintManager::paint() {
    QPainter* painter = new QPainter(canvas);
    painter->translate(offset);
    painter->scale(zoom * scale(), zoom * scale());
    
    drawLayerPreviewUnder(painter);
    painter->drawImage(0, 0, layers[layer_index]);
    drawLayerPreviewOver(painter);

    if (std::max(image_size.width(), image_size.height()) / zoom < 64)
        drawGrid(painter);
    drawBorder(painter);
    drawPreview(painter);
    
    painter->end();
    delete painter;
}

void PaintManager::mousePress(QMouseEvent* event) {
    if (!(event->button() & (Qt::MouseButton::LeftButton | Qt::MouseButton::RightButton)))
        return;

    color = event->button() == Qt::MouseButton::LeftButton ? primary_color : secondary_color;
    last_mouse_point_active = canvasSpace(event->position());
    last_mouse_click = last_mouse_point_active;

    if (event->button() & Qt::MouseButton::LeftButton)
        left_mouse_pressed = true;
    else
        right_mouse_pressed = true;

    switch (tool_id) {
        case 0:
            pencil(event);
            break;
        case 1:
            floodFill(event);
            break;
        case 2:
            matchingFill(event);
            break;
        case 3:
            eraser(event);
            break;
        case 4:
            eyedropper(event);
            break;
        case 5:
            airbrush(event);
            break;
        case 9:
            pressSelection(event);
            break;
    }

    canvas->update();
}

void PaintManager::mouseRelease(QMouseEvent* event) {
    if (!(event->button() & (Qt::MouseButton::LeftButton | Qt::MouseButton::RightButton)))
        return;

    if (event->button() & Qt::MouseButton::LeftButton)
        left_mouse_pressed = false;
    else
        right_mouse_pressed = false;

    switch (tool_id)
    {
        case 6:
            line(event);
            break;
        case 7:
            ellipse(event);
            break;
        case 8:
            rectangle(event);
            break;
        case 9:
            releaseSelection(event);
            break;
        case 10:
            crop(event);
            break;
    }

    if (!left_mouse_pressed && !right_mouse_pressed) {
        last_mouse_release = canvasSpace(event->position());
        last_mouse_point_active.reset();
        last_mouse_click.reset();
    }
    
    // We dont need to create a restore point if dragging, this prevents that. Selection functions will handle undo/redo stack.
    if (tool_id != 9)
        emit create_restore_point();
    
    canvas->update();
}

void PaintManager::mouseMove(QMouseEvent* event) {
    last_mouse_point = canvasSpace(event->position());

    if (!(event->buttons() & (Qt::MouseButton::LeftButton | Qt::MouseButton::RightButton)))
        return;

    switch (tool_id) {
        case 0:
            pencil(event);
            break;
        case 3:
            eraser(event);
            break;
        case 5:
            airbrush(event);
            break;
        case 7:
            if (last_mouse_point_active)
                emit selection_size_changed(QRect(last_mouse_click.value(), last_mouse_point + QPoint(-1, -1)).normalized().size() + QSize(1, 1));
            
            break;
        case 8:
            if (last_mouse_point_active)
                emit selection_size_changed(QRect(last_mouse_click.value(), last_mouse_point + QPoint(-1, -1)).normalized().size() + QSize(1, 1));
            
            break;
        case 9:
            moveSelection(event);
            break;
        case 10:
            moveCrop(event);
            break;
    }

    last_mouse_point_active = last_mouse_point;
    canvas->update();
}

// --------------------------------
// |         Tool Actions         |
// --------------------------------

void PaintManager::clear() {
    clearUnderSelection();

    layers[layer_index].fill(secondary_color);
    emit create_restore_point();
    canvas->update();
}

void PaintManager::mirrorHorizontal() {
    clearUnderSelection();

    if (!selection_image || !selection_rect || tool_id != 9) {
        layers[layer_index] = layers[layer_index].flipped(Qt::Orientation::Horizontal);
        emit create_restore_point();
    }
    else {
        selection_image = selection_image.value().flipped(Qt::Orientation::Horizontal);
    }

    canvas->update();
}

void PaintManager::mirrorVertical() {
    clearUnderSelection();

    if (!selection_image || !selection_rect || tool_id != 9) {
        layers[layer_index] = layers[layer_index].flipped(Qt::Orientation::Vertical);
        emit create_restore_point();
    }
    else {
        selection_image = selection_image.value().flipped(Qt::Orientation::Vertical);
    }

    canvas->update();
}

void PaintManager::rotateClockwise() {
    rotate(90);
}

void PaintManager::rotateCounterClockwise() {
    rotate(-90);
}

void PaintManager::deleteSelection() {
    clearUnderSelection();

    selection_rect.reset();
    selection_image.reset();
    dragging_selection = false;
    drag_position.reset();
    cleared_selection = false;
    emit create_restore_point();

    canvas->update();
}

void PaintManager::placeSelection() {
    if (!selection_image || !selection_rect)
        return;

    QPainter* painter = new QPainter(&layers[layer_index]);
    painter->drawImage(selection_rect.value(), selection_image.value());
    painter->end();
    delete painter;

    selection_rect.reset();
    selection_image.reset();
    dragging_selection = false;
    drag_position.reset();
    cleared_selection = false;
    emit create_restore_point();

    canvas->update();
}

// ------------------------------------------------
// |                  Functions                   |
// ------------------------------------------------

// --------------------------------
// |     Selection Functions      |
// --------------------------------

void PaintManager::pressSelection(QMouseEvent* event) {
    if (event->button() != Qt::MouseButton::LeftButton)
        return;

    if (!selection_rect)
        return;

    QPoint point = canvasSpace(event->position());
    if (selection_rect.value().contains(point)) {
        dragging_selection = true;
        drag_position = point;

        clearUnderSelection();
    }
}

void PaintManager::releaseSelection(QMouseEvent* event) {
    if (event->button() != Qt::MouseButton::LeftButton)
        return;

    QRect rect = unorderedQRect(clampPoint(last_mouse_click.value()), clampPoint(canvasSpace(event->position())));

    if (dragging_selection) {
        dragging_selection = false;
        drag_position.reset();
        return;
    }

    placeSelection();

    if (rect.width() <= 1 && rect.height() <= 1){
        emit selection_size_changed(QSize(0, 0));
        return;
    }

    selection_rect = rect;
    selection_image = layers[layer_index].copy(selection_rect.value());
    emit selection_size_changed(rect.size());
    if (transparent_selection)
        filterSelection();
}

void PaintManager::moveSelection(QMouseEvent* event) {
    if (!dragging_selection)
        emit selection_size_changed(unorderedQRect(clampPoint(last_mouse_click.value()), clampPoint(canvasSpace(event->position()))).size());

    if (!dragging_selection || !drag_position)
        return;

    QPointF point = clampPoint(canvasSpace(event->position()));
    QPointF delta = point - drag_position.value();
    drag_position = point;

    selection_rect->translate(delta.toPoint());
    selection_rect = clampRect(selection_rect.value());
}

void PaintManager::filterSelection() {
    if (!selection_image)
        return;

    const int width = selection_image.value().width();
    const int height = selection_image.value().height();

    const QRgb target_rgba = secondary_color.rgba();

    for (int y = 0; y < height; y++){
        QRgb* scan_line = reinterpret_cast<QRgb*>(selection_image.value().scanLine(y));
        
        for (int x = 0; x < width; x++) {
            if (scan_line[x] == target_rgba)
                scan_line[x] = qRgba(0, 0, 0, 0);
        }
    }
}

void PaintManager::clearUnderSelection() {
    if (tool_id != 9)
        return;

    if (!selection_image || !selection_rect)
        return;

    if (cleared_selection)
        return;

    cleared_selection = true;
    QPainter* painter = new QPainter(&layers[layer_index]);
    painter->setCompositionMode(QPainter::CompositionMode::CompositionMode_Source);
    painter->fillRect(selection_rect.value(), secondary_color);
    painter->end();
    delete painter;
}

void PaintManager::ensureSelectionFit() {
    if (selection_image.value().width() <= image_size.width() && selection_image.value().height() <= image_size.height())
        return;

    if (selection_image.value().width() > image_size.width())
        image_size = QSize(selection_image.value().width(), image_size.height());
    if (selection_image.value().height() > image_size.height())
        image_size = QSize(image_size.width(), selection_image.value().height());
    
    emit layer_size_changed(image_size);

    for (int i = 0; i < layers.count(); i++){
        QImage layer = QImage(image_size, QImage::Format::Format_ARGB32);
        layer.fill(secondary_color);

        QPainter* painter = new QPainter(&layer);
        painter->drawImage(layers[i].rect(), layers[i]);
        painter->end();
        delete painter;
    
        layers[i] = layer;
    }
}

// --------------------------------
// |        Crop Functions        |
// --------------------------------

void PaintManager::crop(QMouseEvent* event) {
    if (event->button() != Qt::MouseButton::LeftButton)
        return;

    QRect rect = unorderedQRect(clampPoint(last_mouse_click.value()), clampPoint(canvasSpace(event->position())));

    image_size = rect.size();
    emit layer_size_changed(image_size);

    for (int i = 0; i < layers.count(); i++){
        QImage layer = QImage(image_size, QImage::Format::Format_ARGB32);
        layer.fill(secondary_color);

        QPainter* painter = new QPainter(&layer);
        painter->drawImage(0, 0, layers[i].copy(rect));
        painter->end();
        delete painter;

        layers[i] = layer;
    }

    emit selection_size_changed(QSize(0, 0));
    canvas->update();
}

void PaintManager::moveCrop(QMouseEvent* event) {
    emit selection_size_changed(unorderedQRect(clampPoint(last_mouse_click.value()), clampPoint(canvasSpace(event->position()))).size());
}

// --------------------------------
// |       Layer Functions        |
// --------------------------------

QVector<QImage> PaintManager::getLayers() {
    return layers;
}

int PaintManager::getIndex() {
    return layer_index;
}

QSize PaintManager::getImageSize() {
    return image_size;
}

// --------------------------------
// |       Helper Functions       |
// --------------------------------

float PaintManager::scale() {
    return std::min(
        static_cast<float>(canvas->width()) / static_cast<float>(image_size.width()), 
        static_cast<float>(canvas->height()) / static_cast<float>(image_size.height())
    );
}

bool PaintManager::isValidPoint(QPoint point) {
    if (point.x() < 0 || point.y() < 0 || point.x() >= image_size.width() || point.y() >= image_size.height())
        return false;

    return true;
}

QPoint PaintManager::canvasSpace(QPointF point) {
    float inverse_scale = std::max(
        static_cast<float>(image_size.width()) / static_cast<float>(canvas->width()), 
        static_cast<float>(image_size.height()) / static_cast<float>(canvas->height())
    );

    return QPoint(
        (int)((point.x() - offset.x()) * inverse_scale / zoom),
        (int)((point.y() - offset.y()) * inverse_scale / zoom)
    );
}

QPoint PaintManager::canvasSpace(QPoint point) {
    return canvasSpace(point.toPointF());
}

QPoint PaintManager::clampPoint(QPoint point) {
    return QPoint(
        std::clamp(static_cast<int>(point.x()), 0, image_size.width() - 1),
        std::clamp(static_cast<int>(point.y()), 0, image_size.height() - 1)
    );
}

QRect PaintManager::clampRect(QRect rect){
    rect.moveTo(
        std::clamp(rect.left(), 0, image_size.width() - rect.width()),
        std::clamp(rect.top(), 0, image_size.height() - rect.height())
    );

    return rect;
}

QRect PaintManager::unorderedQRect(QPoint a, QPoint b) {
    QPoint _a = QPoint(
        std::min(a.x(), b.x()),
        std::min(a.y(), b.y())
    );
    QPoint _b = QPoint(
        std::max(a.x(), b.x()),
        std::max(a.y(), b.y())
    );

    return QRect(_a, _b);
}

void PaintManager::rotate(int measure) {
    clearUnderSelection();

    QTransform transform = QTransform();
    transform.rotate(measure);

    if (!selection_image || !selection_rect || tool_id != 9) {
        for (int i = 0; i < layers.count(); i++) {
            layers[i] = layers[i].transformed(transform, Qt::TransformationMode::SmoothTransformation);
        }

        image_size = layers[layer_index].size();
        emit layer_size_changed(image_size);

        emit create_restore_point();
    }
    else {
        QPoint center = clampPoint(selection_rect.value().center());

        selection_image = selection_image.value().transformed(transform, Qt::TransformationMode::SmoothTransformation);
        selection_rect = QRect(QPoint(0, 0), selection_image.value().size());
        selection_rect.value().moveTo(center + (selection_rect.value().topLeft() - selection_rect.value().center()));
        selection_rect = clampRect(selection_rect.value());
        ensureSelectionFit();
    }

    canvas->update();
}

QPainter* PaintManager::getPainter() {
    QPainter* painter = new QPainter(&layers[layer_index]);
    QPen pen = painter->pen();
    pen.setWidth(tool_size);
    pen.setColor(color);
    switch (cap_style) {
        case 0:
            pen.setCapStyle(Qt::PenCapStyle::RoundCap);
            break;
        case 1:
            pen.setCapStyle(Qt::PenCapStyle::SquareCap);
            break;
        case 2:
            pen.setCapStyle(Qt::PenCapStyle::FlatCap);
            break;
    }
    painter->setPen(pen);
    painter->setCompositionMode(QPainter::CompositionMode::CompositionMode_Source);

    return painter;
}

// --------------------------------
// |       Paint Functions        |
// --------------------------------

void PaintManager::drawGrid(QPainter* painter) {
    QPen pen = QPen(QColor("#40000000"));
    pen.setCosmetic(true);
    painter->setPen(pen);

    QVector<QLine> lines;
    for (int i = 1; i < image_size.width(); i++)
        lines.append(QLine(i, 0, i, image_size.height()));
    for (int i = 1; i < image_size.height(); i++)
        lines.append(QLine(0, i, image_size.width(), i));
    
    painter->drawLines(lines);
    pen.setCosmetic(false);
}

void PaintManager::drawBorder(QPainter* painter) {
    QPen pen = QPen(QColor("#40000000"));
    pen.setCosmetic(true);
    painter->setPen(pen);

    painter->drawLines(QVector<QLine>{
        {0, 0, 0, image_size.height()},
        {image_size.width(), 0, image_size.width(), image_size.height()},
        {0, 0, image_size.width(), 0},
        {0, image_size.height(), image_size.width(), image_size.height()}
    });
    pen.setCosmetic(false);
}

void PaintManager::drawPreview(QPainter* painter) {
    QColor current_color = QColor(color);
    current_color.setAlpha(128);

    QPen pen = painter->pen();
    pen.setColor(current_color);
    pen.setWidth(tool_size);
    switch (cap_style) {
        case 0:
            pen.setCapStyle(Qt::PenCapStyle::RoundCap);
            break;
        case 1:
            pen.setCapStyle(Qt::PenCapStyle::SquareCap);
            break;
        case 2:
            pen.setCapStyle(Qt::PenCapStyle::FlatCap);
            break;
    }
    pen.setCosmetic(false);
    pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
    painter->setPen(pen);

    switch (tool_id) {
        case 6:
            if (!last_mouse_point_active || !last_mouse_click)
                return;

            painter->drawLine(last_mouse_click.value() + QPointF(0.5 * (tool_size % 2), 0.5 * (tool_size % 2)), 
                            last_mouse_point_active.value() + QPointF(0.5 * (tool_size % 2), 0.5 * (tool_size % 2)));
            break;
        case 7:
            if (!last_mouse_point_active || !last_mouse_click)
                return;

            painter->drawEllipse(QRect(last_mouse_click.value(), last_mouse_point_active.value() + QPoint(-1, -1)).toRectF().adjusted(0.5, 0.5, 0.5, 0.5));
            break;
        case 8:
            if (!last_mouse_point_active || !last_mouse_click)
                return;
            
            painter->drawRect(QRect(last_mouse_click.value(), last_mouse_point_active.value() + QPoint(-1, -1)).toRectF().adjusted(0.5, 0.5, 0.5, 0.5));
            break;
        case 9:
            pen.setColor(QColor(QSettings().value("Selection/highlight_color", "#e62d42").toString()));
            pen.setCosmetic(true);
            pen.setCapStyle(Qt::PenCapStyle::SquareCap);
            pen.setStyle(Qt::PenStyle::DashLine);
            painter->setPen(pen);

            if (last_mouse_click && !dragging_selection)
                painter->drawRect(unorderedQRect(last_mouse_click.value(), last_mouse_point_active.value()));

            if (!selection_rect || !selection_image)
                return;

            painter->drawImage(selection_rect.value(), selection_image.value());
            painter->drawRect(selection_rect.value());
            break;
        case 10:
            pen.setColor(QColor(QSettings().value("Selection/highlight_color", "#e62d42").toString()));
            pen.setCosmetic(true);
            pen.setCapStyle(Qt::PenCapStyle::SquareCap);
            pen.setStyle(Qt::PenStyle::DashLine);
            painter->setPen(pen);

            if (!last_mouse_click)
                return;

            painter->drawRect(unorderedQRect(clampPoint(last_mouse_click.value()), clampPoint(last_mouse_point_active.value())));
            break;
    }
}

void PaintManager::drawLayerPreviewUnder(QPainter* painter) {
    painter->setOpacity(0.25);
    for (int i = 0; i < layer_index; i++)
        painter->drawImage(0, 0, layers[i]);
    painter->setOpacity(1);
}

void PaintManager::drawLayerPreviewOver(QPainter* painter) {
    painter->setOpacity(0.5);
    for (int i = layer_index + 1; i < layers.count(); i++)
        painter->drawImage(0, 0, layers[i]);
    painter->setOpacity(1);
}

// --------------------------------
// |        Tool Functions        |
// --------------------------------

void PaintManager::pencil(QMouseEvent* event) {
    QPoint point_a = last_mouse_point_active.value();
    QPoint point_b = canvasSpace(event->position());

    if (!isValidPoint(point_a) || !isValidPoint(point_b))
        return;

    QPainter* painter = getPainter();
    
    if (point_a == point_b)
        painter->drawPoint(point_a);
    else
        painter->drawLine(point_a, point_b);

    painter->end();
    delete painter;
}

void PaintManager::floodFill(QMouseEvent* event) {
    const int width = image_size.width();
    const int height = image_size.height();
    const QRgb target_rgba = layers[layer_index].pixelColor(canvasSpace(event->position())).rgb();

    if (target_rgba == color.rgba())
        return;
    
    std::vector<bool> seen(width * height, false);
    QVector<QPoint> queue = {canvasSpace(event->position())};
    int i = 0;
    const QImage image = layers[layer_index];
    while (i < queue.count()) {
        const QPoint point = queue[i];
        const int _x = point.x();
        const int _y = point.y();
        if (_x + 1 < width && !seen[(_x + 1) + width * _y] && image.pixel(point + QPoint(1, 0)) == target_rgba) {
            queue.append(point + QPoint(1, 0));
            seen[(_x + 1) + width * _y] = true;
        }
        if (_x - 1 >= 0 && !seen[(_x - 1) + width * _y] && image.pixel(point + QPoint(-1, 0)) == target_rgba) {
            queue.append(point + QPoint(-1, 0));
            seen[(_x - 1) + width * _y] = true;
        }
        if (_y + 1 < height && !seen[_x + width * (_y + 1)] && image.pixel(point + QPoint(0, 1)) == target_rgba) {
            queue.append(point + QPoint(0, 1));
            seen[_x + width * (_y + 1)] = true;
        }
        if (_y - 1 >= 0 && !seen[_x + width * (_y - 1)] && image.pixel(point + QPoint(0, -1)) == target_rgba) {
            queue.append(point + QPoint(0, -1));
            seen[_x + width * (_y - 1)] = true;
        }
        i++;
    }

    QPainter* painter = getPainter();
    QPen pen = painter->pen();
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawPoints(queue);
    painter->end();
    delete painter;
}

void PaintManager::matchingFill(QMouseEvent* event) {
    const QRgb target_rgba = layers[layer_index].pixelColor(canvasSpace(event->position())).rgba();

    if (target_rgba == color.rgba())
        return;

    for (int y = 0; y < image_size.height(); y++){
        QRgb* scan_line = reinterpret_cast<QRgb*>(layers[layer_index].scanLine(y));
        
        for (int x = 0; x < image_size.width(); x++) {
            if (scan_line[x] == target_rgba)
                scan_line[x] = color.rgba();
        }
    }

    canvas->update();
}

void PaintManager::eraser(QMouseEvent* event) {
    QPoint point_a = last_mouse_point_active.value();
    QPoint point_b = canvasSpace(event->position());

    if (!isValidPoint(point_a) || !isValidPoint(point_b))
        return;

    QPainter* painter = getPainter();
    QPen pen = painter->pen();
    pen.setColor(QColor(0, 0, 0, 0));
    painter->setPen(pen);

    if (point_a == point_b)
        painter->drawPoint(point_a);
    else
        painter->drawLine(point_a, point_b);

    painter->end();
    delete painter;
}

void PaintManager::eyedropper(QMouseEvent* event) {
    QPoint point = canvasSpace(event->position());
    if (!isValidPoint(point))
        return;

    QColor color = QColor(layers[layer_index].pixelColor(point));
    if (color.alpha() == 0)
        color = QColor(0, 0, 0, 0);

    if (event->buttons() && Qt::MouseButton::LeftButton) {
        primary_color = color;
        emit colorChanged(color, true);
    }
    else {
        secondary_color = color;
        emit colorChanged(color, false);
    }
}

void PaintManager::airbrush(QMouseEvent* event) {
    QPoint origin = canvasSpace(event->position());
    QVector<QPoint> points;

    static std::mt19937 rng(std::random_device{}());
    std::normal_distribution<double> gauss(0.0, airbrush_diameter);

    points.reserve(airbrush_density);
    for (int i = 0; i < airbrush_density; i++)
        points.append(QPoint(origin.x() + static_cast<int>(std::round(gauss(rng))), origin.y() + static_cast<int>(std::round(gauss(rng)))));

    QPainter* painter = getPainter();
    painter->drawPoints(points);
    painter->end();
    delete painter;
}

void PaintManager::line(QMouseEvent* event) {
    if (!last_mouse_click)
        return;
    
    QPainter* painter = getPainter();
    painter->drawLine(last_mouse_click.value() + QPointF(0.5 * (tool_size % 2), 0.5 * (tool_size % 2)), 
                    canvasSpace(event->position()) + QPointF(0.5 * (tool_size % 2), 0.5 * (tool_size % 2)));
    painter->end();
    delete painter;
}

void PaintManager::ellipse(QMouseEvent* event) {
    if (!last_mouse_click)
        return;
    
    QPainter* painter = getPainter();
    QPen pen = painter->pen();
    pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
    painter->setPen(pen);

    // https://doc.qt.io/archives/qt-5.15/qrect.html#rendering
    painter->drawEllipse(QRect(last_mouse_click.value(), canvasSpace(event->position()) + QPoint(-1, -1)));
    painter->end();
    delete painter;

    emit selection_size_changed(QSize(0, 0));
}

void PaintManager::rectangle(QMouseEvent* event) {
    if (!last_mouse_click)
        return;

    QPainter* painter = getPainter();
    QPen pen = painter->pen();
    pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
    painter->setPen(pen);

    // https://doc.qt.io/archives/qt-5.15/qrect.html#rendering
    painter->drawRect(QRect(last_mouse_click.value(), canvasSpace(event->position()) + QPoint(-1, -1)));
    painter->end();
    delete painter;

    emit selection_size_changed(QSize(0, 0));
}
