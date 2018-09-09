#ifndef STEREOCAMCALIBRATOR_H
#define STEREOCAMCALIBRATOR_H

#include <QObject>
#include "opencv2/opencv.hpp"

class StereoCamCalibrator : public QObject {
    Q_OBJECT
public:
    StereoCamCalibrator( QObject *parent = nullptr );
    void init( cv::Size frameSize = cv::Size{ 640, 480 },
               const QString &dirPath = "" );
    void calibrate( cv::Mat &left, cv::Mat &right );
private:
    const QString EXTRINSICS_FILE;
    cv::Rect    _targetRect;
    cv::Mat     _lmapx, _lmapy;
    cv::Mat     _rmapx, _rmapy;
    bool        _init;
};

#endif // STEREOCAMCALIBRATOR_H
