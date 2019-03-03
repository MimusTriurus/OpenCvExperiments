#include "StereoCamCvMatProvider.h"
#include <QDebug>

StereoCamCvMatProvider::StereoCamCvMatProvider( QObject *parent ) :
    _leftCamIndx    { 1 },
    _rightCamIndx   { 0 },
    QObject( parent )
{

}

bool StereoCamCvMatProvider::init( const QString &configPath ) {
    if ( !ICvMatProvider::init( configPath ) )
        return false;
    _fs[ "leftCamIndex" ] >> _leftCamIndx;
    _fs[ "rightCamIndex" ] >> _rightCamIndx;
    _fs[ "frameWidth" ] >> _frameWidth;
    _fs[ "frameHeight" ] >> _frameHeight;
    _fs[ "useCalibration" ] >> _useCalibration;
    cv::String calibFilePath;
    _fs[ "calibrationFilePath" ] >> calibFilePath;
    _fs.release( );
    qDebug( ) << _leftCamIndx << _rightCamIndx << _frameWidth << _frameHeight << calibFilePath.c_str( ) << _useCalibration;
    if ( _useCalibration )
        _calibrator.init( cv::Size( _frameWidth, _frameHeight ), calibFilePath.c_str( ) );
    return true;
}

void StereoCamCvMatProvider::start( ) {
    _left.open( _leftCamIndx );
    _left.set( cv::CAP_PROP_FRAME_WIDTH, _frameWidth );
    _left.set( cv::CAP_PROP_FRAME_HEIGHT, _frameHeight );
    _right.set( cv::CAP_PROP_FRAME_WIDTH, _frameWidth );
    _right.set( cv::CAP_PROP_FRAME_HEIGHT, _frameHeight );

    if ( !_left.isOpened( ) )
        qDebug( ) << "error on open left cam:" << _leftCamIndx;
    else {
        qDebug( ) << "left cam is opend:" << _leftCamIndx
                  << _left.get( cv::CAP_PROP_FRAME_WIDTH )
                  << _left.get( cv::CAP_PROP_FRAME_HEIGHT );

    }
    _right.open( _rightCamIndx );
    if ( !_right.isOpened( ) ) {
        qDebug( ) << "error on open right cam";
    }
    else {
        qDebug( ) << "right cam is opend:" << _rightCamIndx
                  << _right.get( cv::CAP_PROP_FRAME_WIDTH )
                  << _right.get( cv::CAP_PROP_FRAME_HEIGHT );
    }
}
