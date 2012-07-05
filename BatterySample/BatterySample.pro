TEMPLATE = app
TARGET = BatterySample

CONFIG += qt warn_on debug_and_release cascades
LIBS += -lbbdevice

INCLUDEPATH += ../src
SOURCES += ../src/*.cpp
HEADERS += ../src/*.hpp ../src/*.h
RESOURCES += BatterySample.qrc

device {
	CONFIG(release, debug|release) {
		DESTDIR = o.le-v7
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

TRANSLATIONS += translations/BatterySample_de.ts \
                translations/BatterySample_en_GB.ts \
                translations/BatterySample_fr.ts \
                translations/BatterySample_it.ts \
                translations/BatterySample_es.ts