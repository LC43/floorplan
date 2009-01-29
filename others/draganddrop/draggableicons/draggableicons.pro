QT += svg xml

HEADERS     = dragwidget.h \
			  qpixeltool.h \
			  dragwidgetgrid.h
RESOURCES   = draggableicons.qrc
SOURCES     = dragwidget.cpp \
			  dragwidgetgrid.cpp \
			  qpixeltool.cpp \
              main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/draggableicons
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.pro images
sources.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/draggableicons
INSTALLS += target sources
