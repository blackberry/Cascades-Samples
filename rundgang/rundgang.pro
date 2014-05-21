APP_NAME = rundgang

CONFIG += qt warn_on cascades10

LIBS += -lbbdata
LIBS += -lbbcascadespickers
LIBS += -lbbpim
LIBS += -lexif
LIBS += -lbbsystem
LIBS += -lbb
LIBS += -lbbcascadesmultimedia 
LIBS += -lbbdevice

include(config.pri)

# Add qml subfolders and relevant cpp files for translation 
lupdate_inclusion {
	SOURCES +=  $$quote($$BASEDIR/../src/common/*.c) \
             $$quote($$BASEDIR/../src/common/*.cpp) \
             $$quote($$BASEDIR/../src/rundgang/*.cpp) \
             $$quote($$BASEDIR/../src/rundgang/*.c) \
             $$quote($$BASEDIR/../assets/Common/*.qml) \
             $$quote($$BASEDIR/../assets/History/*.qml) \
             $$quote($$BASEDIR/../assets/Rundgang/*.qml) 

	HEADERS +=  $$quote($$BASEDIR/../src/common/*.h) \
             $$quote($$BASEDIR/../src/common/*.hpp) \
             $$quote($$BASEDIR/../src/rundgang/*.h) \
             $$quote($$BASEDIR/../src/rundgang/*.hpp)
}