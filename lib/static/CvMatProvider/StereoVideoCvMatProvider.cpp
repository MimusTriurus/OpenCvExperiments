#include "StereoVideoCvMatProvider.h"
#include <QDebug>

StereoVideoCvMatProvider::StereoVideoCvMatProvider( QObject *parent ) :
    QObject( parent )
{

}

bool StereoVideoCvMatProvider::init( const QString &configPath ) {
    if ( !ICvMatProvider::init( configPath ) )
        return false;
    _fs[ "leftVideoPath" ] >> _leftVideoPath;
    _fs[ "rightVideoPath" ] >> _rightVideoPath;
    _fs[ "frameWidth" ] >> _frameWidth;
    _fs[ "frameHeight" ] >> _frameHeight;
    _fs[ "useCalibration" ] >> _useCalibration;
    cv::String calibFilePath;
    _fs[ "calibrationFilePath" ] >> calibFilePath;
    _fs.release( );
    qDebug( ) << _leftVideoPath.c_str( ) << _rightVideoPath.c_str( ) << _frameWidth << _frameHeight << calibFilePath.c_str( ) << _useCalibration;
    if ( _useCalibration )
        _calibrator.init( cv::Size( _frameWidth, _frameHeight ), calibFilePath.c_str( ) );
    return true;
}

void StereoVideoCvMatProvider::start( ) {
    _left.open( _leftVideoPath );
    _left.set( cv::CAP_PROP_FRAME_WIDTH, _frameWidth );
    _left.set( cv::CAP_PROP_FRAME_HEIGHT, _frameHeight );
    _right.set( cv::CAP_PROP_FRAME_WIDTH, _frameWidth );
    _right.set( cv::CAP_PROP_FRAME_HEIGHT, _frameHeight );

    if ( !_left.isOpened( ) )
        qDebug( ) << "error on open left video:" << _leftVideoPath.c_str( );
    else {
        qDebug( ) << "left video is opend:"
                  << _left.get( cv::CAP_PROP_FRAME_WIDTH )
                  << _left.get( cv::CAP_PROP_FRAME_HEIGHT );

    }
    _right.open( _rightVideoPath );
    if ( !_right.isOpened( ) ) {
        qDebug( ) << "error on open right video:" << _rightVideoPath.c_str( );
    }
    else {
        qDebug( ) << "right video is opend:"
                  << _right.get( cv::CAP_PROP_FRAME_WIDTH )
                  << _right.get( cv::CAP_PROP_FRAME_HEIGHT );
    }
    qDebug( ) << "*****************";
}
