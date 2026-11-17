#ifndef RESIZE_H
#define RESIZE_H

#include <QDialog>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSize>
#include <QLineEdit>
#include <QIntValidator>
#include <QCheckBox>

class Resize : public QDialog {
    Q_OBJECT

public:
    Resize(QWidget* parent, QSize size);

    QSize getSize();

private:
    QLineEdit* width_pixel_line_edit;
    QLineEdit* width_percentage_line_edit;
    QLineEdit* height_pixel_line_edit;
    QLineEdit* height_percentage_line_edit;
    QCheckBox* maintain_aspect_ratio_checkbox;
    QSize original_size;

private slots:
    void widthPercentageChanged();
    void heightPercentageChanged();
    void widthPixelChanged();
    void heightPixelChanged();
};

#endif