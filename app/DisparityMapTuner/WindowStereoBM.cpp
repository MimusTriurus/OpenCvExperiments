#include "WindowStereoBM.h"
#include <QDebug>
#include <QFileDialog>

WindowStereoBM::WindowStereoBM( QWidget *parent ) :
    FILTERED_IMG_WIN{ "FilteredDisparity" },
    BaseDisparityMapTuner( parent )
{
    initInterface( );

    subscribe( );

    setBaseValues( );

    initFilter( );
}

WindowStereoBM::~WindowStereoBM( ) {
    cv::destroyWindow( FILTERED_IMG_WIN.toStdString( ) );
}

void WindowStereoBM::initFilter( ) {
    //ROI = computeROI( Size( 574, 400 ), _bm );
    wls_filter = createDisparityWLSFilterGeneric( false );
    wls_filter->setDepthDiscontinuityRadius( ( int )ceil( 0.33 * _sldrBlockSize.value( ) ) );
}

void WindowStereoBM::initInterface( ) {
    DEPTH_IMG_WIN_NAME = "CPU_StereoBM_Depth_Map";
    ORIGIN_IMG_WIN_NAME = "CPU_StereoBM_original";
    this->setWindowTitle( DEPTH_IMG_WIN_NAME );
    this->setLayout( &_layout );

    _layout.addWidget( &_lblPrefilterCap         , 0, 0 );
    _layout.addWidget( &_lblBlockSize            , 1, 0 );
    _layout.addWidget( &_lblMinDisparity         , 2, 0 );
    _layout.addWidget( &_lblNumDisparities       , 3, 0 );
    _layout.addWidget( &_lblTextureThreshold     , 4, 0 );
    _layout.addWidget( &_lblUniquenessRatio      , 5, 0 );
    _layout.addWidget( &_lblSpeckleWindowSize    , 6, 0 );
    _layout.addWidget( &_lblSpeckleRange         , 7, 0 );
    _layout.addWidget( &_lblDisp12MaxDiff        , 8, 0 );

    _layout.addWidget( &_sldrPrefilterCap        , 0, 1 );
    _layout.addWidget( &_sldrBlockSize           , 1, 1 );
    _layout.addWidget( &_sldrMinDisparity        , 2, 1 );
    _layout.addWidget( &_sldrNumDisparities      , 3, 1 );
    _layout.addWidget( &_sldrTextureThreshold    , 4, 1 );
    _layout.addWidget( &_sldrUniquenessRatio     , 5, 1 );
    _layout.addWidget( &_sldrSpeckleWindowSize   , 6, 1 );
    _layout.addWidget( &_sldrSpeckleRange        , 7, 1 );
    _layout.addWidget( &_sldrDisp12MaxDiff       , 8, 1 );

    _layout.addWidget( &_btnOpen                 , 9, 0, 1, 2 );
    _layout.addWidget( &_btnOpenCam              , 10, 0, 1, 2 );
    _layout.addWidget( &_btnOpenVideo            , 11, 0, 1, 2 );
}

void WindowStereoBM::setBaseValues( ) {
    _bm = cv::StereoBM::create( 16, 9 );

    _sldrPrefilterCap.setRange      ( 1, 63 );
    _sldrBlockSize.setRange         ( 5, 255 );
    _sldrMinDisparity.setRange      ( -300, 100 );
    _sldrNumDisparities.setRange    ( 16, 256 );
    _sldrTextureThreshold.setRange  ( 0, 1000 );
    _sldrUniquenessRatio.setRange   ( 0, 30 );
    _sldrSpeckleWindowSize.setRange ( 0, 250 );
    _sldrSpeckleRange.setRange      ( 0, 40 );
    _sldrDisp12MaxDiff.setRange     ( -1, 100 );

    _sldrPrefilterCap.setValue      ( 63 );
    _sldrBlockSize.setValue         ( 9 );
    _sldrMinDisparity.setValue      ( -21 );
    _sldrNumDisparities.setValue    ( 176 );
    _sldrTextureThreshold.setValue  ( 1000 );
    _sldrUniquenessRatio.setValue   ( 2 );
    _sldrSpeckleWindowSize.setValue ( 116 );
    _sldrSpeckleRange.setValue      ( 40 );
    _sldrDisp12MaxDiff.setValue     ( 100 );

    onChangePrefilterCap       ( _sldrPrefilterCap.value( ) );
    onChangeBlockSize          ( _sldrBlockSize.value( ) );
    onChangeMinDisparity       ( _sldrMinDisparity.value( ) );
    onChangeNumDisparities     ( _sldrNumDisparities.value( ) );
    onChangeTextureThreshold   ( _sldrTextureThreshold.value( ) );
    onChangeUniquenessRatio    ( _sldrUniquenessRatio.value( ) );
    onChangeSpeckleWindowSize  ( _sldrSpeckleWindowSize.value( ) );
    onChangeSpeckleRange       ( _sldrSpeckleRange.value( ) );
    onChangeDisp12MaxDiff      ( _sldrDisp12MaxDiff.value( ) );
}

