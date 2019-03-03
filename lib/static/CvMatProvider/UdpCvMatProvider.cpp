#include "UdpCvMatProvider.h"
#include <MatSerialization.h>
#include <QDebug>

UdpCvMatProvider::UdpCvMatProvider( QObject *parent ) :
    QObject( parent ) {
//    connect( &_receiver, SIGNAL( imgDataReceived( QByteArray ) ),
//             this, SLOT( onReceiveData( QByteArray ) ) );
}

bool UdpCvMatProvider::init( const QString &configPath ) {
    Q_UNUSED( configPath )
    return true;
}

void UdpCvMatProvider::start( ) {
    this->_receiver.listen( 10000 );
}

void UdpCvMatProvider::stop( ) {
    this->_receiver.stopListen( );
}

bool UdpCvMatProvider::read( cv::Mat &frame ) {
    _mat.copyTo( frame );
    return true;
}

void UdpCvMatProvider::onReceiveData( QByteArray data ) {
    _mat = MatSerialization::deserializeMat( data );
}
