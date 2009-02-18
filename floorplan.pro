QT += svg xml

# binary
TARGET  = floorplan

INCLUDEPATH = include
LIB     = lib
# destination of the binary
#DESTDIR = bin
DESTDIR = .
BUILD   = build
SRC     = src
RES     = resources
UI      = ui

# add .ui -> .h file to include path
unix:UI_DIR  = $$INCLUDEPATH

# moc_ 
unix:MOC_DIR = $$BUILD

# .o
OBJECTS_DIR  = $$BUILD

# from resources -> cpp
RCC_DIR      = $$BUILD


INTERFACES = $$UI/floorplan2.ui

HEADERS =    $$INCLUDEPATH/svglistwidget.h \
             $$INCLUDEPATH/assistant.h \
             $$INCLUDEPATH/dragwidgetgrid.h \
             $$INCLUDEPATH/memoriadescritiva.h \
             $$INCLUDEPATH/pages.h \
             $$INCLUDEPATH/configdialog.h \
             $$INCLUDEPATH/mainwindow.h \
               

RESOURCES =  $$RES/floorplan.qrc

SOURCES =    $$SRC/svglistwidget.cpp \
             $$SRC/dragwidgetgrid.cpp \
             $$SRC/assistant.cpp \
             $$SRC/memoriadescritiva.cpp \
             $$SRC/pages.cpp \
             $$SRC/configdialog.cpp \
             $$SRC/mainwindow.cpp \
             $$SRC/main.cpp
 
# install
target.path = $$DESTDIR
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.pro images
#sources.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/draggableicons
INSTALLS += target 