void WindowStereoBM::subscribe( ) {
    BaseDisparityMapTuner::subscribe( );
    connect( &_sldrPrefilterCap     , SIGNAL( valueChanged( int ) ), SLOT( onChangePrefilterCap     ( int ) ) );
    connect( &_sldrBlockSize        , SIGNAL( valueChanged( int ) ), SLOT( onChangeBlockSize        ( int ) ) );
    connect( &_sldrMinDisparity     , SIGNAL( valueChanged( int ) ), SLOT( onChangeMinDisparity     ( int ) ) );
    connect( &_sldrNumDisparities   , SIGNAL( valueChanged( int ) ), SLOT( onChangeNumDisparities   ( int ) ) );
    connect( &_sldrTextureThreshold , SIGNAL( valueChanged( int ) ), SLOT( onChangeTextureThreshold ( int ) ) );
    connect( &_sldrUniquenessRatio  , SIGNAL( valueChanged( int ) ), SLOT( onChangeUniquenessRatio  ( int ) ) );
    connect( &_sldrSpeckleWindowSize, SIGNAL( valueChanged( int ) ), SLOT( onChangeSpeckleWindowSize( int ) ) );
    connect( &_sldrSpeckleRange     , SIGNAL( valueChanged( int ) ), SLOT( onChangeSpeckleRange     ( int ) ) );
    connect( &_sldrDisp12MaxDiff    , SIGNAL( valueChanged( int ) ), SLOT( onChangeDisp12MaxDiff    ( int ) ) );
}

void WindowStereoBM::compute( ) {
    if ( _leftFrame.empty( ) | _rightFrame.empty( ) ) {
        return;
    }
    cv::Mat _map;
    cv::Mat _map8;
    _bm->compute( _leftFrame, _rightFrame, _map );
    _map.convertTo( _map8, CV_8U, 255 / ( _sldrNumDisparities.value( ) * 16. ) );
    cv::applyColorMap( _map8, _map8, cv::COLORMAP_JET );

    imshow( DEPTH_IMG_WIN_NAME.toStdString( ), _map8 );

    wls_filter->setLambda( 4000 );
    wls_filter->setSigmaColor( 1.5 );
    Mat filtered_disp;
    ROI = computeROI( _leftFrame.size( ), _bm );
    wls_filter->filter( _map, _leftFrame, filtered_disp, Mat( ), ROI );
    Mat filtered_disp_vis;
    getDisparityVis( filtered_disp, filtered_disp_vis, 1.0 );
    cv::applyColorMap( filtered_disp_vis, filtered_disp_vis, cv::COLORMAP_JET );
    imshow( FILTERED_IMG_WIN.toStdString( ), filtered_disp_vis );
}

void WindowStereoBM::closeEvent( QCloseEvent * ) {
    this->deleteLater( );
}

Rect WindowStereoBM::computeROI( Size2i srcSz, Ptr<StereoMatcher> matcherInstance ) {
    int min_disparity = matcherInstance->getMinDisparity( );
    int num_disparities = matcherInstance->getNumDisparities( );
    int block_size = matcherInstance->getBlockSize( );

    int bs2 = block_size / 2;
    int minD = min_disparity, maxD = min_disparity + num_disparities - 1;

    int xmin = maxD + bs2;
    int xmax = srcSz.width + minD - bs2;
    int ymin = bs2;
    int ymax = srcSz.height - bs2;

    Rect r(xmin, ymin, xmax - xmin, ymax - ymin);
    return r;
}

void WindowStereoBM::onChangePrefilterCap( int i ) {
    _lblPrefilterCap.setText( "PrefilterCap:" + QString::number( i ) );
    _bm->setPreFilterCap        ( _sldrPrefilterCap.value( ) );
    compute( );
}

void WindowStereoBM::onChangeBlockSize( int i ) {
    Q_UNUSED( i )
    int v = ( ( _sldrBlockSize.value( ) % 2 ) == 0 ) ? _sldrBlockSize.value( ) + 1: _sldrBlockSize.value( );
    _lblBlockSize.setText( "BlockSize:" + QString::number( v ) );
    _bm->setBlockSize           ( v );
    compute( );
}

void WindowStereoBM::onChangeMinDisparity( int i ) {
    _lblMinDisparity.setText( "MinDisparity:" + QString::number( i ) );
    _bm->setMinDisparity        ( _sldrMinDisparity.value( ) );
    compute( );
}

void WindowStereoBM::onChangeNumDisparities( int i ) {
    i = i / 16;
    i = i * 16;
    _lblNumDisparities.setText( "NumDisparities:" + QString::number( i ) );
    _bm->setNumDisparities      ( i );
    compute( );
}

void WindowStereoBM::onChangeTextureThreshold( int i ) {
    _lblTextureThreshold.setText( "TextureThreshold:" + QString::number( i ) );
    _bm->setTextureThreshold    ( _sldrTextureThreshold.value( ) );
    compute( );
}

void WindowStereoBM::onChangeUniquenessRatio( int i ) {
    _lblUniquenessRatio.setText( "UniquenessRatio:" + QString::number( i ) );
    _bm->setUniquenessRatio     ( _sldrUniquenessRatio.value( ) );
    compute( );
}

void WindowStereoBM::onChangeSpeckleWindowSize( int i ) {
    _lblSpeckleWindowSize.setText( "SpeckleWindowSize:" + QString::number( i ) );
    _bm->setSpeckleWindowSize   ( _sldrSpeckleWindowSize.value( ) );
    compute( );
}

void WindowStereoBM::onChangeSpeckleRange( int i ) {
    _lblSpeckleRange.setText( "SpeckleRange:" + QString::number( i ) );
    _bm->setSpeckleRange        ( _sldrSpeckleRange.value( ) );
    compute( );
}

void WindowStereoBM::onChangeDisp12MaxDiff( int i ) {
    _lblDisp12MaxDiff.setText( "Disp12MaxDiff:" + QString::number( i ) );
    _bm->setDisp12MaxDiff       ( _sldrDisp12MaxDiff.value( ) );
    compute( );
}

