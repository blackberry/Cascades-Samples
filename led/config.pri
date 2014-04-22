# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/device)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/device)

            LIBS += -lbbdevice

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        } else {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/device)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/device)

            LIBS += -lbbdevice

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }

    }

    CONFIG(release, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/device)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/device)

            LIBS += -lbbdevice

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/device)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/device)

            LIBS += -lbbdevice

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

config_pri_assets {
    OTHER_FILES += \
        $$quote($$BASEDIR/assets/images/background.png) \
        $$quote($$BASEDIR/assets/images/blue.png) \
        $$quote($$BASEDIR/assets/images/cyan.png) \
        $$quote($$BASEDIR/assets/images/green.png) \
        $$quote($$BASEDIR/assets/images/magenta.png) \
        $$quote($$BASEDIR/assets/images/red.png) \
        $$quote($$BASEDIR/assets/images/white.png) \
        $$quote($$BASEDIR/assets/images/yellow.png) \
        $$quote($$BASEDIR/assets/main.qml)
}

config_pri_source_group1 {
    SOURCES += $$quote($$BASEDIR/src/main.cpp)

}

INCLUDEPATH += $$quote($$BASEDIR/src)

lupdate_inclusion {
    SOURCES += \
        $$quote($$BASEDIR/../src/*.c) \
        $$quote($$BASEDIR/../src/*.c++) \
        $$quote($$BASEDIR/../src/*.cc) \
        $$quote($$BASEDIR/../src/*.cpp) \
        $$quote($$BASEDIR/../src/*.cxx) \
        $$quote($$BASEDIR/../assets/*.qml) \
        $$quote($$BASEDIR/../assets/*.js) \
        $$quote($$BASEDIR/../assets/*.qs) \
        $$quote($$BASEDIR/../assets/images/*.qml) \
        $$quote($$BASEDIR/../assets/images/*.js) \
        $$quote($$BASEDIR/../assets/images/*.qs)

    HEADERS += \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}.ts)
