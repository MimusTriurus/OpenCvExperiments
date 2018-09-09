#ifndef VIDEORECIEVER_H
#define VIDEORECIEVER_H

#include <QtCore>
#include <QUdpSocket>
#include <QThread>
#include <QMutex>

/**
 * @brief Приемник видеопотока
 */
class VideoReciever : public QThread {
    Q_OBJECT
public:
    explicit VideoReciever( QObject *parent = nullptr );
    ~VideoReciever( );

    void listen( const quint16 port, const double interval = 5 );
    void stopListen( );

    const QByteArray &imgData( ) const;
signals:
    void imgDataReceived( QByteArray );
protected:
    void run( ) override;
private:
    int         _imgSize;
    QByteArray  _imgData;
    QByteArray  _imgBuffer;
    quint16     _port;
    double      _interval;
    bool        _workInProgress{ false };

    mutable QMutex _mutex;
};

#endif // VIDEORECIEVER_H
