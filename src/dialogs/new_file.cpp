#include "dialogs/new_file.hpp"

// ------------------------------------------------
// |                 Constructor                  |
// ------------------------------------------------

NewFile::NewFile(QWidget* parent, QSize size) : QDialog(parent) {
    setFixedSize(256, 108);
    setWindowTitle("New File");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(8, 8, 8, 8);

    // --------------------------------
    // |     New File Label Setup     |
    // --------------------------------

    QLabel* label = new QLabel(this);
    label->setText("New File");
    QFont font = label->font();
    font.setBold(true);
    font.setUnderline(true);
    label->setFont(font);
    label->setContentsMargins(0, 0, 0, 0);
    label->setAlignment(Qt::AlignTop);

    // --------------------------------
    // |       Text Enter Setup       |
    // --------------------------------

    QHBoxLayout* width_layout = new QHBoxLayout();
    width_layout->setContentsMargins(0, 0, 0, 0);

    QLabel* width_label = new QLabel(this);
    width_label->setText("Width:");
    width_label->setFixedWidth(64);
    width_layout->addWidget(width_label);

    width_line_edit = new QLineEdit(QString::number(size.width()), this);
    width_line_edit->setValidator(new QIntValidator(width_line_edit));
    width_layout->addWidget(width_line_edit);

    QHBoxLayout* height_layout = new QHBoxLayout();
    height_layout->setContentsMargins(0, 0, 0, 0);

    QLabel* height_label = new QLabel(this);
    height_label->setText("Height:");
    height_label->setFixedWidth(64);
    height_layout->addWidget(height_label);

    height_line_edit = new QLineEdit(QString::number(size.height()), this);
    height_line_edit->setValidator(new QIntValidator(width_line_edit));
    height_layout->addWidget(height_line_edit);

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
    layout->addLayout(width_layout);
    layout->addLayout(height_layout);
    layout->addLayout(button_layout);
    this->setLayout(layout);
}

QSize NewFile::getSize() {
    return QSize(width_line_edit->text().toInt(), height_line_edit->text().toInt());
}

void NewFile::closeEvent(QCloseEvent* event) {
    emit rejected();
}