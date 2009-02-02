QT += svg xml
INTERFACES  = mainwindow.ui
HEADERS     = svglistwidget.h \
			  dragwidgetgrid.h \
			  mainwindow.h
RESOURCES   = draggableicons.qrc
SOURCES     = svglistwidget.cpp \
			  dragwidgetgrid.cpp \
			  mainwindow.cpp \
              main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/draggableicons
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.pro images
sources.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/draggableicons
INSTALLS += target sources
