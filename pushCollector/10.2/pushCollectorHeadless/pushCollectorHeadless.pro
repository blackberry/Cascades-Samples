APP_NAME = pushCollectorHeadless

CONFIG += qt warn_on cascades10
LIBS += -lbbsystem -lbbnetwork -lbbplatform -lbbdata -L../../pushCollectorCommon/arm -lpushCollectorCommon

INCLUDEPATH += ../../pushCollectorCommon/public

include(config.pri)
