#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BaseDisparityMapTuner.h"

class WindowStereoBM : public BaseDisparityMapTuner {
    Q_OBJECT
public:
    explicit WindowStereoBM( QWidget *parent = nullptr );
    ~WindowStereoBM( );
protected:
    void initInterface( ) override;
    void setBaseValues( ) override;
    void subscribe( ) override;
    void compute( ) override;
    void closeEvent( QCloseEvent * ) override;
private:
    QLabel      _lblPrefilterCap     { "PrefilterCap" };
    QLabel      _lblBlockSize        { "BlockSize" };
    QLabel      _lblMinDisparity     { "MinDisparity" };
    QLabel      _lblNumDisparities   { "NumDisparities" };
    QLabel      _lblTextureThreshold { "TextureThreshold" };
    QLabel      _lblUniquenessRatio  { "UniquenessRatio" };
    QLabel      _lblSpeckleWindowSize{ "SpeckleWindowSize" };
    QLabel      _lblSpeckleRange     { "SpeckleRange" };
    QLabel      _lblDisp12MaxDiff    { "Disp12MaxDiff" };

    QSlider _sldrPrefilterCap       { Qt::Horizontal };
    QSlider _sldrBlockSize          { Qt::Horizontal };
    QSlider _sldrMinDisparity       { Qt::Horizontal };
    QSlider _sldrNumDisparities     { Qt::Horizontal };
    QSlider _sldrTextureThreshold   { Qt::Horizontal };
    QSlider _sldrUniquenessRatio    { Qt::Horizontal };
    QSlider _sldrSpeckleWindowSize  { Qt::Horizontal };
    QSlider _sldrSpeckleRange       { Qt::Horizontal };
    QSlider _sldrDisp12MaxDiff      { Qt::Horizontal };

    cv::Ptr<cv::StereoBM> _bm;

    // FILTERING
    const QString FILTERED_IMG_WIN;
    cv::Rect ROI;
    cv::Ptr<DisparityWLSFilter> wls_filter;
    void initFilter( );
    Rect computeROI( Size2i srcSz, Ptr<StereoMatcher> matcherInstance );
private slots:
    void onChangePrefilterCap       ( int i );
    void onChangeBlockSize          ( int i );
    void onChangeMinDisparity       ( int i );
    void onChangeNumDisparities     ( int i );
    void onChangeTextureThreshold   ( int i );
    void onChangeUniquenessRatio    ( int i );
    void onChangeSpeckleWindowSize  ( int i );
    void onChangeSpeckleRange       ( int i );
    void onChangeDisp12MaxDiff      ( int i );
};

#endif // MAINWINDOW_H
