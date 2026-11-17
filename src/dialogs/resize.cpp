#include "dialogs/resize.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

Resize::Resize(QWidget* parent, QSize size) : QDialog(parent) {
    setFixedSize(256, 128);
    setWindowTitle("Resize");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(8, 8, 8, 8);

    original_size = size;

    // --------------------------------
    // |     New File Label Setup     |
    // --------------------------------

    QLabel* label = new QLabel(this);
    label->setText("Resize");
    QFont font = label->font();
    font.setBold(true);
    font.setUnderline(true);
    label->setFont(font);
    label->setContentsMargins(0, 0, 0, 0);
    label->setAlignment(Qt::AlignTop);

    // --------------------------------
    // |  Maintain Aspect Ratio Setup |
    // --------------------------------

    QBoxLayout* maintain_aspect_ratio_layout = new QHBoxLayout();
    maintain_aspect_ratio_layout->setContentsMargins(0, 0, 0, 0);

    QLabel* maintain_aspect_ratio_label = new QLabel(this);
    maintain_aspect_ratio_label->setText("Maintain Aspect Ratio:");
    maintain_aspect_ratio_layout->addWidget(maintain_aspect_ratio_label);

    maintain_aspect_ratio_checkbox = new QCheckBox(this);
    maintain_aspect_ratio_checkbox->setChecked(true);
    maintain_aspect_ratio_layout->addWidget(maintain_aspect_ratio_checkbox);

    // --------------------------------
    // |       Text Enter Setup       |
    // --------------------------------

    QHBoxLayout* width_layout = new QHBoxLayout();
    width_layout->setContentsMargins(0, 0, 0, 0);

    QLabel* width_label = new QLabel(this);
    width_label->setText("Width:");
    width_label->setFixedWidth(64);
    width_layout->addWidget(width_label);

    width_percentage_line_edit = new QLineEdit(QString::number(100), this);
    width_percentage_line_edit->setValidator(new QDoubleValidator(0, INT_MAX, 2, width_percentage_line_edit));
    connect(width_percentage_line_edit, &QLineEdit::textEdited, this, &Resize::widthPercentageChanged);
    width_layout->addWidget(width_percentage_line_edit);
    
    QLabel* width_percentage_label = new QLabel(this);
    width_percentage_label->setText("%");
    width_percentage_label->setFixedWidth(8);
    width_layout->addWidget(width_percentage_label);

    width_pixel_line_edit = new QLineEdit(QString::number(size.width()), this);
    width_pixel_line_edit->setValidator(new QIntValidator(width_pixel_line_edit));
    connect(width_pixel_line_edit, &QLineEdit::textEdited, this, &Resize::widthPixelChanged);
    width_layout->addWidget(width_pixel_line_edit);

    QLabel* width_pixel_label = new QLabel(this);
    width_pixel_label->setText("px");
    width_pixel_label->setFixedWidth(16);
    width_layout->addWidget(width_pixel_label);

    QHBoxLayout* height_layout = new QHBoxLayout();
    height_layout->setContentsMargins(0, 0, 0, 0);

    QLabel* height_label = new QLabel(this);
    height_label->setText("Height:");
    height_label->setFixedWidth(64);
    height_layout->addWidget(height_label);

    height_percentage_line_edit = new QLineEdit(QString::number(100), this);
    height_percentage_line_edit->setValidator(new QDoubleValidator(0, INT_MAX, 2, height_percentage_line_edit));
    connect(height_percentage_line_edit, &QLineEdit::textEdited, this, &Resize::heightPercentageChanged);
    height_layout->addWidget(height_percentage_line_edit);

    QLabel* height_percentage_label = new QLabel(this);
    height_percentage_label->setText("%");
    height_percentage_label->setFixedWidth(8);
    height_layout->addWidget(height_percentage_label);

    height_pixel_line_edit = new QLineEdit(QString::number(size.height()), this);
    height_pixel_line_edit->setValidator(new QIntValidator(height_pixel_line_edit));
    connect(height_pixel_line_edit, &QLineEdit::textEdited, this, &Resize::heightPixelChanged);
    height_layout->addWidget(height_pixel_line_edit);

    QLabel* height_pixel_label = new QLabel(this);
    height_pixel_label->setText("px");
    height_pixel_label->setFixedWidth(16);
    height_layout->addWidget(height_pixel_label);

    // --------------------------------
    // |        Buttons Setup         |
    // --------------------------------

    QHBoxLayout* button_layout = new QHBoxLayout();
    button_layout->setContentsMargins(0, 0, 0, 0);

    QPushButton* accept = new QPushButton(this);
    accept->setText("Ok");
    connect(accept, &QPushButton::pressed, this, &QDialog::accept);
    button_layout->addWidget(accept);

    QPushButton* cancel = new QPushButton(this);
    cancel->setText("Cancel");
    connect(cancel, &QPushButton::pressed, this, &QDialog::reject);
    button_layout->addWidget(cancel);

    // --------------------------------
    // |         Final Setup          |
    // --------------------------------

    layout->addWidget(label);
    layout->addLayout(maintain_aspect_ratio_layout);
    layout->addLayout(width_layout);
    layout->addLayout(height_layout);
    layout->addLayout(button_layout);
    this->setLayout(layout);
}

