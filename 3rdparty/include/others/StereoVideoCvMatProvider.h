#ifndef STEREOVIDEOCVMATPROVIDER_H
#define STEREOVIDEOCVMATPROVIDER_H

#include <QObject>
#include <opencv2/opencv.hpp>

#include "ICvMatProvider.h"

class StereoVideoCvMatProvider : public QObject, public ICvMatProvider {
    Q_OBJECT
public:
    explicit StereoVideoCvMatProvider( QObject *parent = nullptr );

    bool init( const QString &configPath )      override;
    void start( )                               override;
private:
    cv::String _leftVideoPath;
    cv::String _rightVideoPath;
};

#endif // STEREOVIDEOCVMATPROVIDER_H
