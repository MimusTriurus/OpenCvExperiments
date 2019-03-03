#ifndef CAMCVMATPROVIDER_H
#define CAMCVMATPROVIDER_H

#include <QObject>
#include <opencv2/opencv.hpp>

#include "ICvMatProvider.h"

class CamCvMatProvider : public QObject, public ICvMatProvider {
    Q_OBJECT
public:
    explicit CamCvMatProvider( QObject *parent = nullptr );

    bool    init( const QString &configPath )   override;
    void    start( )                            override;
private:
    int _cameraIndex;
};

#endif // CAMCVMATPROVIDER_H
