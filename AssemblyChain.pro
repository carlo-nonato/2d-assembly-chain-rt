TEMPLATE = app
TARGET = AssemblyChain
DESTDIR = bin

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += debug_and_release c++11

QT += widgets gui core

INCLUDEPATH += /usr/local/include/opencv4/opencv /usr/local/include/opencv4

RESOURCES = AssemblyChain.qrc

HEADERS += src/MainWindow.hpp \
           src/ConveyorBelt.hpp \
           src/ItemStack.hpp \
           src/Robot.hpp \
           src/Controller.hpp \
           src/Simulation.hpp \         

SOURCES += src/main.cpp \
           src/MainWindow.cpp \
           src/ConveyorBelt.cpp \
           src/ItemStack.cpp \
           src/Robot.cpp \
           src/Controller.cpp \
           src/Simulation.cpp \         
           
LIBS += -lpthread `pkg-config --libs opencv4`