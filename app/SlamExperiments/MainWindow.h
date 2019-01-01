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

    Ptr<cv::cuda::ORB> _orb;
    Ptr<cv::cuda::DescriptorMatcher> _matcher;

    void compute( );
private slots:
    void on_btnOpenPairFrame_clicked( );
    void on_btnOpenStereoVideo_clicked( );
    void on_btnOpenStereoCam_clicked( );

    void update( );
};

#endif // MAINWINDOW_H
