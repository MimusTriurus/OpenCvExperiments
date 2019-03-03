#ifndef BASEDISPARITYMAPTUNER_H
#define BASEDISPARITYMAPTUNER_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTimer>
#include "memory"

#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>

#include <stdio.h>
#include <iostream>

#include "ICvMatProvider.h"

using namespace std;
using namespace cv;
using namespace cv::ximgproc;

class BaseDisparityMapTuner : public QWidget {
    Q_OBJECT
public:
    virtual ~BaseDisparityMapTuner( );
protected:
    explicit BaseDisparityMapTuner( QWidget *parent = nullptr );

    QGridLayout _layout;

    QPushButton _btnOpen             { "Open pair frame" };
    QPushButton _btnOpenCam          { "Open stereo camera" };
    QPushButton _btnOpenVideo        { "Open stereo video" };
////////////////////////////////////////////////////////////////
    Mat _leftFrame, _rightFrame;

    QTimer _tmrGetFrames;
    ICvMatProvider *_stereoDataProvider;

    QString _leftVideoPath;
    QString _rightVideoPath;

    QString DEPTH_IMG_WIN_NAME;
    QString ORIGIN_IMG_WIN_NAME;

    virtual void initInterface( ) = 0;
    virtual void setBaseValues( ) = 0;
    virtual void subscribe( );
    virtual void compute( ) = 0;
    void releaseDataProvider( );
protected slots:
    void onOpenFrame( );
    void onOpenStereoCam( );
    void onOpenStereoVideo( );

    void update( );
};

#endif // BASEDISPARITYMAPTUNER_H
