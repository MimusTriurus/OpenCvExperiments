#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow( QWidget *parent ) :
    _btnLeftImgsDir     { "Set left imgs dir path" },
    _btnRightImgsDir    { "Set right imgs dir path" },
    _btnOutputDir       { "Set stereo video output dir path" },
    _btnOpenCalibFiles  { "Open calib files:" },
    _btnStart           { "Generate video" },
    _lblLeftImgsDir     { "Dir:" },
    _lblRightImgsDir    { "Dir:" },
    _lblCalibFiles      { "Use calibration:no" },
    _lblOutputDir       { "Dir:" },
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    this->centralWidget( )->setLayout( &_layout );
    _layout.addWidget( &_lblLeftImgsDir );
    _layout.addWidget( &_btnLeftImgsDir );
    _layout.addWidget( &_lblRightImgsDir );
    _layout.addWidget( &_btnRightImgsDir );
    _layout.addWidget( &_lblCalibFiles );
    _layout.addWidget( &_btnOpenCalibFiles );
    _layout.addWidget( &_lblOutputDir );
    _layout.addWidget( &_btnOutputDir );
    _layout.addWidget( &_btnStart );

    this->setWindowTitle( "Generate video from images" );

    connect( &_btnLeftImgsDir, &QPushButton::clicked, [ this ]( ){
        QString targetDirPath( QFileDialog::getExistingDirectory( 0, "Set Left imgs dir path" ) );
        _leftImgsList = getImgsFileInfo( targetDirPath );
        qDebug( ) << "_leftImgs" << _leftImgsList;
        this->_lblLeftImgsDir.setText( "Dir: " + targetDirPath );
    } );
    connect( &_btnRightImgsDir, &QPushButton::clicked, [ this ]( ) {
        QString targetDirPath( QFileDialog::getExistingDirectory( 0, "Set Right imgs dir path" ) );
        _rightImgsList = getImgsFileInfo( targetDirPath );
        this->_lblRightImgsDir.setText( "Dir: " + targetDirPath );
    } );
    connect( &_btnOutputDir, &QPushButton::clicked, [ this ]( ) {
        _stereoVideoDirPath = QFileDialog::getExistingDirectory( 0, "Set video dir path" );
        this->_lblOutputDir.setText( "Dir: " + _stereoVideoDirPath );
    } );

    connect( &_btnOpenCalibFiles, &QPushButton::clicked, [ this ]( ) {
        QString filePath = QFileDialog::getOpenFileName( 0, "Open left calib file" );
        if ( filePath.isEmpty( ) )
            return;
        cv::FileStorage fs1( filePath.toStdString( ), cv::FileStorage::READ );
        fs1[ "K" ] >> _leftIntr;
        fs1[ "D" ] >> _leftDist;
        filePath = QFileDialog::getOpenFileName( 0, "Open right calib file" );
        if ( filePath.isEmpty( ) )
            return;
        cv::FileStorage fs2( filePath.toStdString( ), cv::FileStorage::READ );
        fs2[ "K" ] >> _rightIntr;
        fs2[ "D" ] >> _rightDist;
        _lblCalibFiles.setText( "Use calibration: yes" );
    } );

    connect( &_btnStart, &QPushButton::clicked, [ this ]( ) {
        if ( _stereoVideoDirPath.isEmpty( )
             | ( _leftImgsList.count( ) == 0 )
             | ( _rightImgsList.count( ) == 0 ) ) {
            QMessageBox mess;
            mess.setText( "Error. Set stereoVideoDirPath, LeftImgsDir, RightImgsDir" );
            mess.exec( );
            return;
        }
        int currentFrame = 0;
        cv::Mat lImg = cv::imread( _leftImgsList.at( currentFrame ).absoluteFilePath( ).toStdString( ) );
        cv::Mat rImg;
        cv::VideoWriter leftCapture( QString( _stereoVideoDirPath + "/leftVideo.mp4" ).toStdString( ),
                                     CV_FOURCC( 'X','V','I','D' ), 30, lImg.size( ) );
        cv::VideoWriter rigthCapture( QString( _stereoVideoDirPath + "/rightVideo.mp4" ).toStdString( ),
                                     CV_FOURCC( 'X','V','I','D' ), 30, lImg.size( ) );
        this->setWindowTitle( this->windowTitle( ) + " [in progress...]" );
        while ( currentFrame != _leftImgsList.count( ) ) {
            qDebug( ) << currentFrame << _leftImgsList.at( currentFrame ).absoluteFilePath( );
            lImg = cv::imread( _leftImgsList.at( currentFrame ).absoluteFilePath( ).toStdString( ) );
            cv::Mat lUImg;
            cv::Mat rUImg;
            if ( !_leftDist.empty( ) & !_leftIntr.empty( ) )
                cv::undistort( lImg, lUImg, _leftIntr, _leftDist );
            else
                lUImg = lImg;
            rImg = cv::imread( _rightImgsList.at( currentFrame ).absoluteFilePath( ).toStdString( ) );
            if ( !_rightDist.empty( ) & !_rightIntr.empty( ) )
                cv::undistort( rImg, rUImg, _rightIntr, _rightDist );
            else
                rUImg = rImg;
            currentFrame++;
            leftCapture.write( lUImg );
            rigthCapture.write( rUImg );
        }
        QMessageBox mess;
        mess.setText( "Done." );
        this->setWindowTitle( this->windowTitle( ) + " [Done]" );
        mess.exec( );
    } );
}

MainWindow::~MainWindow( ) {
    delete ui;
}

QFileInfoList MainWindow::getImgsFileInfo( const QString &dirPath ) {
    QDir targetDir( dirPath );
    return targetDir.entryInfoList( QDir::Files, QDir::NoSort );
}
