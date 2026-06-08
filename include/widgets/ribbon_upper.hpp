#ifndef RIBBON_UPPER_H
#define RIBBON_UPPER_H

#include <QToolBar>
#include <QWidget>
#include <QLabel>
#include <QSettings>
#include <QToolButton>
#include <QButtonGroup>
#include <QEvent>
#include <QObject>
#include <QLineEdit>
#include <QColorDialog>
#include <QGridLayout>
#include <QIntValidator>

class RibbonUpper : public QToolBar {
    Q_OBJECT

public:
    // Constructor
    RibbonUpper(QWidget* parent);

    // --------------------------------
    // |           Getters            |
    // --------------------------------

    // Image
    QAction* getRotateLeft() const { return rotate_left; }
    QAction* getRotateRight() const { return rotate_right; }
    QAction* getMirrorHorizontal() const { return mirror_horizontal; }
    QAction* getMirrorVertical() const { return mirror_vertical; }
    QAction* getClear() const { return clear; }
    QAction* getResetZoom() const { return reset_zoom; }

    // History
    QAction* getUndo() const { return undo; }
    QAction* getRedo() const { return redo; }

    // IO
    QAction* getSave() const { return save; }
    QAction* getSaveAs() const { return save_as; }
    QAction* getLoad() const { return load; }
    QAction* getCopy() const { return copy; }
    QAction* getPaste() const { return paste; }

    // Layers
    QAction* getToggleLayers() const { return toggle_layers; }

    // Selection
    QAction* getDeleteSelection() const { return delete_selection; }
    QAction* getTransparentSelection() const { return transparent_selection; }
    QAction* getPlaceSelection() const { return place_selection; }

private:
    // --------------------------------
    // |            Colors            |
    // --------------------------------

    QColor primary_color,  secondary_color;
    QToolButton* color_primary;
    QToolButton* color_secondary;

    // --------------------------------
    // |           QActions           |
    // --------------------------------

    // Image
    QAction* rotate_left;
    QAction* rotate_right;
    QAction* mirror_horizontal;
    QAction* mirror_vertical;
    QAction* clear;
    QAction* reset_zoom;

    // History
    QAction* undo;
    QAction* redo;

    // IO
    QAction* save;
    QAction* save_as;
    QAction* load;
    QAction* copy;
    QAction* paste;

    // Layers
    QAction* toggle_layers;

    // Selection
    QAction* delete_selection;
    QAction* transparent_selection;
    QAction* place_selection;

    // Tool Size
    QAction* tool_size;
    QAction* tool_density;
    QAction* tool_diameter;

    // --------------------------------
    // |          Functions           |
    // --------------------------------

    QToolButton* createButton(QWidget* parent, QString tooltip, QString icon, QString shortcut = "");
    QToolButton* createToolButton(QWidget* parent, QString tooltip, QString icon, int id, QButtonGroup* group);
    QLabel* createLabel(QWidget* parent, QString text);
    bool eventFilter(QObject* object, QEvent* event);

public slots:
    void updateColor(QColor color, bool is_primary);

private slots:
    void askPrimaryColor();
    void askSecondaryColor();

signals:
    void colorChanged(QColor, bool);
    void toolChanged(int);
    void capChanged(int);

    void toolSizeChanged(const QString&);
    void toolDensityChanged(const QString&);
    void toolDiameterChanged(const QString&);
};

#endif