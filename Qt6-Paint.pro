QT += widgets

SOURCES = src/main.cpp \
        src/core/history_manager.cpp \
        src/core/paint_manager.cpp \
        src/core/settings.cpp \
        src/core/viewport.cpp \
        src/widgets/canvas.cpp \
        src/widgets/layer_menu.cpp \
        src/widgets/main_window.cpp \
        src/widgets/ribbon_lower.cpp \
        src/widgets/ribbon_upper.cpp \
        src/dialogs/new_file.cpp

INCLUDEPATH += ./include

HEADERS = include/core/history_manager.hpp \
        include/core/paint_manager.hpp \
        include/core/settings.hpp \
        include/core/viewport.hpp \
        include/widgets/canvas.hpp \
        include/widgets/layer_menu.hpp \
        include/widgets/main_window.hpp \
        include/widgets/ribbon_lower.hpp \
        include/widgets/ribbon_upper.hpp \
        include/dialogs/new_file.hpp

RESOURCES = assets/resources.qrc

DESTDIR = ./
MOC_DIR = ./moc
OBJECTS_DIR = ./objects