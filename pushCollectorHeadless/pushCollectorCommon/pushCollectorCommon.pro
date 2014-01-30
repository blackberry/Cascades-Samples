include(config.pri)

TEMPLATE = lib
TARGET = pushCollectorCommon

DEFINES += PUSHCOLLECTOR_BUILD_LIB

CONFIG += qt warn_on
LIBS += -lbbsystem -lbbnetwork -lbbplatform -lbbdata

INCLUDEPATH += ../public ../public/dao ../public/service ../public/vo
HEADERS += ../public/*.hpp ../public/*.h
HEADERS += ../public/dao/*.hpp ../public/dao/*.h
HEADERS += ../public/service/*.hpp ../public/service/*.h
HEADERS += ../public/vo/*.hpp ../public/vo/*.h

