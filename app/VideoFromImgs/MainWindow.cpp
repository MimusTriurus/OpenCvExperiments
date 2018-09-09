#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMessageBox>
#include "opencv2/opencv.hpp"

MainWindow::MainWindow( QWidget *parent ) :
    _btnLeftImgsDir { "Set left imgs dir path" },
    _btnRightImgsDir{ "Set right imgs dir path" },
    _btnOutputDir   { "Set stereo video output dir path" },
    _btnStart       { "Generate video" },
    _lblLeftImgsDir { "Dir:" },
    _lblRightImgsDir{ "Dir:" },
    _lblOutputDir   { "Dir:" },
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    this->centralWidget( )->setLayout( &_layout );
    _layout.addWidget( &_lblLeftImgsDir );
    _layout.addWidget( &_btnLeftImgsDir );
    _layout.addWidget( &_lblRightImgsDir );
    _layout.addWidget( &_btnRightImgsDir );
    _layout.addWidget( &_lblOutputDir );
    _layout.addWidget( &_btnOutputDir );
    _layout.addWidget( &_btnStart );

    this->setWindowTitle( "Generate video from images" );

    connect( &_btnLeftImgsDir, &QPushButton::clicked, [ this ]( ){
        QString targetDirPath( QFileDialog::getExistingDirectory( 0, "Set Left imgs dir path" ) );
        _leftImgsList = getImgsFileInfo( targetDirPath );
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
    connect( &_btnStart, &QPushButton::clicked, [ this ]( ) {
        if ( _stereoVideoDirPath.isEmpty( ) | _leftImgsList.count( ) == 0 | _rightImgsList.count( ) == 0 ) {
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
            rImg = cv::imread( _rightImgsList.at( currentFrame ).absoluteFilePath( ).toStdString( ) );
            currentFrame++;
            leftCapture.write( lImg );
            rigthCapture.write( rImg );
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
