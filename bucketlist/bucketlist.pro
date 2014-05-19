# libbbsystem has been added by right click on the project
# and selecting Configure->Add Library | "Standard BlackBerry platform Library"
APP_NAME = bucketlist

CONFIG += qt warn_on cascades10

LIBS += -lbbdata
LIBS += -lbbplatformbbm
LIBS += -lbbsystem

include(config.pri)

# Add qml subfolders and relevant cpp files for translation 
lupdate_inclusion {
	SOURCES +=  $$quote($$BASEDIR/../src/bucketbbm/*.c) \
             $$quote($$BASEDIR/../src/bucketbbm/*.cpp) \
             $$quote($$BASEDIR/../assets/items/*.qml) 

	HEADERS +=  $$quote($$BASEDIR/../src/bucketbbm/*.h) \
             $$quote($$BASEDIR/../src/bucketbbm/*.hpp)
}

# Add additional translation files, in this case only swedish.
TRANSLATIONS += \
    $${TARGET}_sv_SE.ts \