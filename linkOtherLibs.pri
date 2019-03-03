win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/static/CvMatProvider/release/ -lCvMatProvider
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/static/CvMatProvider/debug/ -lCvMatProvider
else:unix:!macx: LIBS += -L$$OUT_PWD/../../lib/static/CvMatProvider/ -lCvMatProvider

INCLUDEPATH += $$PWD/lib/static/CvMatProvider
DEPENDPATH += $$PWD/lib/static/CvMatProvider

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/release/libCvMatProvider.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/debug/libCvMatProvider.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/release/CvMatProvider.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/debug/CvMatProvider.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/libCvMatProvider.a
