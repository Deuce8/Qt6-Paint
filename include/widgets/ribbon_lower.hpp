#ifndef RIBBON_LOWER_H
#define RIBBON_LOWER_H

#include <QToolBar>
#include <QWidget>
#include <QLabel>
#include <QSettings>
#include <QLayout>

class RibbonLower : public QToolBar {
    Q_OBJECT

public:
    RibbonLower(QWidget* parent);

private:
    QLabel* mouse_text;
    QLabel* selection_text;
    QLabel* canvas_text;
    QLabel* layer_text;
    QLabel* zoom_text;

public slots:
    void updateMousePosition(QPoint position);
    void updateSelectionSize(QSize size);
    void updateCanvasSize(QSize size);
    void updateLayer(QString layer);
    void updateZoom(float zoom);
};

#endif