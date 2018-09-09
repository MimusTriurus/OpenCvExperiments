#include "BaseDisparityMapTuner.h"
#include <QDebug>
#include <QFileDialog>
#include "StereoCamCvMatProvider.h"
#include "StereoVideoCvMatProvider.h"

BaseDisparityMapTuner::BaseDisparityMapTuner( QWidget *parent ) :
    QWidget( parent )
{
    connect( &_tmrGetFrames , SIGNAL( timeout( ) ), SLOT( update( ) ) );
    _tmrGetFrames.setInterval( 30 );
}

void BaseDisparityMapTuner::subscribe( ) {
    connect( &_btnOpen     , SIGNAL( clicked( bool ) ), SLOT( onOpenFrame( ) ) );
    connect( &_btnOpenCam  , SIGNAL( clicked( bool ) ), SLOT( onOpenStereoCam( ) ) );
    connect( &_btnOpenVideo, SIGNAL( clicked( bool ) ), SLOT( onOpenStereoVideo( ) ) );
}

BaseDisparityMapTuner::~BaseDisparityMapTuner( ) {
    destroyWindow( DEPTH_IMG_WIN_NAME.toStdString( ) );
    destroyWindow( ORIGIN_IMG_WIN_NAME.toStdString( ) );
    releaseDataProvider( );
}

void BaseDisparityMapTuner::releaseDataProvider( ) {
    _tmrGetFrames.stop( );
    if ( _stereoDataProvider != nullptr ) {
        _stereoDataProvider->stop( );
        delete _stereoDataProvider;
        _stereoDataProvider = nullptr;
    }
}

void BaseDisparityMapTuner::onOpenFrame( ) {
    QString path = QFileDialog::getOpenFileName( 0, "Open Left frame", "", "*.jpg *.png" );
    _leftFrame = cv::imread( path.toStdString( ), 0 );

    path = QFileDialog::getOpenFileName( 0, "Open Right frame", "", "*.jpg *.png" );
    _rightFrame = cv::imread( path.toStdString( ), 0 );

    if ( _rightFrame.empty( ) | _leftFrame.empty( ) )
        return;
    cv::imshow( ORIGIN_IMG_WIN_NAME.toStdString( ), _leftFrame );
    compute( );
}

void BaseDisparityMapTuner::onOpenStereoCam( ) {
    releaseDataProvider( );
    QString path = QFileDialog::getOpenFileName( 0, "Open camera settings xml", "", "*.yaml" );
    if ( path.isEmpty( ) )
        return;
    _stereoDataProvider = new StereoCamCvMatProvider( this );
    _stereoDataProvider->init( path );
    _stereoDataProvider->start( );
    _tmrGetFrames.start( );
}

void BaseDisparityMapTuner::onOpenStereoVideo( ) {
    releaseDataProvider( );
    QString path = QFileDialog::getOpenFileName( 0, "Open video settings xml", "", "*.yaml" );
    if ( path.isEmpty( ) )
        return;
    _stereoDataProvider = new StereoVideoCvMatProvider( this );
    _stereoDataProvider->init( path );
    _stereoDataProvider->start( );
    _tmrGetFrames.start( );
}

void BaseDisparityMapTuner::update( ) {
    bool success = _stereoDataProvider->read( _leftFrame, _rightFrame );
    if ( !success ) {
        qDebug( ) << "return update";
//        _tmrGetFrames.stop( );
        return;
    }

    cv::cvtColor( _leftFrame, _leftFrame, cv::COLOR_BGR2GRAY );
    cv::cvtColor( _rightFrame, _rightFrame, cv::COLOR_BGR2GRAY );

    if ( _rightFrame.empty( ) | _leftFrame.empty( ) )
        return;
    cv::imshow( this->windowTitle( ).toStdString( ), _leftFrame );
    compute( );
}
