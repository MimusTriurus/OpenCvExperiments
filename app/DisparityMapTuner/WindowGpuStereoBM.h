#ifndef MAINWINDOWGPU_H
#define MAINWINDOWGPU_H

#include "BaseDisparityMapTuner.h"

class WindowGpuStereoBM : public BaseDisparityMapTuner {
    Q_OBJECT
public:
    explicit WindowGpuStereoBM( QWidget *parent = nullptr );
    ~WindowGpuStereoBM( );
protected:
    void initInterface( ) override;
    void setBaseValues( ) override;
    void subscribe( ) override;
    void compute( ) override;
    void closeEvent( QCloseEvent * ) override;
private:
    QLabel      _lblBlockSize               { "BlockSize" };
    QLabel      _lblNumDisparities          { "NumDisparities" };
    QLabel      _lblBilateralFilterRadius   { "BilateralFilterRadius:" };
    QLabel      _lblBilateralFilterIteration{ "BilateralFilter Iteration" };

    QSlider _sldrBlockSize                  { Qt::Horizontal };
    QSlider _sldrNumDisparities             { Qt::Horizontal };
    QSlider _sldrBilateralFilterRadius      { Qt::Horizontal };
    QSlider _sldrBilateralFilterIteration   { Qt::Horizontal };

    Ptr<cv::cuda::StereoBM> _bm;
private slots:
    void onChangeBlockSize              ( int i );
    void onChangeNumDisparities         ( int i );
    void onChangeBilateralFilterRadius  ( int i );
    void onChangeBilateralFilterIterat  ( int i );
};

#endif // MAINWINDOW_H
