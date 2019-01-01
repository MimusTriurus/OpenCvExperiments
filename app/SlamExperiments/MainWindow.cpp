#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QFileDialog>

#include "StereoCamCvMatProvider.h"
#include "StereoVideoCvMatProvider.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    _orb = cuda::ORB::create( 1500, 1.2f, 8, 31, 0, 2, 0, 31, 20, true );
    _matcher = cv::cuda::DescriptorMatcher::createBFMatcher( cv::NORM_HAMMING );

    connect( &_tmrGetFrames , SIGNAL( timeout( ) ), SLOT( update( ) ) );
    _tmrGetFrames.setInterval( 30 );
}

MainWindow::~MainWindow( ) {
    delete ui;
}

void MainWindow::compute( ) {
    if ( _leftFrame.empty( ) && _rightFrame.empty( ) )
        return;

    GpuMat imgL, imgR;
    imgL.upload( _leftFrame );
    imgR.upload( _rightFrame );

    vector<KeyPoint> keypointsL;
    GpuMat descriptorsL;

    vector<KeyPoint> keypointsR;
    GpuMat descriptorsR;

    _orb->detectAndCompute( imgL, noArray( ), keypointsL, descriptorsL );
    _orb->detectAndCompute( imgR, noArray( ), keypointsR, descriptorsR );

    vector< vector<DMatch> >  matchesArr;

    _matcher->radiusMatch( descriptorsL, descriptorsR, matchesArr, 15.f );

    Mat result;
    std::vector<std::vector<char>> mask;
    drawMatches( _leftFrame, keypointsL, _rightFrame, keypointsR, matchesArr, result,
                 Scalar::all( -1 ), Scalar::all( -1 ),
                 mask,
                 cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    int count = 0;
    for ( auto it = matchesArr.begin( ); it < matchesArr.end( ); ++it ) {
        count += static_cast<int>( it->size( ) );
    }
    QString matchesCount{ "Matches:" + QString::number( count ) };
    if ( !result.empty( ) ) {
        putText( result, matchesCount.toStdString( ).c_str( ), Point( 5, 25 ),
                 FONT_HERSHEY_DUPLEX, 1, Scalar( 0, 255, 0 ), 1 );
        imshow( "result", result );
    }
}

void MainWindow::on_btnOpenPairFrame_clicked( ) {
    _tmrGetFrames.stop( );
    QString path = QFileDialog::getOpenFileName( 0, "Open Left frame", "", "*.jpg *.png" );
    _leftFrame = cv::imread( path.toStdString( ), 0 );

    path = QFileDialog::getOpenFileName( 0, "Open Right frame", "", "*.jpg *.png" );
    _rightFrame = cv::imread( path.toStdString( ), 0 );

    if ( _rightFrame.empty( ) | _leftFrame.empty( ) )
        return;
    _orb->setMaxFeatures( ui->sbMaxFeatures->value( ) );
    compute( );
}

void MainWindow::on_btnOpenStereoVideo_clicked( ) {
    _tmrGetFrames.stop( );
    _dataProvider.release( );
    QString path = QFileDialog::getOpenFileName( 0, "Open video settings xml", "", "*.yaml" );
    if ( path.isEmpty( ) )
        return;
    _dataProvider = std::unique_ptr<StereoVideoCvMatProvider>( new StereoVideoCvMatProvider( this ) );
    _dataProvider->init( path );
    _dataProvider->start( );
    _orb->setMaxFeatures( ui->sbMaxFeatures->value( ) );
    _tmrGetFrames.start( );
}

void MainWindow::on_btnOpenStereoCam_clicked( ) {
    _tmrGetFrames.stop( );
    _dataProvider.release( );
    QString path = QFileDialog::getOpenFileName( 0, "Open video settings xml", "", "*.yaml" );
    if ( path.isEmpty( ) )
        return;
    _dataProvider = std::unique_ptr<StereoVideoCvMatProvider>( new StereoVideoCvMatProvider( this ) );
    _dataProvider->init( path );
    _dataProvider->start( );
    _orb->setMaxFeatures( ui->sbMaxFeatures->value( ) );
    _tmrGetFrames.start( );
}

void MainWindow::update( ) {
    bool success = _dataProvider->read( _leftFrame, _rightFrame );
    if ( !success ) {
        qDebug( ) << "return update";
        return;
    }

    cv::cvtColor( _leftFrame, _leftFrame, cv::COLOR_BGR2GRAY );
    cv::cvtColor( _rightFrame, _rightFrame, cv::COLOR_BGR2GRAY );

    if ( _rightFrame.empty( ) | _leftFrame.empty( ) )
        return;
    compute( );
}
