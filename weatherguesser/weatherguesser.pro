APP_NAME = weatherguesser

CONFIG += qt warn_on cascades10

LIBS   += -lbbdata -lbbsystem

include(config.pri)

# Add qml subfolders 
lupdate_inclusion {
	SOURCES += $$quote($$BASEDIR/../assets/WeatherPage/*.qml) 
}