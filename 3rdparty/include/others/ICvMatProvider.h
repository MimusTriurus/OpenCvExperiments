#ifndef ICVMATPROVIDER_H
#define ICVMATPROVIDER_H

#include <QObject>
#include "opencv2/opencv.hpp"
#include "VideoReceiver.h"
#include "StereoCamCalibrator.h"

class ICvMatProvider {
public:
    virtual ~ICvMatProvider( );
    virtual bool init( const QString &configPath );
    virtual void start( )                           = 0;
    virtual void stop( );
    virtual bool read( cv::Mat &frame );
    virtual bool read( cv::Mat &left, cv::Mat &right );
protected:
    ICvMatProvider( ) { }
    cv::VideoCapture _left;
    cv::VideoCapture _right;
    int _frameWidth{ 640 };
    int _frameHeight{ 480 };
    cv::Mat _mat;
    bool _useCalibration{ true };
    StereoCamCalibrator _calibrator;
    cv::FileStorage _fs;
};

#endif // ICVMATPROVIDER_H
