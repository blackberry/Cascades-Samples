OTHER_FILES += $$PWD/cascades.qdocconf

online_docs.commands = qdoc3 $$PWD/cascades.qdocconf
docs.depends = online_docs
QMAKE_EXTRA_TARGETS = docs online_docs
QMAKE_CLEAN += "-r $$PWD/html"
