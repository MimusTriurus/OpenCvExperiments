#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>

#include <stdio.h>
#include <iostream>

#include "ICvMatProvider.h"

using namespace cv;
using namespace cuda;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow( );
private:
    Ui::MainWindow *ui;
    QTimer _tmrGetFrames;
    std::unique_ptr<ICvMatProvider> _dataProvider;

    cv::Mat _leftFrame, _rightFrame;
    // опорные точки
    Ptr<cv::cuda::ORB> _orb;
    Ptr<cv::cuda::DescriptorMatcher> _matcher;
    // оптический поток
    Ptr<cv::cuda::SparsePyrLKOpticalFlow> _lkOpticalFlow;
    Ptr< cuda::CornersDetector> _cornersDetector;
    GpuMat _prevImgL, _prevImgR;

    void compute( );

    void printPoints( Mat points );

    void detectAndMatchKeypoint( );
    void calcOpticalFlow( );

    vector<Point2f> _goodKeysL, _goodKeysR;

    static void download( const cuda::GpuMat& d_mat, vector< Point2f>& vec );
    static void download( const cuda::GpuMat& d_mat, vector< uchar>& vec );

    static void download( const vector<Point2f> &inputVec, GpuMat &outputMat );

    static void drawArrows( Mat& frame, const vector< Point2f>& prevPts
                            , const vector< Point2f>& nextPts
                            , const vector< uchar>& status
                            , Scalar line_color = Scalar( 0, 0, 255 ) );
private slots:
    void on_btnOpenPairFrame_clicked( );
    void on_btnOpenStereoVideo_clicked( );
    void on_btnOpenStereoCam_clicked( );

    void update( );
};

#endif // MAINWINDOW_H
