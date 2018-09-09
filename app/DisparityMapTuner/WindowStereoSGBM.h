#ifndef WINDOSSTEREOSGBM_H
#define WINDOSSTEREOSGBM_H

#include "BaseDisparityMapTuner.h"

class WindowStereoSGBM : public BaseDisparityMapTuner {
    Q_OBJECT
public:
    explicit WindowStereoSGBM( QWidget *parent = nullptr );
protected:
    void initInterface( ) override;
    void setBaseValues( ) override;
    void subscribe( ) override;
    void compute( ) override;
    void closeEvent( QCloseEvent *) override;
private:
    QLabel      _lblPrefilterCap     { "PrefilterCap" };
    QLabel      _lblBlockSize        { "BlockSize" };
    QLabel      _lblMinDisparity     { "MinDisparity" };
    QLabel      _lblNumDisparities   { "NumDisparities" };
    QLabel      _lblUniquenessRatio  { "UniquenessRatio" };
    QLabel      _lblSpeckleWindowSize{ "SpeckleWindowSize" };
    QLabel      _lblSpeckleRange     { "SpeckleRange" };
    QLabel      _lblDisp12MaxDiff    { "Disp12MaxDiff" };

    QSlider _sldrPrefilterCap       { Qt::Horizontal };
    QSlider _sldrBlockSize          { Qt::Horizontal };
    QSlider _sldrMinDisparity       { Qt::Horizontal };
    QSlider _sldrNumDisparities     { Qt::Horizontal };
    QSlider _sldrUniquenessRatio    { Qt::Horizontal };
    QSlider _sldrSpeckleWindowSize  { Qt::Horizontal };
    QSlider _sldrSpeckleRange       { Qt::Horizontal };
    QSlider _sldrDisp12MaxDiff      { Qt::Horizontal };

    cv::Ptr<cv::StereoSGBM> _sgbm;
private slots:
    void onChangePrefilterCap       ( int i );
    void onChangeBlockSize          ( int i );
    void onChangeMinDisparity       ( int i );
    void onChangeNumDisparities     ( int i );
    void onChangeUniquenessRatio    ( int i );
    void onChangeSpeckleWindowSize  ( int i );
    void onChangeSpeckleRange       ( int i );
    void onChangeDisp12MaxDiff      ( int i );
};

#endif // WINDOSSTEREOSGBM_H
