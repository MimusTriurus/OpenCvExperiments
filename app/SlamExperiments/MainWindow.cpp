#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QFileDialog>

#include "StereoCamCvMatProvider.h"
#include "StereoVideoCvMatProvider.h"

#include "opencv2/cudaoptflow.hpp"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    _orb = cuda::ORB::create( 1500, 1.2f, 8, 31, 0, 2, 0, 31, 20, true );
    _matcher = cv::cuda::DescriptorMatcher::createBFMatcher( cv::NORM_HAMMING );

    _lkOpticalFlow = cv::cuda::SparsePyrLKOpticalFlow::create( );
    _cornersDetector = cv::cuda::createGoodFeaturesToTrackDetector( CV_8UC1 );

    connect( &_tmrGetFrames , SIGNAL( timeout( ) ), SLOT( update( ) ) );
    _tmrGetFrames.setInterval( 30 );
}

MainWindow::~MainWindow( ) {
    delete ui;
}

void MainWindow::compute( ) {
    if ( _leftFrame.empty( ) && _rightFrame.empty( ) )
        return;

    detectAndMatchKeypoint( );
}

void MainWindow::printPoints( cv::Mat points ) {
    vector<float> array;
    if ( points.isContinuous( ) ) {
      array.assign( ( float* )points.datastart, ( float* )points.dataend );
    }
    vector<Vec3f> pnts;
    for ( auto it = array.cbegin( ); it < array.cend( ); it += 4 ) {
        auto w = *( it + 3 );
        auto x = *( it ) / w;
        auto y = *( it + 1 ) / w;
        auto z = *( it + 2 ) / w;
        cout << "x:" << x << " y:" << y << " z:" << z /*<< " w:" << w*/ << endl;
    }
    cout << "******************" << endl;
}

void MainWindow::detectAndMatchKeypoint( ) {
    GpuMat imgL, imgR;
    imgL.upload( _leftFrame );
    imgR.upload( _rightFrame );

    vector<KeyPoint> keypointsL, keypointsR;
    GpuMat descriptorsL, descriptorsR;
    // ищем опорные точки для левого и правого изображения и вычисляем дескрипторы
    _orb->detectAndCompute( imgL, noArray( ), keypointsL, descriptorsL );
    _orb->detectAndCompute( imgR, noArray( ), keypointsR, descriptorsR );

    vector< vector<DMatch> >  matchesArr;
    if ( descriptorsL.empty( ) || descriptorsR.empty( ) )
        return;
    // сопоставляем дескрипторы опорных точек левого и правого изображения
    // находим соответствия
    _matcher->radiusMatch( descriptorsL, descriptorsR, matchesArr, ui->cbFeatureRadius->value( ) );

    if ( matchesArr.empty( ) )
        return;

    Mat result;
    std::vector<std::vector<char>> mask;
    drawMatches( _leftFrame, keypointsL, _rightFrame, keypointsR, matchesArr, result,
                 Scalar::all( -1 ), Scalar::all( -1 ),
                 mask,
                 cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS /*| cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS*/ );
    int count = 0;
    //vector<Point2f> goodKeysL, goodKeysR;

    calcOpticalFlow( );

    _goodKeysL.clear( );
    _goodKeysR.clear( );
    for ( auto itI = matchesArr.cbegin( ); itI < matchesArr.cend( ); ++itI ) {
        count += static_cast<int>( itI->size( ) );
        for ( auto itJ = itI->cbegin( ); itJ < itI->cend( ); ++itJ ) {
            auto l = keypointsL[ itJ->queryIdx ].pt;
            auto r = keypointsR[ itJ->trainIdx ].pt;
            _goodKeysL.push_back( l );
            _goodKeysR.push_back( r );
        }
    }
    if ( _goodKeysL.empty( ) || _goodKeysR.empty( ) )
        return;
    cv::Mat pnts3D( 1, static_cast<int>( _goodKeysL.size( ) ), CV_64FC4 );
    cv::triangulatePoints( _dataProvider->projectionMatrix1( ), _dataProvider->projectionMatrix2( ), _goodKeysL, _goodKeysR, pnts3D );

//    printPoints( pnts3D );

    QString matchesCount{ "Matches:" + QString::number( count ) };
    if ( !result.empty( ) ) {
        putText( result, matchesCount.toStdString( ).c_str( ), Point( 5, 25 ),
                 FONT_HERSHEY_DUPLEX, 1, Scalar( 0, 255, 0 ), 1 );
        imshow( "result", result );
    }
}

