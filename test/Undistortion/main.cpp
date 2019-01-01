#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include "opencv2/opencv.hpp"

int main( int argc, char *argv[ ] ) {
    QCoreApplication a( argc, argv );

    cv::Mat imgBase = cv::imread( "E:/Projects/Qt/Builds/DisparityMapExperiments/bin/video/framesForVideo/3/left/1403715273262142976.png" );
    cv::Mat outBase;
    QString path{ "E:/Projects/Qt/Builds/DisparityMapExperiments/bin/video/framesForVideo/3/leftCalib.yaml" };
    QFile f( path );
    if ( !f.exists( ) )
        qDebug( ) << "file:" << path << " not found";
    cv::FileStorage fs1( path.toStdString( ), cv::FileStorage::READ );

    cv::Mat intrinsics;
    cv::Mat disCooef;
    fs1[ "K1" ] >> intrinsics;
    fs1[ "D1" ] >> disCooef;

    cv::undistort( imgBase, outBase, intrinsics, disCooef );

    cv::imshow( "input", imgBase );
    cv::imshow( "out", outBase );

    return a.exec( );
}
