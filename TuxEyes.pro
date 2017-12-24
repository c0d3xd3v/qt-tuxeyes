TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT += gui core widgets svg

SOURCES += \
    TuxEyes.cpp \
    main.cpp

DISTFILES += \
    tux.svg

HEADERS += \
    TuxEyes.h

RESOURCES += \
    resources.qrc
