#include "StereoCamCalibrator.h"
#include <QFile>
#include <QDebug>

StereoCamCalibrator::StereoCamCalibrator( QObject *parent ) :
    EXTRINSICS_FILE{ "extrinsics.yaml" },
    _init{ false },
    QObject( parent )
{

}

void StereoCamCalibrator::init( cv::Size frameSize, const QString &dirPath ) {
    cv::Mat R1, R2, P1, P2, Q;
    cv::Mat K1, K2, R;
    cv::Vec3d T;
    cv::Mat D1, D2;

    cv::Rect validROI[ 2 ];
    QString path{ dirPath };
    //path.append( EXTRINSICS_FILE );
    QFile f( path );
    if ( !f.exists( ) )
        qDebug( ) << "file:" << path << " not found";
    cv::FileStorage fs1( path.toStdString( ), cv::FileStorage::READ );

    fs1[ "K1" ] >> K1;
    fs1[ "K2" ] >> K2;
    fs1[ "D1" ] >> D1;
    fs1[ "D2" ] >> D2;
    fs1[ "R" ]  >> R;
    fs1[ "T" ]  >> T;

    fs1[ "R1" ] >> R1;
    fs1[ "R2" ] >> R2;
    fs1[ "P1" ] >> P1;
    fs1[ "P2" ] >> P2;
    fs1[ "Q" ]  >> Q;
    fs1[ "vRoi0" ] >> validROI[ 0 ];
    fs1[ "vRoi1" ] >> validROI[ 1 ];
// exported calib matrix
    fs1[ "P1" ] >> _p1;
    fs1[ "P2" ] >> _p2;
////////////////////////
    _targetRect = validROI[ 0 ] & validROI[ 1 ];

    cv::initUndistortRectifyMap( K1, D1, R1, P1, frameSize, CV_32F, _lmapx, _lmapy );
    cv::initUndistortRectifyMap( K2, D2, R2, P2, frameSize, CV_32F, _rmapx, _rmapy );

    _init = true;
}

void StereoCamCalibrator::calibrate( cv::Mat &left, cv::Mat &right ) {
    if ( !_init )
        return;
    if ( left.empty( ) || right.empty( ) ) {
        std::cout << "some frame is empty" << std::endl;
        return;
    }

    cv::remap( left , left , _lmapx, _lmapy, cv::INTER_LINEAR );
    cv::remap( right, right, _rmapx, _rmapy, cv::INTER_LINEAR );

    left  = left ( _targetRect );
    right = right( _targetRect );
}

cv::Mat StereoCamCalibrator::projectionMatrix1( ) const {
    return _p1;
}

cv::Mat StereoCamCalibrator::projectionMatrix2( ) const {
    return _p2;
}
