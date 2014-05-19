APP_NAME = hellocascades

CONFIG += qt warn_on cascades10

TRANSLATIONS += \
    $${TARGET}_en_GB.ts \
    $${TARGET}_fr_FR.ts \
    $${TARGET}_it_IT.ts \    
    $${TARGET}_de_DE.ts \
    $${TARGET}_es_ES.ts \
    $${TARGET}_ja_JP.ts \
    $${TARGET}_ko_KR.ts \
    $${TARGET}_zh_CN.ts \
    $${TARGET}_zh_TW.ts     
        
include(config.pri)
