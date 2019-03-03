#ifndef STEREOCAMCVMATPROVIDER_H
#define STEREOCAMCVMATPROVIDER_H

#include <QObject>
#include <opencv2/opencv.hpp>

#include "ICvMatProvider.h"

class StereoCamCvMatProvider : public QObject, public ICvMatProvider {
    Q_OBJECT
public:
    explicit StereoCamCvMatProvider( QObject *parent = nullptr );

    bool init( const QString &configPath )     override;
    void start( )                              override;
private:
    int _leftCamIndx;
    int _rightCamIndx;
};

#endif // STEREOCAMCVMATPROVIDER_H
