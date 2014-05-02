APP_NAME = PushCollectorUI

CONFIG += qt warn_on cascades10
LIBS += -lbbsystem -lbbnetwork -lbbplatform -lbbdata -L../../pushCollectorCommon/arm -lpushCollectorCommon

INCLUDEPATH += ../src ../../pushCollectorCommon/public

include(config.pri)