void MainWindow::calcOpticalFlow( ) {
    GpuMat imgL, imgR;
    imgL.upload( _leftFrame );
    imgR.upload( _rightFrame );
    if ( !_prevImgL.empty( ) && !_prevImgR.empty( ) && !_goodKeysL.empty( ) ) {
        cuda::GpuMat d_prevPts;
        cuda::GpuMat d_nextPts;
        cuda::GpuMat d_status;

        download( _goodKeysL, d_prevPts );
        _lkOpticalFlow->calc( _prevImgL, imgL, d_prevPts, d_nextPts, d_status );

        vector<Point2f> prevPts( d_prevPts.cols );
        download( d_prevPts, prevPts );

        vector<Point2f> nextPts(d_nextPts.cols);
        download( d_nextPts, nextPts );

        vector<uchar> status( d_status.cols );
        download( d_status, status );

        drawArrows( _leftFrame, prevPts, nextPts, status, Scalar( 255, 0, 0 ) );

        imshow("PyrLK [Sparse]", _leftFrame );
    }
    _prevImgL = imgL;
    _prevImgR = imgR;
}

void MainWindow::download( const GpuMat &d_mat, vector<Point2f> &vec ) {
    vec.resize( d_mat.cols );
    Mat mat( 1, d_mat.cols, CV_32FC2, ( void* )&vec[ 0 ] );
    d_mat.download ( mat );
}

void MainWindow::download( const GpuMat &d_mat, vector<uchar> &vec ) {
    vec.resize( d_mat.cols );
    Mat mat( 1, d_mat.cols, CV_8UC1, ( void* )&vec[ 0 ] );
    d_mat.download( mat );
}

void MainWindow::download( const vector<Point2f> &inputVec, GpuMat &outputMat ) {
    Mat mat( 1, static_cast<int>( inputVec.size( ) ), CV_32FC2, ( void* )&inputVec[ 0 ] );
    outputMat.upload( mat );
}

void MainWindow::drawArrows( Mat &frame, const vector<Point2f> &prevPts, const vector<Point2f> &nextPts, const vector<uchar> &status, Scalar line_color ) {
    for ( size_t i = 0; i < prevPts.size( ); ++i ) {
        if ( status[ i ] ) {
            int line_thickness = 1;

            Point p = prevPts[ i ];
            Point q = nextPts[ i ];

            double angle = atan2( ( double )p.y - q.y, ( double )p.x - q.x );

            double hypotenuse = sqrt( ( double )( p.y - q.y )*( p.y - q.y ) + ( double )( p.x - q.x )*( p.x - q.x ) );

            if ( hypotenuse < 1.0 )
            continue;

            // Here we lengthen the arrow by a factor of three.
            q.x = ( int )( p.x - 3 * hypotenuse * cos( angle ) );
            q.y = ( int )( p.y - 3 * hypotenuse * sin( angle ) );

            // Now we draw the main line of the arrow.
            line( frame, p, q, line_color, line_thickness );

            // Now draw the tips of the arrow. I do some scaling so that the
            // tips look proportional to the main line of the arrow.
            p.x = ( int )( q.x + 9 * cos( angle + CV_PI / 4 ) );
            p.y = ( int )( q.y + 9 * sin( angle + CV_PI / 4 ) );
            line( frame, p, q, line_color, line_thickness );
            p.x = ( int )( q.x + 9 * cos(angle - CV_PI / 4 ) );
            p.y = ( int )( q.y + 9 * sin(angle - CV_PI / 4 ) );
            line( frame, p, q, line_color, line_thickness );
        }
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
    _dataProvider = std::unique_ptr<StereoCamCvMatProvider>( new StereoCamCvMatProvider( this ) );
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
