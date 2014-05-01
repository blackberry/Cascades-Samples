APP_NAME = pushCollectorHeadless

CONFIG += qt warn
LIBS += -lbb -lbbsystem -lbbnetwork -lbbplatform -lbbdata -L../../pushCollectorCommon/arm -lpushCollectorCommon

INCLUDEPATH += ../../pushCollectorCommon/public

include(config.pri)
