QT += svg xml
INTERFACES = floorplan.ui
HEADERS =    svglistwidget.h \
		     dragwidgetgrid.h \
             memoriadescritiva.h \
             mainwindow.h
RESOURCES =  floorplan.qrc
SOURCES =    svglistwidget.cpp \
             dragwidgetgrid.cpp \
             memoriadescritiva.cpp \
             mainwindow.cpp \
             main.cpp
 
# install
target.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/draggableicons
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.pro images
sources.path = $$[QT_INSTALL_EXAMPLES]/draganddrop/draggableicons
INSTALLS += target sources