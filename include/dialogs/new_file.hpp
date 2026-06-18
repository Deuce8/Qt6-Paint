#ifndef NEW_FILE_H
#define NEW_FILE_H

#include <QDialog>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSize>
#include <QLineEdit>
#include <QIntValidator>

class NewFile : public QDialog {
    Q_OBJECT

public:
    NewFile(QWidget* parent, QSize size);

    QSize getSize();

private:
    QLineEdit* width_line_edit;
    QLineEdit* height_line_edit;
};

#endif