// ------------------------------------------------
// |                    Slots                     |
// ------------------------------------------------

void Resize::widthPixelChanged() {
    width_percentage_line_edit->setText(QString::number(static_cast<double>(width_pixel_line_edit->text().toInt()) * 100.0 / static_cast<double>(original_size.width()), 'f', 2));

    if (maintain_aspect_ratio_checkbox->isChecked()) {
        height_percentage_line_edit->setText(width_percentage_line_edit->text());
        height_pixel_line_edit->setText(QString::number(static_cast<int>(static_cast<double>(original_size.height()) * height_percentage_line_edit->text().toDouble() / 100.0)));
    }
}

void Resize::widthPercentageChanged() {
    width_pixel_line_edit->setText(QString::number(static_cast<double>(original_size.width()) * width_percentage_line_edit->text().toDouble() / 100.0));

    if (maintain_aspect_ratio_checkbox->isChecked()) {
        height_percentage_line_edit->setText(width_percentage_line_edit->text());
        height_pixel_line_edit->setText(QString::number(static_cast<int>(static_cast<double>(original_size.height()) * height_percentage_line_edit->text().toDouble() / 100.0)));
    }
}

void Resize::heightPixelChanged() {
    height_percentage_line_edit->setText(QString::number(static_cast<double>(height_pixel_line_edit->text().toInt()) * 100.0 / static_cast<double>(original_size.height()), 'f', 2));

    if (maintain_aspect_ratio_checkbox->isChecked()) {
        width_percentage_line_edit->setText(height_percentage_line_edit->text());
        width_pixel_line_edit->setText(QString::number(static_cast<int>(static_cast<double>(original_size.width()) * width_percentage_line_edit->text().toDouble() / 100.0)));
    }
}

void Resize::heightPercentageChanged() {
    height_pixel_line_edit->setText(QString::number(static_cast<double>(original_size.height()) * height_percentage_line_edit->text().toDouble() / 100.0));

    if (maintain_aspect_ratio_checkbox->isChecked()) {
        width_percentage_line_edit->setText(height_percentage_line_edit->text());
        width_pixel_line_edit->setText(QString::number(static_cast<int>(static_cast<double>(original_size.width()) * width_percentage_line_edit->text().toDouble() / 100.0)));
    }
}

// ------------------------------------------------
// |                  Functions                   |
// ------------------------------------------------

QSize Resize::getSize() {
    return QSize(width_pixel_line_edit->text().toInt(), height_pixel_line_edit->text().toInt());
}