TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
INCLUDEPATH += $$PWD/irrlicht/include
LIBS += -L"$$PWD/irrlicht/lib/Linux" -lIrrlicht
