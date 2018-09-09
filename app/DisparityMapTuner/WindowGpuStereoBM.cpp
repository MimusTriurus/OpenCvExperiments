#include "WindowGpuStereoBM.h"
#include <QDebug>
#include <QFileDialog>
#include "StereoCamCvMatProvider.h"
#include "StereoVideoCvMatProvider.h"

WindowGpuStereoBM::WindowGpuStereoBM( QWidget *parent ) :
    BaseDisparityMapTuner( parent )
{
    initInterface( );

    subscribe( );

    setBaseValues( );
}

WindowGpuStereoBM::~WindowGpuStereoBM( ) {

}

void WindowGpuStereoBM::initInterface( ) {
    DEPTH_IMG_WIN_NAME = "GPU_StereoBM_Depth_Map";
    ORIGIN_IMG_WIN_NAME = "GPU_StereoBM_original";
    this->setWindowTitle( DEPTH_IMG_WIN_NAME );
    this->setLayout( &_layout );

    _layout.addWidget( &_lblBlockSize                , 1, 0 );
    _layout.addWidget( &_lblNumDisparities           , 2, 0 );
    _layout.addWidget( &_lblBilateralFilterRadius    , 3, 0 );
    _layout.addWidget( &_lblBilateralFilterIteration , 4, 0 );

    _layout.addWidget( &_sldrBlockSize               , 1, 1 );
    _layout.addWidget( &_sldrNumDisparities          , 2, 1 );
    _layout.addWidget( &_sldrBilateralFilterRadius   , 3, 1 );
    _layout.addWidget( &_sldrBilateralFilterIteration, 4, 1 );

    _layout.addWidget( &_btnOpen                     , 5, 0, 1, 2 );
    _layout.addWidget( &_btnOpenCam                  , 6, 0, 1, 2 );
    _layout.addWidget( &_btnOpenVideo                , 7, 0, 1, 2 );
}

void WindowGpuStereoBM::setBaseValues( ) {
    _bm = cv::cuda::createStereoBM( 88 );

    _sldrBlockSize.setRange                 ( 5, 255 );
    _sldrNumDisparities.setRange            ( 8, 256 );
    _sldrBilateralFilterRadius.setRange     ( 1, 255 );
    _sldrBilateralFilterIteration.setRange  ( 1, 255 );

    _sldrBlockSize.setValue                 ( 5 );
    _sldrNumDisparities.setValue            ( 72 );
    _sldrBilateralFilterRadius.setValue     ( 3 );
    _sldrBilateralFilterIteration.setValue  ( 1 );

    onChangeBlockSize               ( _sldrBlockSize.value( ) );
    onChangeNumDisparities          ( _sldrNumDisparities.value( ) );
    onChangeBilateralFilterRadius   ( _sldrBilateralFilterRadius.value( ) );
    onChangeBilateralFilterIterat   ( _sldrBilateralFilterIteration.value( ) );
}

void WindowGpuStereoBM::subscribe( ) {
    BaseDisparityMapTuner::subscribe( );
    connect( &_sldrBlockSize                , SIGNAL( valueChanged( int ) ),
             SLOT( onChangeBlockSize            ( int ) ) );
    connect( &_sldrNumDisparities           , SIGNAL( valueChanged( int ) ),
             SLOT( onChangeNumDisparities       ( int ) ) );
    connect( &_sldrBilateralFilterRadius    , SIGNAL( valueChanged( int ) ),
             SLOT( onChangeBilateralFilterRadius( int ) ) );
    connect( &_sldrBilateralFilterIteration , SIGNAL( valueChanged( int ) ),
             SLOT( onChangeBilateralFilterIterat( int ) ) );
}

void WindowGpuStereoBM::compute( ) {
    if ( _leftFrame.empty( ) | _rightFrame.empty( ) )
        return;
    cv::cuda::GpuMat d_left, d_right;
    cv::Mat disp( _leftFrame.size( ), CV_8U );
    cv::cuda::GpuMat d_disp( _leftFrame.size( ), CV_8U );

    d_left.upload( _leftFrame );
    d_right.upload( _rightFrame );

    _bm->compute( d_left, d_right, d_disp);
    auto f = cv::cuda::createDisparityBilateralFilter( _sldrNumDisparities.value( ),
                                                       _sldrBilateralFilterRadius.value( ),
                                                       _sldrBilateralFilterIteration.value( ) );
    f->apply( d_disp, d_left, d_disp );
    cv::cuda::drawColorDisp( d_disp, d_disp, _sldrNumDisparities.value( ) );

    d_disp.download( disp );
    cv::imshow( DEPTH_IMG_WIN_NAME.toStdString( ), disp );
}

void WindowGpuStereoBM::closeEvent( QCloseEvent * ) {
    this->deleteLater( );
}

void WindowGpuStereoBM::onChangeBlockSize( int i ) {
    Q_UNUSED( i )
    int v = ( ( _sldrBlockSize.value( ) % 2 ) == 0 ) ? _sldrBlockSize.value( ) + 1: _sldrBlockSize.value( );
    _lblBlockSize.setText( "BlockSize:" + QString::number( v ) );
    _bm->setBlockSize           ( v );
    compute( );
}

void WindowGpuStereoBM::onChangeNumDisparities( int i ) {
    i = i / 8;
    i = i * 8;
    _lblNumDisparities.setText( "NumDisparities:" + QString::number( i ) );
    _bm->setNumDisparities      ( i );
    compute( );
}

void WindowGpuStereoBM::onChangeBilateralFilterRadius( int i ) {
    _lblBilateralFilterRadius.setText( "BilateralFilterRadius:" + QString::number( i ) );
    compute( );
}

void WindowGpuStereoBM::onChangeBilateralFilterIterat( int i ) {
    _lblBilateralFilterIteration.setText( "BilateralFilterIteration:" + QString::number( i ) );
    compute( );
}

