TEMPLATE = app
TARGET = led

CONFIG += qt warn_on debug_and_release cascades
LIBS += -lbbdevice

INCLUDEPATH += ../src
SOURCES += ../src/*.cpp
HEADERS += ../src/*.h ../src/*.hpp

device {
    CONFIG(release, debug|release) {
        DESTDIR = o.le-v7
        TEMPLATE = lib
        QMAKE_CXXFLAGS_RELEASE += -fvisibility=hidden -mthumb
    }
    CONFIG(debug, debug|release) {
        DESTDIR = o.le-v7-g
    }
}

simulator {
    CONFIG(release, debug|release) {
        DESTDIR = o
    }
    CONFIG(debug, debug|release) {
        DESTDIR = o-g
    }
}

OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui
