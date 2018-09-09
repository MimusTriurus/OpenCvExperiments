INCLUDEPATH += $$PWD/3rdparty/include/others
DEPENDPATH += $$PWD/3rdparty/include/others

win32:LIBS += -L$$PWD/3rdparty/lib/others/ -lCvMatProvider
