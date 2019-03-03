#include "ICvMatProvider.h"
#include <QDebug>

ICvMatProvider::~ICvMatProvider( ) {

}

bool ICvMatProvider::init( const QString &configPath ) {
    return _fs.open( configPath.toStdString( ), cv::FileStorage::READ );
}

void ICvMatProvider::stop( ) {
    if ( _left.isOpened( ) ) {
        _left.release( );
    }
    if ( _right.isOpened( ) ) {
        _right.release( );
    }
}

bool ICvMatProvider::read( cv::Mat &frame ) {
    if ( _left.isOpened( ) ) {
        return _left.read( frame );
    }
    return false;
}

bool ICvMatProvider::read( cv::Mat &left, cv::Mat &right ) {
    if ( _left.isOpened( ) && _right.isOpened( ) ) {
        if ( _left.grab( ) && _right.grab( ) ) {
            bool result = _right.retrieve( right ) & _left.retrieve( left );
            if ( _useCalibration )
                _calibrator.calibrate( left, right );
            return result;
        }
        else {
            qDebug( ) << "not grab";
            return false;
        }
    }
    else {
        qDebug( ) << "not opened";
        return false;
    }
}

cv::Mat ICvMatProvider::projectionMatrix1( ) const {
    return _calibrator.projectionMatrix1( );
}

cv::Mat ICvMatProvider::projectionMatrix2( ) const {
    return _calibrator.projectionMatrix2( );
}
