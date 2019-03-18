TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    eco2dsolver.cpp \
    reader.cpp \
    output.cpp

HEADERS += \
    eco2dsolver.h \
    reader.h \
    output.h
