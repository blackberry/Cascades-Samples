TEMPLATE = app
TARGET = pushCollector

CONFIG += qt warn_on debug_and_release cascades10
LIBS += -lbbsystem -lbbnetwork -lbbplatform -lbbdata

INCLUDEPATH += ../src
SOURCES += ../src/*.cpp
HEADERS += ../src/*.hpp ../src/*.h

SOURCES += ../src/dao/*.cpp
HEADERS += ../src/dao/*.hpp ../src/dao/*.h

SOURCES += ../src/service/*.cpp
HEADERS += ../src/service/*.hpp ../src/service/*.h

SOURCES += ../src/vo/*.cpp
HEADERS += ../src/vo/*.hpp ../src/vo/*.h

lupdate_inclusion {
    SOURCES += ../assets/*.qml
}

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

suredelete.target = sureclean
suredelete.commands = $(DEL_FILE) $${MOC_DIR}/*; $(DEL_FILE) $${RCC_DIR}/*; $(DEL_FILE) $${UI_DIR}/*
suredelete.depends = distclean

QMAKE_EXTRA_TARGETS += suredelete

TRANSLATIONS = \
    $${TARGET}_en_GB.ts \
    $${TARGET}_fr.ts \
    $${TARGET}_it.ts \
    $${TARGET}_de.ts \
    $${TARGET}_es.ts \
    $${TARGET}.ts

