APP_NAME = quotes

CONFIG += qt warn_on cascades10

LIBS   += -lbbdata

include(config.pri)

# Add qml subfolders 
lupdate_inclusion {
	SOURCES += $$quote($$BASEDIR/../assets/QuotePage/*.qml) \
             $$quote($$BASEDIR/../assets/QuotesListPage/*.qml) 
}