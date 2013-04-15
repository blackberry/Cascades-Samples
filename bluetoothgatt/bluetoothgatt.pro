APP_NAME = bluetoothgatt

CONFIG += qt warn_on cascades10

INCLUDEPATH += ../src ${QNX_TARGET}/usr/include/btapi ${QNX_TARGET}/usr/include/bb/system
DEPENDPATH += ../src ${QNX_TARGET}/usr/include/btapi ${QNX_TARGET}/usr/include/bb/system
LIBS += -lbtapi -lbbsystem

include(config.pri)
