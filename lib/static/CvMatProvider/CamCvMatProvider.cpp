#include "CamCvMatProvider.h"
#include <QDebug>

CamCvMatProvider::CamCvMatProvider( QObject *parent ) :
    _cameraIndex{ 0 },
    QObject( parent ) {

}

bool CamCvMatProvider::init( const QString &configPath ) {
    if ( !ICvMatProvider::init( configPath ) )
        return false;
    _fs[ "leftCamIndex" ] >> _cameraIndex;
    _fs[ "frameWidth" ] >> _frameWidth;
    _fs[ "frameHeight" ] >> _frameHeight;
    _fs[ "useCalibration" ] >> _useCalibration;
    cv::String calibFilePath;
    _fs[ "calibrationFilePath" ] >> calibFilePath;
    _fs.release( );
    qDebug( ) << _cameraIndex << _frameWidth << _frameHeight << calibFilePath.c_str( );
    if ( _useCalibration )
        _calibrator.init( cv::Size( _frameWidth, _frameHeight ), calibFilePath.c_str( ) );
    return true;
}

void CamCvMatProvider::start( ) {
    _left.open( _cameraIndex );
    if ( !_left.isOpened( ) )
        qDebug( ) << "error on open cam" << _cameraIndex;
    else
        qDebug( ) << "cam is opend";
}
