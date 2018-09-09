#ifndef UDPCVMATPROVIDER_H
#define UDPCVMATPROVIDER_H

#include <QObject>
#include <VideoReceiver.h>
#include <opencv2/opencv.hpp>

#include "ICvMatProvider.h"

class UdpCvMatProvider : public QObject, public ICvMatProvider {
    Q_OBJECT
public:
    explicit UdpCvMatProvider( QObject *parent = nullptr );

    bool init( const QString &configPath ) override;
    void start( )                          override;
    void stop( )                           override;
    bool read( cv::Mat &frame )            override;
private:
    VideoReciever   _receiver;
private slots:
    void onReceiveData( QByteArray data );
};

#endif // UDPCVMATPROVIDER_H
