//#include "WindowStereoBM.h"
#include <QApplication>
#include "WindowSelectAlgorithm.h"

int main( int argc, char *argv[ ] ) {
    QApplication a( argc, argv );
    WindowSelectAlgorithm w;
    w.show( );

    return a.exec( );
}
